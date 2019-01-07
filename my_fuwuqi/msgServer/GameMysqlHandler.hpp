
#ifndef _GAMEMYSQLHANDLER_HPP_
#define _GAMEMYSQLHANDLER_HPP_

#include "Handler.hpp"
#include "../proto/CSmsg.pb.h"
#include "quanju.hpp"

class GameMysqlHandler : public IHandler
{
public:
	GameMysqlHandler();
	virtual ~GameMysqlHandler();

	virtual int OnServerMsg(const CSMsg& rCSMsg, int iFd);//iFd为io套接字

	virtual int OnClientMsg(const CSMsg& rCSMsg, int iFd);

	static int OnSendRoleUpLine(RoleInfo * pRoleInfo,uint64_t rUid);		//向好友发送上线广播

	static int OnSendRoleQuitLine(RoleInfo * pRoleInfo,uint64_t rUid);		//向好友发送离线广播

	static int OnSendChangeStatus(RoleInfo * pRoleInfo,uint64_t rUid);		//向好友发送状态改变广播


private:
	//返回一个结构内存地址出来。eMsgId为第一层协议号，CmdType为第2层协议号，pMsgObj传入一个结构的空地址，函数会给该空地址附上值
	//该函数每个模块的handler都会有这个函数，每个模块有自己该函数的封装

	 static void* OnCSMsg(CSMsg& rMsg, uint64_t Uid, CSMsgID eMsgId, int CmdType); //服务器向服务器发送

	//检查客户端的数据包是否存在，正确就返回0，不正确就返回负数
	int OnCheckCSMsg(const CSMsg& rMsg, CSMsgID eMsgId);

	// 注: OnSSMsg 和 OnCheckSSMsg 和 OnServerMsg 这三个函数每个模块的Handler都有自个的封装。必须要有的


	//下面是所有模块功能的入口

	int OnMsgRegisterReq(const CSMsg& rCSMsg, int iFd);							
	int OnMsgLoginReq(const CSMsg& rCSMsg, int iFd);								
	int OnMsgQuitReq(const CSMsg& rCSMsg, int iFd);									
	int OnAskAddFriendReq(const CSMsg& rCSMsg, int iFd);						
	int OnSuccessAddFriendReq(const CSMsg& rCSMsg, int iFd);
	int OnDeleteFriendReq(const CSMsg& rCSMsg, int iFd);
	int OnSendChatReq(const CSMsg& rCSMsg, int iFd);
	int OnFindNameReq(const CSMsg& rCSMsg, int iFd);
	int OnChangeStatusReq(const CSMsg& rCSMsg, int iFd);
	int OnConnectSuccessReq(const CSMsg& rCSMsg, int iFd);

};


#endif