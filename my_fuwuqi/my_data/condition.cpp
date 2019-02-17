
#include "condition.hpp"

int condition_init(condition_t* cond)
{
	int status;
	if ((status = pthread_mutex_init(&cond->pmutex,NULL)))
	{
		return status;
	}
	if ((status = pthread_cond_init(&cond->pcond,NULL)))
	{
		return status;
	}
	return 0;
}

int condition_lock(condition_t* cond)
{
	return pthread_mutex_lock(&cond->pmutex);
}

int condition_unlock(condition_t* cond)
{
	return pthread_mutex_unlock(&cond->pmutex);
}

int condition_wait(condition_t* cond)
{
	return pthread_cond_wait(&cond->pcond,&cond->pmutex);
}

int condition_signal(condition_t* cond)
{
	return pthread_cond_signal(&cond->pcond);
}
