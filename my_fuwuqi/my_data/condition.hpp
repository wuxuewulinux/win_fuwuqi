
#ifndef _CONDITION_HPP_
#define _CONDITION_HPP_

#include <pthread.h>

typedef struct condition
{
	pthread_cond_t pcond; 

	pthread_mutex_t pmutex; 

}condition_t;

int condition_init(condition_t* cond);

int condition_lock(condition_t* cond);

int condition_unlock(condition_t* cond);

int condition_wait(condition_t* cond);

int condition_signal(condition_t* cond);

#endif