#ifndef _SOCKETKU_HPP_
#define _SOCKETKU_HPP_
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

class Socket_Ku
{
 public:

 Socket_Ku(){listcnfd=0;}
 Socket_Ku(const char * dizi,int duankou);
 int socket_creat();
 int socket_setsockopt();
 int socket_bind();
 int socket_listen();
 int Accept_t();
 int socket_listcnfd(){return listcnfd;}
 ~Socket_Ku(){close(listcnfd);}

 private:

 struct sockaddr_in addr;
 int listcnfd;
};

#endif

