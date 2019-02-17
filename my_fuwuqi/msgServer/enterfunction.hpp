#ifndef _ENTERFUNCTION_HPP_
#define _ENTERFUNCTION_HPP_

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include "quanju.hpp"

void gateway();         //������������

int ConditionInit();	//��������������ʼ��.

bool LogInit(std::string path);				//����������־�ļ�

int GetRoleIo(uint64_t Uid);				//��ȡһ������IO

void PushRoleIo(uint64_t Uid,int SockIo);	//ѹ��һ������IO

void DeleteRoleIo(uint64_t Uid);			//���߾�Ҫ�Ƴ�IO


#endif // !_ENTERFUNCTION_HPP_
