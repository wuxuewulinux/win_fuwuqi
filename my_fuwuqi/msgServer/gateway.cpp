

#include"logicconfigmanager.hpp"
#include "lognamespace.hpp"
#include "enterfunction.hpp"
#include "socketku.hpp"
#include "pthreadstart.hpp"
#include "epoll_ku.hpp"
#include "HandlerList.hpp"
#include "MysqlKu.hpp"

void gateway()
{
int litenfd;
//��ʼ����Ϸģ�� ��־
if(!LogInit("/home/wuxuewu/msgServer/"))
{
return;
}
//��ʼ����¼������Ϣ����־
if(!MYLOG.Init("/home/wuxuewu/msgServer/log/error.txt"))
{
std::cout<<"error log create fail !"<<std::endl;
return;
}
//��ʼ������ģ���handler
HANDLERLIST->Init();
//���������̣߳��� д ������ ��ʱ�� ���ĸ��̣߳�
if(pthreadstart() < 0)
 return;

//��ʼ��ȡ��Ϸ����
if(!LOGIC_CONFIG->Init("/home/wuxuewu/msgServer/config_xml/"))
	return;

//��ʼ����mysql���ݿ�ͳ�ʼ��mysql����
struct my_mysql * pMysql = LOGIC_CONFIG->GetMsgServerConfig().GetMysqlConfig();	//��ȡmysql���ݿ������Ϣ
if(MYSQLKU->InitMysql(pMysql->user,pMysql->mima,pMysql->database,pMysql->table) < 0)//�������ݿ�
	return;
const struct msgServer * test = LOGIC_CONFIG->GetMsgServerConfig().GetMsgServerConfig(); //��ȡ������IP�Ͷ˿�
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
