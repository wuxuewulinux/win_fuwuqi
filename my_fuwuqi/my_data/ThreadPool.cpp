
#include "ThreadPool.hpp"
#include <stdio.h>
#include <stdlib.h>
void * thread_routine(void* arg)
{
	pthread_detach(pthread_self());   //����ʬ�߳�
	threadpool_t* pool = (threadpool_t*)arg;
	while (1)
	{
		condition_lock(&pool->ready);
		++pool->idle;
		//�ȴ�������������
		while (pool->first == NULL)
		{
			condition_wait(&pool->ready);
		}
		//�ȴ����������ڹ���״̬
		--pool->idle;
		//�ȴ���������
		if (pool->first != NULL)
		{
			//�Ӷ���ͷ��ȡ������������
			task_t* t = pool->first;
			pool->first = t->next;
			//ִ��������Ҫһ����ʱ�����Ա����ͷ������Ա������߳�ʹ�ø���ȥִ������
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
	//����������
	task_t * newtask = (task_t *)malloc(sizeof(task_t));
	newtask->run = run;
	newtask->arg = arg;
	newtask->next = NULL;
	condition_lock(&pool->ready);
	//��������ӵ�����
	if (pool->first == NULL)
	{
		pool->first = newtask;
	}
	else
	{
		pool->last->next = newtask;
	}
	pool->last = newtask;
	//����еȴ��̣߳���������һ��
	if (pool->idle > 0)
	{
		condition_signal(&pool->ready);
	}
	else if (pool->counter < pool->max_threads)
	{
		//û�еȴ��̣߳������߳�������������߳������򴴽�һ�����߳�
		pthread_t tid;
		pthread_create(&tid,NULL,thread_routine,pool);
		++pool->counter;
	}
	
	condition_unlock(&pool->ready);
	return;
}