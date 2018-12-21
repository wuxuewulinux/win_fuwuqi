
#ifndef _TIMEKU_HPP_
#define _TIMEKU_HPP_

#include<vector>
#include<sys/time.h>
#include<algorithm>
#include<stack>
#include<iostream>

struct timenode
{

void (*p)(void *canshu);     //超时就触发执行该函数

void *data;                  //该指针变量是要传入触发执行函数中的参数值,大多数指向全局容器的某个变量地址上

long endtime;                 //规定的系统时间内触发，以毫秒为单位，系统时间加上你要触发的时间，就是最后的时间

int uid;                   //保存客户端uid,每个用户对应一个uid，可以用来区别对应用户时间节点。

};


struct fuzhunode                 //遍历函数返回该结构体，返回的是最短的触发时间 
{
long timeid;                     //触发时间

struct timenode chufanode;                //触发时间对应的时间节点
};

bool paixu(const fuzhunode & a,const fuzhunode & b);   //计算排序，主要是排序出哪个时间触发比较短。



uint64_t get_time();     //获取目前系统的时间，时间以毫秒为单位




//该类是以棋牌的打牌定时为标准来设计的。

class timeku
{

	public:

           timeku();
	
      bool pushtime(int index,int miao,int user_id);         //该函数刷新用户的定时时间，重置为初始时间,轮到该用户打牌时就执行该函数

      bool poptime(int index,int user_id);                  //当用户执行打牌完就要重置为0状态。
                                                 
      int startgame(int user_id,void (*p)(void * canshu),void * data_t);       //每局游戏刚开始时就要分配一个时间内存给用户，返回值为时间节点下标，每个内存对应一个下标                                                                                   值,user_id是用户的uid
      
      bool endgame(int index,int user_id);             //每局游戏结束就要释放时间节点下标
 		
      
      struct fuzhunode bianli();                                  //遍历容器，取出最短的时间节点,返回值为毫秒。返回剩余多少毫秒后就触发条件。 

      struct timespec chufatime(long haomiao);               //把最短的触发时间换算成系统的等待调用时间，也就是超时处理时间，返回值为系统触发等待结构体，参数为最短触发
                                                             //long haomiao参数为最短触发时间，以毫秒为单位     
 
	public:

	std::vector<struct timenode> rongqi;    //用一个动态数组容器去管理所有的time模块

        std::stack<int> index_stack;           //该栈是用来保存空闲时间节点容器的下标值。

//startgame和endgame函数用在棋牌游戏逻辑上，push和bianli和chufatime用在定时器执行线程上
//poptime函数放在游戏逻辑打牌还要超时执行函数打牌上
//注意点：容器下标每当游戏开始，每个用户游戏逻辑类就会保存该容器下标数值

};

#endif











