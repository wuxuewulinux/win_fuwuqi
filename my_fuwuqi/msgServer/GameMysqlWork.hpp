
#ifndef _GAMEMYSQLWORK_HPP_
#define _GAMEMYSQLWORK_HPP_

#include "../proto/CSmsg.pb.h"
#include "MysqlKu.hpp"

enum eRegister
{
	Register_Account_Fail = 1,
	Register_Name_Fail,
	Register_Success
};

enum eLogin
{
	Login_Account_Fail = 1,
	Login_Password_Fail,
	Login_Success,
	Login_Repeat
};

class GameMysqlWork
{
public:
	GameMysqlWork();
	~GameMysqlWork();

	//所有对数据库的操作逻辑都会在这里实现

	static int MsgRegister(const CSMsgRegisterSuccessReq& rReq);

	static int MsgLogin(const CSMsgLoginSuccessReq& rReq,CSMsgLoginSuccessRsp * pRsp);

	static int MsgQuit(const CSMsgQuitReq& rReq);

	static int MsgAskAddFriendReq(const CSMsgAskAddFriendReq& rReq,CSMsgAskAddFriendRsp* pRsp,uint64_t uiUid);

	static int MsgSuccessAddOneFriendReq(const CSMsgSuccessAddFriendReq& rReq,CSMsgSuccessAddFriendRsp* pRsp);

	static int MsgSuccessAddTwoFriendReq(const CSMsgSuccessAddFriendReq& rReq,CSMsgSuccessAddFriendRsp* pRsp);

	static int MsgDeleteOneFriendReq(const CSMsgDeleteFriendReq& rReq,CSMsgDeleteFriendRsp* pRsp);

	static int MsgDeleteTwoFriendReq(const CSMsgDeleteFriendReq& rReq,CSMsgDeleteFriendRsp* pRsp);

	static int MsgSendChatReq(const CSMsgSendChatReq& rReq,CSMsgSendChatRsp* pRsp);

	static int MsgFindNameReq(const CSMsgFindNameReq& rReq,CSMsgFindNameRsp* pRsp);

	static int MsgChangeStatusReq(const CSMsgChangeStatusReq& rReq);

private:


};


#endif