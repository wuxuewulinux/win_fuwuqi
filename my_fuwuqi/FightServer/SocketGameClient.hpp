
#ifndef _SOCKETDBCLIENT_HPP_
#define _SOCKETDBCLIENT_HPP_

#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <iostream>
#include <map>

using namespace std;

#define GAMECLIENT SocketGameClient::Instance()

class SocketGameClient
{
public:
	SocketGameClient();
	~SocketGameClient();
	static SocketGameClient * Instance();
	int CreateSocket(const char * dizi,int duankou);
	int ConnectGameServer();
	int GetSocketIo(){return SocketIo;}

private:
	int SocketIo;
	struct sockaddr_in addrCli;
};




#endif