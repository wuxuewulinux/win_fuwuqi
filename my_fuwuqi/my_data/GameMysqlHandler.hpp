
#ifndef _GAMEMYSQLHANDLER_HPP_
#define _GAMEMYSQLHANDLER_HPP_

#include "Handler.hpp"
#include "../proto/DBmsg.pb.h"
#include "../proto/SSmsg.pb.h"


class GameMysqlHandler : public IHandler
{
public:
	GameMysqlHandler();
	virtual ~GameMysqlHandler();

	virtual int OnServerMsg(const SSMsg& rSSMsg, int iFd);//iFd为io套接字

private:
	//返回一个结构内存地址出来。eMsgId为第一层协议号，CmdType为第2层协议号，pMsgObj传入一个结构的空地址，函数会给该空地址附上值
	//该函数每个模块的handler都会有这个函数，每个模块有自己该函数的封装

	void* OnSSMsg(SSMsg& rMsg, uint64_t Uid, SSMsgID eMsgId, int CmdType); //服务器向服务器发送

	//检查客户端的数据包是否存在，正确就返回0，不正确就返回负数
	int OnCheckSSMsg(const SSMsg& rMsg, SSMsgID eMsgId);

	// 注: OnSSMsg 和 OnCheckSSMsg 和 OnServerMsg 这三个函数每个模块的Handler都有自个的封装。必须要有的


	//下面是所有模块功能的入口

	int OnRegisterReq(const SSMsg& rSSMsg, int iFd);							//注册功能
	int OnLoginReq(const SSMsg& rSSMsg, int iFd);								//登陆功能
	int OnQuitReq(const SSMsg& rSSMsg, int iFd);								//退出功能	
	int OnUpdateDatabaseReq(const SSMsg& rSSMsg, int iFd);						//更新该role在数据库的所有数据
};


#endif