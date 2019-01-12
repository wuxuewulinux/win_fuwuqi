

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
//初始化游戏模块 日志
if(!LogInit("/home/wuxuewu/FightServer/"))
{
return;
}
//初始化记录错误信息的日志
if(!MYLOG.Init("/home/wuxuewu/FightServer/log/error.txt"))
{
std::cout<<"error log create fail !"<<std::endl;
return;
}
//开始读取游戏配置
if(!LOGIC_CONFIG->Init("/home/wuxuewu/FightServer/config_xml/"))
	return;
//连接游戏服务器
const struct GameServer * mGameServer = LOGIC_CONFIG->GetFightServerConfig().GetGameServerConfig(); //获取mysql服务器IP和端口
if(GAMECLIENT->CreateSocket(mGameServer->ip.c_str(),mGameServer->port) < 0)
	return;
if(GAMECLIENT->ConnectGameServer() < 0)
	return;
//初始化各个模块的handler
HANDLERLIST->Init();
//启动所有线程（读 写 处理工作 定时器 这四个线程）
if(pthreadstart() < 0)
 return;

//获取战斗服务器IP和端口
const struct FigthServer * test = LOGIC_CONFIG->GetFightServerConfig().GetFightServerConfig(); 
// TCP/IP启动
Socket_Ku socket_lei(test->ip.c_str(),test->port);
if(socket_lei.socket_creat() < 0)
	return;
if(socket_lei.socket_setsockopt() < 0)
	return;
if(socket_lei.socket_bind() < 0)
	return;
if(socket_lei.socket_listen() < 0)
	return;
litenfd=socket_lei.socket_listcnfd();   //获取服务器监听IO
//epoll模型启动
if(EPOLLKU->Epoll_Creat() < 0)
	return;
EPOLLKU->Epoll_Add(litenfd);

while(1)
{
 EPOLLKU->Epoll_Wait(&socket_lei);   //epoll检测IO事件

}

return;

}
