

#include"logicconfigmanager.hpp"
#include "lognamespace.hpp"
#include "enterfunction.hpp"
#include "socketku.hpp"
#include "pthreadstart.hpp"
#include "epoll_ku.hpp"
#include "HandlerList.hpp"
#include "SocketGameClient.hpp"

void gateway()
{
int litenfd;
//��ʼ����Ϸģ�� ��־
if(!LogInit("/home/wuxuewu/FightServer/"))
{
return;
}
//��ʼ����¼������Ϣ����־
if(!MYLOG.Init("/home/wuxuewu/FightServer/log/error.txt"))
{
std::cout<<"error log create fail !"<<std::endl;
return;
}
//��ʼ��ȡ��Ϸ����
if(!LOGIC_CONFIG->Init("/home/wuxuewu/FightServer/config_xml/"))
	return;
//������Ϸ������
const struct GameServer * mGameServer = LOGIC_CONFIG->GetFightServerConfig().GetGameServerConfig(); //��ȡmysql������IP�Ͷ˿�
if(GAMECLIENT->CreateSocket(mGameServer->ip.c_str(),mGameServer->port) < 0)
	return;
if(GAMECLIENT->ConnectGameServer() < 0)
	return;
//��ʼ������ģ���handler
HANDLERLIST->Init();
//���������̣߳��� д ������ ��ʱ�� ���ĸ��̣߳�
if(pthreadstart() < 0)
 return;

//��ȡս��������IP�Ͷ˿�
const struct FigthServer * test = LOGIC_CONFIG->GetFightServerConfig().GetFightServerConfig(); 
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
