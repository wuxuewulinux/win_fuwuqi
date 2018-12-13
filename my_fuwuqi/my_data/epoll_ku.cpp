
#include "epoll_ku.hpp"
#include <iostream>
#include"quanju.hpp"
using namespace std;

Epoll_Ku *gs_Epoll = NULL;

Epoll_Ku * Epoll_Ku::Instance()
{
	if (NULL == gs_Epoll)
	{
		gs_Epoll = new Epoll_Ku();
	}

	return gs_Epoll;
}


int Epoll_Ku::Epoll_Creat()
{
	if((EpollId=epoll_create1(EPOLL_CLOEXEC)) < 0)
	{
		cout<<"Creat Epoll fail !"<<endl;
		return -1;
	}
	nread=-1;
	eventts.resize(100);
	return 0;
}


void Epoll_Ku::Epoll_Add(int conns)
{

	eventt.data.fd=conns;
	eventt.events=EPOLLIN | EPOLLET;
	epoll_ctl(EpollId,EPOLL_CTL_ADD,conns,&eventt);

}

void Epoll_Ku::Epoll_ShanChu(int conns)
{
	eventt.data.fd=conns;
	eventt.events=EPOLLIN | EPOLLET;
	epoll_ctl(EpollId,EPOLL_CTL_DEL,conns,&eventt);

}



void Epoll_Ku::Epoll_Size(int count)
{

	int a=eventts.size()+count;
	eventts.resize(a);

}

void Epoll_Ku::Epoll_Wait(Socket_Ku * socket_t)
{
	int conn,i;
	nread=epoll_wait(EpollId,&(*eventts.begin()),(int)(eventts.size()),-1);
	for(i=0;i<nread;i++)
	{
		if(nread == eventts.size())
			Epoll_Size(eventts.size());
		if(eventts[i].data.fd == socket_t->socket_listcnfd())
		{
			conn=socket_t->Accept_t();
			Epoll_Add(conn);
		}
		else     //把IO压入接收队列中
		{
			if(readduilie.empty() )                 
			{	
				//防止死锁出现
				pthread_mutex_lock(&readsuo);
				pthread_mutex_unlock(&readsuo);
				conn=eventts[i].data.fd;                      
				readduilie.push(conn);               //把该客户端IO压入队列中，让读线程去处理        
				pthread_cond_signal(&readcode);      //发送信号
			}
			else 
			{
				conn=eventts[i].data.fd;                      
				readduilie.push(conn);        //把该客户端IO压入队列中，让读线程去处理
				pthread_cond_signal(&readcode);            
			}

		}

	}
}
