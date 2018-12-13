#ifndef _QUANJU_HPP_
#define _QUANJU_HPP_

//��ͷ�ļ�����洢��־�߳�����Ҫ��׼����Դ
//
#include <pthread.h>
#include <queue>
#include <unistd.h>
#include <iostream>
#include <map>
#include "readn.hpp"
#include "txtlog.hpp"
#include "timeku.hpp"
#include "../proto/CSmsg.pb.h"
#include "RoleObj.hpp"

//�ж�ָ��ĺ꺯��
#define HANDCHECH_P(a,b) \
if (a == NULL)\
{\
	return b;\
}\

#define HANDCHECH_I(a,b) \
	if (a < 0 )\
{\
	return b;\
}\


struct shuji
{

	int io;

	struct message mess;

};

struct sendshuji     //���л�����Ȼ�󱣴浽�̶߳������ڷ��͸��ͻ���
{

	int io;

	int length;                         //�����ݵĳ���

	char buff[255];

};

struct timeduilie       //��ʱ����Ҫ����Ľṹ��
{
	bool kaiguan;    //���trueΪ����ʱ�䣨push�����������falseΪȥ��ʱ�䣨pop������
	int index;
	int miao;
	int userid;
};

	extern char BUFF[200];

	extern pthread_cond_t readcode;       //���������������ı���

	extern pthread_mutex_t readsuo;      //�����ݵ���

	extern pthread_t readpthread;      //�������̱߳���

	extern pthread_cond_t writecode;       //д�������������ı���

	extern pthread_mutex_t writesuo;      //д���ݵ���

	extern pthread_t writepthread;      //д�����̱߳���

	extern pthread_cond_t chulicode;       //�����������ݰ���Ҫ�����������ı���

	extern pthread_mutex_t chulisuo;      //�����������ݰ���Ҫ����

	extern pthread_t chulipthread;      //�����������ݰ���Ҫ���̱߳���

	extern pthread_cond_t timecode;       //����ʱ����Ҫ�����������ı���

	extern pthread_mutex_t timesuo;      //����ʱ����Ҫ����

	extern pthread_t timepthread;      //����ʱ����Ҫ���̱߳���

	extern std::queue<int> readduilie;             //ʹ��һ�����н��ж����ݶ��Ĳ��������е����ݾ��� IO��������IO��֤����IO�������ˣ�Ҫ���д���

	extern std::queue<shuji> messageduilie;       //�Ѷ����ͻ��˷����������ݰ��ֽڴ浽�����У�������һ���߳�ȥ���������ݰ�

	extern std::queue<sendshuji> sendduilie;      //��Ҫ���͸��ͻ��˵����ݱ��浽������

	extern std::queue<struct timeduilie> timequeue;    //����ʱʱ��Ľṹ���浽һ����ʱ�����У��ö�ʱ���߳�ȥ�������ж�ʱʱ��

	extern std::map<int,CRoleObj> mRoleList;		//����ȫ�����ߵ��û�role

	extern txtlog MYLOG;                    //����һ����¼������־��ȫ���ࡣ

	extern timeku timelei;                //����һ��ʱ����࣬ר�Ź������еĶ�ʱʱ��仯

	

#endif