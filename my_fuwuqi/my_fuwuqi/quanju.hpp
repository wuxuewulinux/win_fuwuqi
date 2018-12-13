#ifndef _QUANJU_HPP_
#define _QUANJU_HPP_

//该头文件定义存储日志线程所需要的准备资源
//
#include <pthread.h>
#include <queue>
#include <unistd.h>
#include <iostream>
#include <map>
#include "readn.hpp"
#include "txtlog.hpp"
#include "timeku.hpp"
#include "../proto/CSmsg.pb.h"
#include "RoleObj.hpp"

//判断指针的宏函数
#define HANDCHECH_P(a,b) \
if (a == NULL)\
{\
	return b;\
}\

#define HANDCHECH_I(a,b) \
	if (a < 0 )\
{\
	return b;\
}\


struct shuji
{

	int io;

	struct message mess;

};

struct sendshuji     //序列化数据然后保存到线程队列中在发送给客户端
{

	int io;

	int length;                         //该数据的长度

	char buff[255];

};

struct timeduilie       //定时器需要保存的结构体
{
	bool kaiguan;    //如果true为增加时间（push操作），如果false为去掉时间（pop操作）
	int index;
	int miao;
	int userid;
};

	extern char BUFF[200];

	extern pthread_cond_t readcode;       //读数据条件变量的变量

	extern pthread_mutex_t readsuo;      //读数据的锁

	extern pthread_t readpthread;      //读数据线程变量

	extern pthread_cond_t writecode;       //写数据条件变量的变量

	extern pthread_mutex_t writesuo;      //写数据的锁

	extern pthread_t writepthread;      //写数据线程变量

	extern pthread_cond_t chulicode;       //解析处理数据包需要的条件变量的变量

	extern pthread_mutex_t chulisuo;      //解析处理数据包需要的锁

	extern pthread_t chulipthread;      //解析处理数据包需要的线程变量

	extern pthread_cond_t timecode;       //处理定时器需要的条件变量的变量

	extern pthread_mutex_t timesuo;      //处理定时器需要的锁

	extern pthread_t timepthread;      //处理定时器需要的线程变量

	extern std::queue<int> readduilie;             //使用一个队列进行对数据读的操作，队列的数据就是 IO。队列有IO就证明该IO来数据了，要进行处理

	extern std::queue<shuji> messageduilie;       //把读到客户端发过来的数据包字节存到队列中，让另外一个线程去解析该数据包

	extern std::queue<sendshuji> sendduilie;      //把要发送给客户端的数据保存到队列中

	extern std::queue<struct timeduilie> timequeue;    //处理定时时间的结构保存到一个定时队列中，让定时器线程去处理所有定时时间

	extern std::map<int,CRoleObj> mRoleList;		//保存全部在线的用户role

	extern txtlog MYLOG;                    //声明一个记录错误日志的全局类。

	extern timeku timelei;                //声明一个时间库类，专门管理所有的定时时间变化

	

#endif