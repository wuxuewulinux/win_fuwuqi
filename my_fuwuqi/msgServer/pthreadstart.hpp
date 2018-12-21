#ifndef _PTHREADSTART_HPP_
#define _PTHREADSTART_HPP_

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>

void * readxiancheng(void *canshu);      //处理read客户端数据的线程执行函数

void * chulimessage(void *canshu);       //解析数据包然后转发到对应功能模块的线程执行函数

void * writexiancheng(void *canshu);     //把要发送给客户端的数据让写线程去处理发送

void * timexiancheng(void * canshu);     //定时器执行线程

void ChuLiAgemess(struct message * message,int * io);      //处理识别执行哪个功能模块入口

int pthreadstart();     //所有线程的启动入口



#endif // !_PTHREADSTART_HPP_
