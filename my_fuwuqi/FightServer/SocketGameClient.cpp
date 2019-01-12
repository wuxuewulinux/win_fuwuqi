
#include "SocketGameClient.hpp"

SocketGameClient::SocketGameClient()
{

}
SocketGameClient::~SocketGameClient()
{

}

SocketGameClient * gm_client = NULL;

SocketGameClient * SocketGameClient::Instance()
{
	if (NULL == gm_client)
	{
		gm_client = new SocketGameClient();
	}

	return gm_client;

}

int SocketGameClient::CreateSocket(const char * dizi,int duankou)
{
	if((SocketIo = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
	cout<<"Create Socket Fail!"<<endl;
	return -2;
	}
	addrCli.sin_family = AF_INET;
	addrCli.sin_port = htons(duankou);
	addrCli.sin_addr.s_addr = inet_addr(dizi);
	return 0;
}
int SocketGameClient::ConnectGameServer()
{
	if(connect(SocketIo,(struct sockaddr*)&addrCli,sizeof(addrCli)) < 0)
	{
	cout<<"Client Connect GameServer Fail !"<<endl;
	return -1;
	}
	else
	{
		cout<<"Client Connect GameServer Success !"<<endl;
	}
	return 0;
}


