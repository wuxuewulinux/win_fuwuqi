
#ifndef _SOCKETMSGCLIENT_HPP_
#define _SOCKETMSGCLIENT_HPP_

#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <iostream>
#include <map>

using namespace std;

#define MSGCLIENT SocketDBClient::Instance()

class SocketDBClient
{
public:
	SocketDBClient();
	~SocketDBClient();
	static SocketDBClient * Instance();
	int CreateSocket(const char * dizi,int duankou);
	int ConnectDB();
	int GetSocketIo(){return SocketIo;}

	void PushIo(int Io){rMap[rMap.size() + 1] = Io;}	
	int GetMapIo(int Key);
	uint64_t GetMapSize(){return (uint64_t)rMap.size();}
	void DeleteIO(int value){rMap.erase(value);}

private:
	int SocketIo;
	struct sockaddr_in addrCli;
	map<uint64_t,int> rMap;			//用来配合注册和登陆操作的
};




#endif