#include "quanju.hpp"

char BUFF[200];

pthread_cond_t readcode;       //读数据条件变量的变量

pthread_mutex_t readsuo;      //读数据的锁

pthread_t readpthread;      //读数据线程变量

pthread_cond_t writecode;       //写数据条件变量的变量

pthread_mutex_t writesuo;      //写数据的锁

pthread_t writepthread;      //写数据线程变量

pthread_cond_t chulicode;       //解析处理数据包需要的条件变量的变量

pthread_mutex_t chulisuo;      //解析处理数据包需要的锁

pthread_t chulipthread;      //解析处理数据包需要的线程变量

pthread_cond_t timecode;       //处理定时器需要的条件变量的变量

pthread_mutex_t timesuo;      //处理定时器需要的锁

pthread_t timepthread;      //处理定时器需要的线程变量

std::queue<int> readduilie;             //使用一个队列进行对数据读的操作，队列的数据就是 IO。队列有IO就证明该IO来数据了，要进行处理

std::queue<shuji> messageduilie;       //把读到客户端发过来的数据包字节存到队列中，让另外一个线程去解析该数据包

std::queue<sendshuji> sendduilie;      //把要发送给客户端的数据保存到队列中

std::queue<struct timeduilie> timequeue;    //处理定时时间的结构保存到一个定时队列中，让定时器线程去处理所有定时时间

//Epoll_Ku epoll_lei;              //声明一个epoll全局类

txtlog MYLOG;                    //声明一个记录错误日志的全局类。

timeku timelei;                //声明一个时间库类，专门管理所有的定时时间变化

std::map<uint64_t,int> rUidMap;		//保存所有上线的列表


