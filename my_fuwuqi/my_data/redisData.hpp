
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


	//初始化redis和连接redis数据库

	int InitRedis(std::string& Ip,int Port);

	//从mysql插入一条用户数据到redis上。相当于更新活跃的数据到redis

	int InsertUserData(const char* sUid,const char* sName,const char* sRole);


private:

};


#endif