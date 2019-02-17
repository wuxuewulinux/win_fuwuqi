
#include "ThreadPool.hpp"
#include <stdio.h>
#include <stdlib.h>
void * thread_routine(void* arg)
{
	pthread_detach(pthread_self());   //处理僵尸线程
	threadpool_t* pool = (threadpool_t*)arg;
	while (1)
	{
		condition_lock(&pool->ready);
		++pool->idle;
		//等待队列有任务到来
		while (pool->first == NULL)
		{
			condition_wait(&pool->ready);
		}
		//等待到条件处于工作状态
		--pool->idle;
		//等待到的任务
		if (pool->first != NULL)
		{
			//从队列头部取出任务来处理
			task_t* t = pool->first;
			pool->first = t->next;
			//执行任务需要一定的时间所以必须释放锁，以便其他线程使用该锁去执行任务
			condition_unlock(&pool->ready);
			t->run(t->arg);
			free(t);
		}
	}
	return NULL;
}

int threadpool_init(threadpool_t* pool,int thread_count)
{
	int Error = condition_init(&pool->ready);
	if (Error)
	{
		return Error;
	}
	pool->first = NULL;
	pool->last = NULL;
	pool->counter = 0;
	pool->idle = 0;
	pool->max_threads = thread_count;
	return 0;
}


void threadpool_add_task(threadpool_t* pool,void* (*run)(void* arg),void* arg)
{
	//生成新任务
	task_t * newtask = (task_t *)malloc(sizeof(task_t));
	newtask->run = run;
	newtask->arg = arg;
	newtask->next = NULL;
	condition_lock(&pool->ready);
	//将任务添加到队列
	if (pool->first == NULL)
	{
		pool->first = newtask;
	}
	else
	{
		pool->last->next = newtask;
	}
	pool->last = newtask;
	//如果有等待线程，则唤醒其中一个
	if (pool->idle > 0)
	{
		condition_signal(&pool->ready);
	}
	else if (pool->counter < pool->max_threads)
	{
		//没有等待线程，但是线程数不超过最大线程数，则创建一个新线程
		pthread_t tid;
		pthread_create(&tid,NULL,thread_routine,pool);
		++pool->counter;
	}
	
	condition_unlock(&pool->ready);
	return;
}