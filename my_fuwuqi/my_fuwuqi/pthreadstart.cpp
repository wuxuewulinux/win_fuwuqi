
#include "quanju.hpp"
#include "pthreadstart.hpp"
#include <errno.h>
#include "HandlerList.hpp"
#include "SocketDBClient.hpp"
#include "epoll_ku.hpp"

const int MAX_BUFF=255;                  //设置数据包的最大范围

//读线程的执行代码

void * readxiancheng(void *canshu)
{

	pthread_detach(pthread_self());   //处理僵尸线程
	int sockio,ret;
	struct shuji agemess;           //这个结构是数据包结构，序列号需要该结构才能序列化成功
	while(1)
	{

		while(readduilie.empty())  //队列为空将等待
		{
			pthread_cond_wait(&readcode,&readsuo);
		}

		while(!readduilie.empty())   //如果队列有数据将要处理
		{
			sockio=readduilie.front();
			agemess.io=sockio;
			ret=readline(sockio,&agemess.mess,MAX_BUFF);     //开始对该IO的数据进行读取
			if(ret == -2)
			{      
				std::cout<<"read error message!"<<std::endl;
				MYLOG.printflog("read error message!");
				//如果数据发生错误，就需要对该IO进行处理

			}
			if(ret == -1)
			{
				EPOLLKU->Epoll_ShanChu(sockio);
				std::cout<<"Client Quit !"<<std::endl;
				//如果收到客户端退出，就需要对该IO进行处理
			}	
			if(ret > 0 && messageduilie.empty() )                 //证明有一个完整包了 
			{
				//防止死锁出现
				pthread_mutex_lock(&chulisuo);
				pthread_mutex_unlock(&chulisuo);
				messageduilie.push(agemess);        //把接收到的数据包压入队列中，送给别的模块去解析和处理该数据包
				pthread_cond_signal(&chulicode);  //发送信号
			}
			else if(ret > 0)       //有一个数据包，但是队列不为空
			{
				messageduilie.push(agemess);
				pthread_cond_signal(&chulicode);            
			}
			readduilie.pop();                    //处理完的IO把他从队列删除掉
			memset(&agemess,0,sizeof(agemess));
		}

	}
	return NULL;
}



//工作线程的执行函数

void ChuLiAgemessClient(struct message * message,int * io)
{
	CSMsg mess_ti;

	//反序列化数据包

	if (!mess_ti.ParseFromArray(message->buff, message->len))
	{
		std::cout << " Deserialization ChuLiAgemessClient Fali ！" << std::endl;
		MYLOG.printflog("反序列失败！");     
		return;
	}
	//获取模块入口
	if (CS_MSGID_MIN < mess_ti.head().msgid() && mess_ti.head().msgid() < CS_MSGID_MAX)
	{
		IHandler* handler = HANDLERLIST->GetHandler(mess_ti.head().msgid());
		//执行模块功能
		handler->OnClientMsg(mess_ti,*io);
	}
}



void ChuLiAgemessServer(struct message * message,int * io)
{
	SSMsg mess_ti;

	//反序列化数据包

	if (!mess_ti.ParseFromArray(message->buff, message->len))
	{
		std::cout << "Deserialization ChuLiAgemessServer Fali ！！" << std::endl;
		MYLOG.printflog("反序列失败！");     
		return;
	}
	if (SS_MSGID_MIN < mess_ti.head().msgid() && mess_ti.head().msgid() < SS_MSGID_MAX)
	{
		IHandler* handler = HANDLERLIST->GetHandler(mess_ti.head().msgid());
		//执行模块功能
		handler->OnServerMsg(mess_ti);		//游戏服务器主动向别人转发数据，然后别的服务器返回就调用OnServerMsg
	}
}


void * chulimessage(void *canshu)
{
	pthread_detach(pthread_self());   //处理僵尸线程
	struct shuji test;
	while(1)
	{

		while(messageduilie.empty())  //队列为空将等待
		{
			pthread_cond_wait(&chulicode,&chulisuo);
		}

		while(!messageduilie.empty())   //如果队列有数据将要处理
		{
			test=messageduilie.front();
			if(DBCLIENT->GetSocketIo() == test.io)
				ChuLiAgemessServer(&test.mess,&test.io);
			else
			    ChuLiAgemessClient(&test.mess,&test.io);      //该函数负责解析数据和转发到对应的功能模块中    

			messageduilie.pop();         //处理完的消息就要从队列去除          


		}	

	}

	return NULL;
}



