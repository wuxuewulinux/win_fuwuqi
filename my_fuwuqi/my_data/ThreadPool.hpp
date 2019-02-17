#ifndef _THREADPOOL_HPP_
#define _THREADPOOL_HPP_

#include "condition.hpp"

//����ṹ�壬���������������̳߳��е��߳���ִ��
typedef struct task
{
	void* (*run)(void* arg);		//�����ִ�к���
	void* arg;						//��������ִ�к����Ĳ���
	struct task* next;				

}task_t;

typedef struct threadpool
{
	condition_t ready;			//���������ͻ�����
	task_t* first;				//�������ͷָ��
	task_t* last;				//�������βָ��
	int counter;				//�̳߳��е�ǰ�߳���
	int idle;					//�̳߳��е�ǰ���ڵȴ�������߳���
	int max_threads;			//�̳߳������������߳���
}threadpool_t;

//��ʼ���̳߳�
int threadpool_init(threadpool_t* pool,int thread_count);
//���̳߳����������
int threadpool_add_task(threadpool_t* pool,void* (*run)(void* arg),void* arg);


#endif
