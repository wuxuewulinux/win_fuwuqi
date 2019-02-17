
#ifndef _REDISDATA_HPP_
#define _REDISDATA_HPP_

#include<string>
#include<iostream>
#include<stdlib.h>
#include<hiredis/hiredis.h>

#define REDISKU RedisData::Instance()

class RedisData
{
public:
	RedisData();
	~RedisData();

	static RedisData * Instance();


	//��ʼ��redis������redis���ݿ�

	int InitRedis(std::string& Ip,int Port);

	//��mysql����һ���û����ݵ�redis�ϡ��൱�ڸ��»�Ծ�����ݵ�redis

	int InsertUserData(const char* sUid,const char* sName,const char* sRole);


private:

};


#endif