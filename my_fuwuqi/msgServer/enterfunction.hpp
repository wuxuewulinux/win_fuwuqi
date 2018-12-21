#ifndef _ENTERFUNCTION_HPP_
#define _ENTERFUNCTION_HPP_

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include "quanju.hpp"

void gateway();         //启动服务器。

bool LogInit(std::string path);       //启动所有日志文件


#endif // !_ENTERFUNCTION_HPP_
