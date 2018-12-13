#ifndef _ENTERFUNCTION_HPP_
#define _ENTERFUNCTION_HPP_

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include "quanju.hpp"

void gateway();         //������������

bool LogInit(std::string path);       //����������־�ļ�

CRoleObj * GetRole(uint64_t Uid);

void PushRole(uint64_t Uid,CRoleObj* rRole);

void DeleteRole(uint64_t Uid);

#endif // !_ENTERFUNCTION_HPP_
