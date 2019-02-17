#ifndef _ENTERFUNCTION_HPP_
#define _ENTERFUNCTION_HPP_

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include "quanju.hpp"

void gateway();         //启动服务器。

int ConditionInit();	//条件变量和锁初始化.

bool LogInit(std::string path);				//启动所有日志文件

int GetRoleIo(uint64_t Uid);				//获取一个上线IO

void PushRoleIo(uint64_t Uid,int SockIo);	//压入一个上线IO

void DeleteRoleIo(uint64_t Uid);			//离线就要移除IO


#endif // !_ENTERFUNCTION_HPP_
