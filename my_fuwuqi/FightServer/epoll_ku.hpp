#ifndef EPOLL_KU_H_
#define EPOLL_KU_H_

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<vector>
#include <sys/epoll.h>
#include"socketku.hpp"

#define EPOLLKU Epoll_Ku::Instance()

class Epoll_Ku
{
  public:

static Epoll_Ku * Instance();
Epoll_Ku(){EpollId=0;} 
int Epoll_Creat();
void Epoll_Size(int count);
void Epoll_Add(int conns);
void Epoll_ShanChu(int conns);
void Epoll_Wait(Socket_Ku * socket_t);
int fanhui_nread(){return nread;}
struct epoll_event * fanhui_event(){return &(*eventts.begin());}
int * Epoll_epollid(){return &EpollId;}
~Epoll_Ku(){}

private:

int EpollId;
int nread;
struct epoll_event eventt;
std::vector<struct epoll_event> eventts;

};


#endif

