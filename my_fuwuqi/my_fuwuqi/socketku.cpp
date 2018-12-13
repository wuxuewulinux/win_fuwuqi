
#include "socketku.hpp"
#include "quanju.hpp"

Socket_Ku::Socket_Ku(const char * dizi,int duankou)
{
	addr.sin_family=AF_INET;
	addr.sin_port=htons(duankou);
	addr.sin_addr.s_addr=inet_addr(dizi);
	listcnfd=0;
}


int Socket_Ku::socket_creat()
{

	if((listcnfd=socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		std::cout<<"create TCP/IP fail!"<<std::endl;
		MYLOG.printflog("create TCP/IP fail!");
		return -1;
	}
	else
	{std::cout<<"TCP/IP create success!"<<std::endl;}
	return listcnfd;
}


int Socket_Ku::socket_setsockopt()
{

	int on=1;
	if(setsockopt(listcnfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)) < 0)
	{ std::cout<<"TCP/IP dizi  fail!"<<std::endl;
	MYLOG.printflog("TCP/IP dizi  fail!");
	return -1;
	}
	return 0;
}


int Socket_Ku::socket_bind()
{

	if(bind(listcnfd,(struct sockaddr *)&addr,sizeof(addr)) < 0)
	{ std::cout<<"TCP/IP bind fail!"<<std::endl;
	MYLOG.printflog("TCP/IP bind fail!");
	return -1;
	}
	else
	{ std::cout<<"TCP/IP bind success!"<<std::endl;}

	return 0;
}

int Socket_Ku::socket_listen()
{

	if(listen(listcnfd,SOMAXCONN) < 0)
	{
		std::cout<<"TCP/IP listen fail!"<<std::endl;
		MYLOG.printflog("TCP/IP listen fail!");
		return -1;
	}
	return 0;
}


int Socket_Ku::Accept_t()
{
	int taojiezi;
	struct sockaddr_in addr_t;
	socklen_t zijieshu;
	zijieshu=sizeof(addr_t);
	if((taojiezi=accept(listcnfd,(struct sockaddr *)&addr_t,&zijieshu)) < 0)
	{
		std::cout<<"TCP/IP Accept fail!"<<std::endl;
		MYLOG.printflog("TCP/IP Accept fail!");
		return -1;
	}
	std::cout<<inet_ntoa(addr_t.sin_addr)<<" "<<ntohs(addr_t.sin_port)<<std::endl;
	return taojiezi;
}