//写线程的执行函数

void * writexiancheng(void *canshu)
{
	pthread_detach(pthread_self());   //处理僵尸线程
	struct sendshuji send;  
	while(1)
	{
		while(sendduilie.empty())  //队列为空将等待
		{
			pthread_cond_wait(&writecode,&writesuo);
		}
		while(!sendduilie.empty())   //如果队列有数据就要处理把该数据发送出去
		{
			send=sendduilie.front();
			int value=my_write(send.io,(const char *)&send.rMessageBody,send.len);    //向客户端发送一个数据包
			if(value<0)
			{
				//客户端关闭连接，处理关闭连接的操作
			}
			sendduilie.pop();                    //插入的数据，将必须从队列删除

		}	

	}
	return NULL;
}




//定时器线程执行函数

void * timexiancheng(void * canshu) //定时器执行线程
{
	int a;
	struct timeduilie test;
	struct fuzhunode node;
	node.timeid=0;
	while(1)
	{
		while(!timequeue.empty())  //如果队列不为空就一直遍历时间
		{
			test=timequeue.front();
			if(test.kaiguan == true)  //压入时间操作
			{
				timelei.pushtime(test.index,test.miao,test.userid);   //压入一个时间节点到时间管理器中                        
				timequeue.pop();
				if(timequeue.empty())
					node = timelei.bianli();   //遍历时间管理器查看哪些时间是最短触发
			}
			else     //删除时间操作
			{
				timelei.poptime(test.index,test.userid);                    
				timequeue.pop();
				if(timequeue.empty())
					node = timelei.bianli();
			}
		}
		while(timequeue.empty())  //如果队列为空就一次遍历时间，取出超时时间
		{
			while(node.timeid > 0)    //时间大于0证明有时间节点
			{
				struct timespec outtime;
				outtime = timelei.chufatime(node.timeid);
				a = pthread_cond_timedwait(&timecode,&timesuo,&outtime);    //有时间节点，阻塞等待超时事件
				if(a == ETIMEDOUT)   //该条件成立，证明该时间节点超时了
				{
					node.chufanode.p(node.chufanode.data);
					break;               //执行该节点触发条件
				}
				else
					break;
			}
			while(node.timeid == 0)  //时间等于0证明容器没有时间节点，阻塞等待着
			{
				pthread_cond_wait(&timecode,&timesuo);
			}
			if(timequeue.empty())
				node = timelei.bianli();
		}
	}
	return NULL;
} 



//所有线程的启动

int pthreadstart()
{
//开读数据包线程
pthread_mutex_init(&readsuo,NULL);   //创建一把锁
pthread_cond_init(&readcode,NULL);    //创建一个条件变量
int ret=pthread_create(&readpthread,NULL,readxiancheng,NULL);   //创建一个线程
if(ret != 0)
{
std::cout<<"create read pthread fail!"<<std::endl;
MYLOG.printflog("create read pthread fail!");
return -1;
}

//开处理数据包线程
pthread_mutex_init(&chulisuo,NULL);   //创建一把锁
pthread_cond_init(&chulicode,NULL);    //创建一个条件变量
ret=pthread_create(&chulipthread,NULL,chulimessage,NULL);   //创建一个线程
if(ret != 0)
{
std::cout<<"create work  pthread fail!"<<std::endl;
MYLOG.printflog("create work  pthread fail!");
return -1;
}

//开写数据线程
pthread_mutex_init(&writesuo,NULL);   //创建一把锁
pthread_cond_init(&writecode,NULL);    //创建一个条件变量
ret=pthread_create(&writepthread,NULL,writexiancheng,NULL);   //创建一个线程
if(ret != 0)
{
std::cout<<"create write  pthread fail!"<<std::endl;
MYLOG.printflog("create write  pthread fail!");
return -1;
}


//开定时器检测线程
pthread_mutex_init(&timesuo,NULL);   //创建一把锁
pthread_cond_init(&timecode,NULL);    //创建一个条件变量
ret=pthread_create(&timepthread,NULL,timexiancheng,NULL);   //创建一个线程
if(ret != 0)
{
std::cout<<"create time  pthread fail!"<<std::endl;
MYLOG.printflog("create time  pthread fail!");
return -1;
}

return 0;

  }


