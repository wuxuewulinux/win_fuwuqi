#ifndef _TXTLOG_HPP_
#define _TXTLOG_HPP_

#include <fstream>
#include <iostream>
#include<time.h>
#include<stdio.h>
#include<string.h>
#include<stdarg.h>


class txtlog     //专门管理文件日志信息的。
{

public:

bool Init(std::string filename);       //初始化文件，传入你要把日志内容写入的文件名的路径。-1为初始失败，0为初始成功

void printflog(std::string neirong);   //把日志内容写入到文件中。内容结果是上层逻辑处理，这里只负责写入你传入的内容

void sprintf(char * buffer,const char * format,...);   //按日志格式输入信息到日志文件中

//如果以后需要有特别的内容需要写入，在创建新的写入函数，根据以后的需求

~txtlog(){outf.close();}

private:

std::ofstream outf;                            //写入文件需要的变量
  
time_t timep;                                  //写入时间需要的变量

};

#endif






