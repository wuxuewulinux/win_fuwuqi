
#include "redisData.hpp"

RedisData::RedisData()
{


}


RedisData::~RedisData()
{


}


RedisData *gs_RedisData = NULL;


RedisData * RedisData::Instance()
{
	if (NULL == gs_RedisData)
	{
		gs_RedisData = new RedisData();
	}

	return gs_RedisData;
}



int RedisData::InitRedis(std::string& Ip,int Port)
{


	return 0;
}


int RedisData::InsertUserData(const char* sUid,const char* sName,const char* sRole)
{


	return 0;
}
