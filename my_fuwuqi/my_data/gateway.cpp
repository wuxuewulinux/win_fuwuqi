

#include"logicconfigmanager.hpp"
#include "lognamespace.hpp"
#include "enterfunction.hpp"
#include "socketku.hpp"
#include "pthreadstart.hpp"
#include "epoll_ku.hpp"
#include "HandlerList.hpp"
#include "MysqlKu.hpp"
#include "BillMysql.hpp"


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

void gateway()
{
int litenfd;
//��ʼ����Ϸģ�� ��־
if(!LogInit("/home/wuxuewu/datamysql/"))
{
return;
}
//��ʼ����¼������Ϣ����־
if(!MYLOG.Init("/home/wuxuewu/datamysql/log/error.txt"))
{
std::cout<<"error log create fail !"<<std::endl;
return;
}
//��ʼ������ģ���handler
HANDLERLIST->Init();
//��ʼ������������������
int Error;
Error = ConditionInit();
if (Error < 0)
{
	std::cout<<"ConditionInit fail !"<<Error<<std::endl;
	return;
}
//���������̣߳��� д ������ ��ʱ�� ���ĸ��̣߳�
if(pthreadstart() < 0)
 return;
/*
//��ʼ���̳߳أ����ֻ�ܴ�������߳�
if(threadpool_init(&pool_t,5) < 0)
	return;
	*/
//��ʼ��ȡ��Ϸ����
if(!LOGIC_CONFIG->Init("/home/wuxuewu/datamysql/config_xml/"))
	return;

//��ʼ����(�û����ݿ�)xiangmu���ݿ�ͳ�ʼ��mysql����
struct my_mysql * pMysql = LOGIC_CONFIG->GetServerMysqlConfig().GetMysqlConfig();	//��ȡmysql���ݿ������Ϣ
if(MYSQLKU->InitMysql(pMysql->user,pMysql->mima,pMysql->database,pMysql->table) < 0)//�������ݿ�
	return;
//��ʼ����(�û��˵���ˮ���ݿ�)ZhangDan���ݿ�ͳ�ʼ��BillMysql����
struct BillMysql * pBillMysql = LOGIC_CONFIG->GetServerMysqlConfig().GetBillMysqlConfig();	//��ȡmysql���ݿ������Ϣ
if(BILLMYSQLKU->InitBillMysql(pBillMysql->user,pBillMysql->mima,pBillMysql->database,pBillMysql->BillTable) < 0)		//�������ݿ�
	return;
const struct MysqlServer * test = LOGIC_CONFIG->GetServerMysqlConfig().GetMysqlServerConfig(); //��ȡmysql������IP�Ͷ˿�
// TCP/IP����
Socket_Ku socket_lei(test->ip.c_str(),test->port);
if(socket_lei.socket_creat() < 0)
	return;
if(socket_lei.socket_setsockopt() < 0)
	return;
if(socket_lei.socket_bind() < 0)
	return;
if(socket_lei.socket_listen() < 0)
	return;
litenfd=socket_lei.socket_listcnfd();   //��ȡ����������IO
//epollģ������
if(EPOLLKU->Epoll_Creat() < 0)
	return;
EPOLLKU->Epoll_Add(litenfd);

while(1)
{
 EPOLLKU->Epoll_Wait(&socket_lei);   //epoll���IO�¼�

}

return;

}
