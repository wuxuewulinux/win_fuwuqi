
#include "SocketDBClient.hpp"

SocketDBClient::SocketDBClient()
{

}
SocketDBClient::~SocketDBClient()
{

}

SocketDBClient * gm_client = NULL;

SocketDBClient * SocketDBClient::Instance()
{
	if (NULL == gm_client)
	{
		gm_client = new SocketDBClient();
	}

	return gm_client;

}

int SocketDBClient::CreateSocket(const char * dizi,int duankou)
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
int SocketDBClient::ConnectDB()
{
	if(connect(SocketIo,(struct sockaddr*)&addrCli,sizeof(addrCli)) < 0)
	{
	cout<<"Client Connect DB Fail !"<<endl;
	return -1;
	}
	else
	{
		cout<<"Client Connect DB Success !"<<endl;
	}
	return 0;
}


int SocketDBClient::GetMapIo(int Key)
{
map<uint64_t, int>::const_iterator iter = rMap.find(Key);
if (rMap.end() != iter)
{
	return iter->second;
}	
return -1;
}