
#include "enterfunction.hpp"
#include "lognamespace.hpp"


int ConditionInit()
{
	if (pthread_mutex_init(&readsuo,NULL))
	{
		return -1;
	}
	if (pthread_cond_init(&readcode,NULL))
	{
		return -2;
	}
	if (pthread_mutex_init(&writesuo,NULL))
	{
		return -3;
	}
	if (pthread_cond_init(&writecode,NULL))
	{
		return -4;
	}
	if (pthread_mutex_init(&chulisuo,NULL))
	{
		return -5;
	}
	if (pthread_cond_init(&chulicode,NULL))
	{
		return -6;
	}
	if (pthread_mutex_init(&timesuo,NULL))
	{
		return -7;
	}
	if (pthread_cond_init(&timecode,NULL))
	{
		return -8;
	}

	return 0;
}

