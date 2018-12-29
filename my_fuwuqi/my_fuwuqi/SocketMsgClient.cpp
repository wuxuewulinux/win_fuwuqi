
#include "SocketMsgClient.hpp"

SocketMsgDBClient::SocketMsgDBClient()
{

}
SocketMsgDBClient::~SocketMsgDBClient()
{

}

SocketMsgDBClient * gm_Msgclient = NULL;

SocketMsgDBClient * SocketMsgDBClient::Instance()
{
	if (NULL == gm_Msgclient)
	{
		gm_Msgclient = new SocketMsgDBClient();
	}

	return gm_Msgclient;

}

int SocketMsgDBClient::CreateSocket(const char * dizi,int duankou)
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
int SocketMsgDBClient::ConnectDB()
{
	if(connect(SocketIo,(struct sockaddr*)&addrCli,sizeof(addrCli)) < 0)
	{
		cout<<"Client Connect msgDB Fail !"<<endl;
		return -1;
	}
	else
	{
		cout<<"Client Connect msgDB Success !"<<endl;
	}
	return 0;
}


int SocketMsgDBClient::GetMapIo(int Key)
{
	map<uint64_t, int>::const_iterator iter = rMap.find(Key);
	if (rMap.end() != iter)
	{
		return iter->second;
	}	
	return -1;
}