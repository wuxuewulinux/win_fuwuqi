

#include"logicconfigmanager.hpp"
#include "lognamespace.hpp"
#include "enterfunction.hpp"
#include "socketku.hpp"
#include "pthreadstart.hpp"
#include "epoll_ku.hpp"
#include "HandlerList.hpp"
#include "SocketDBClient.hpp"

void gateway()
{
int litenfd;
//��ʼ��ȡ��Ϸ����
if(!LOGIC_CONFIG->Init("/home/wuxuewu/fuwuqi/config_xml/"))
	return;
//����mysql������
const struct MysqlServer * mMysqlServer = LOGIC_CONFIG->GetServerMysqlConfig().GetMysqlServerConfig(); //��ȡmysql������IP�Ͷ˿�
if(DBCLIENT->CreateSocket(mMysqlServer->ip.c_str(),mMysqlServer->port) < 0)
	return;
if(DBCLIENT->ConnectDB() < 0)
	return;
//��ʼ����Ϸģ�� ��־
if(!LogInit("/home/wuxuewu/fuwuqi/"))
{
return;
}
//��ʼ����¼������Ϣ����־
if(!MYLOG.Init("/home/wuxuewu/fuwuqi/log/error.txt"))
{
std::cout<<"error log create fail !"<<std::endl;
return;
}
//��ʼ������ģ���handler
HANDLERLIST->Init();
//���������̣߳��� д ������ ��ʱ�� ���ĸ��̣߳�
if(pthreadstart() < 0)
 return;
const struct my_server * test = LOGIC_CONFIG->GetServerMysqlConfig().GetServerConfig(); //��ȡIP�Ͷ˿�
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
