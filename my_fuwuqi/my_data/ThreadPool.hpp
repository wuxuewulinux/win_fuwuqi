#ifndef _THREADPOOL_HPP_
#define _THREADPOOL_HPP_

#include "condition.hpp"

//任务结构体，将任务放入队列由线程池中的线程来执行
typedef struct task
{
	void* (*run)(void* arg);		//任务的执行函数
	void* arg;						//传入任务执行函数的参数
	struct task* next;				

}task_t;

typedef struct threadpool
{
	condition_t ready;			//条件变量和互斥锁
	task_t* first;				//任务队列头指针
	task_t* last;				//任务队列尾指针
	int counter;				//线程池中当前线程数
	int idle;					//线程池中当前正在等待任务的线程数
	int max_threads;			//线程池中最大允许的线程数
}threadpool_t;

//初始化线程池
int threadpool_init(threadpool_t* pool,int thread_count);
//往线程池中添加任务
int threadpool_add_task(threadpool_t* pool,void* (*run)(void* arg),void* arg);


#endif
