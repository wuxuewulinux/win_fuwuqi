#ifndef _PTHREADSTART_HPP_
#define _PTHREADSTART_HPP_

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <stdio.h>

void * readxiancheng(void *canshu);      //����read�ͻ������ݵ��߳�ִ�к���

void * chulimessage(void *canshu);       //�������ݰ�Ȼ��ת������Ӧ����ģ����߳�ִ�к���

void * writexiancheng(void *canshu);     //��Ҫ���͸��ͻ��˵�������д�߳�ȥ��������

void * timexiancheng(void * canshu);     //��ʱ��ִ���߳�

void ChuLiAgemessClient(struct message * message,int * io);      //����ʶ��ִ���ĸ�����ģ�����(�ͻ�������)

void ChuLiAgemessServer(struct message * message,int * io);		//���������������ݿ�ת��

int pthreadstart();     //�����̵߳��������



#endif // !_PTHREADSTART_HPP_