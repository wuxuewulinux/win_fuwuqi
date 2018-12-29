
#include "GameMysqlWork.hpp"
#include "quanju.hpp"
#include "enterfunction.hpp"

GameMysqlWork::GameMysqlWork()
{

}
GameMysqlWork::~GameMysqlWork()
{

}



int GameMysqlWork::MsgRegister(const CSMsgRegisterSuccessReq& rReq)
{
	MYSQLKU->MsgInsertAccount(rReq.uid(),rReq.name());

	return 0;
}

int GameMysqlWork::MsgLogin(const CSMsgLoginSuccessReq& rReq,CSMsgLoginSuccessRsp * pRsp)
{
	//1表示在线状态
	MYSQLKU->MsgLoginSuccess(rReq.uid(),1,pRsp);

	return 0;
}


int GameMysqlWork::MsgQuit(const CSMsgQuitReq& rReq)
{
	//0表示离线状态
	MYSQLKU->MsgQuitLine(rReq.uid(),0);
	DeleteRoleIo(rReq.uid());
	return 0;
}


int GameMysqlWork::MsgAskAddFriendReq(const CSMsgAskAddFriendReq& rReq,CSMsgAskAddFriendRsp* pRsp,uint64_t uiUid)
{
	pRsp->set_uid(uiUid);
	pRsp->set_friendname(rReq.name());
	return 0;
}


int GameMysqlWork::MsgSuccessAddOneFriendReq(const CSMsgSuccessAddFriendReq& rReq,CSMsgSuccessAddFriendRsp* pRsp)
{
	MYSQLKU->MsgSuccessAddFriend(rReq.uid1(),rReq.uid2(),pRsp);

	return 0;
}


int GameMysqlWork::MsgSuccessAddTwoFriendReq(const CSMsgSuccessAddFriendReq& rReq,CSMsgSuccessAddFriendRsp* pRsp)
{
	MYSQLKU->MsgSuccessAddFriend(rReq.uid2(),rReq.uid1(),pRsp);
	return 0;
}


int GameMysqlWork::MsgDeleteOneFriendReq(const CSMsgDeleteFriendReq& rReq,CSMsgDeleteFriendRsp* pRsp)
{
	MYSQLKU->MsgDeleteFriend(rReq.uid1(),rReq.uid2());
	pRsp->set_uid(rReq.uid2());
	return 0;
}



int GameMysqlWork::MsgDeleteTwoFriendReq(const CSMsgDeleteFriendReq& rReq,CSMsgDeleteFriendRsp* pRsp)
{
	MYSQLKU->MsgDeleteFriend(rReq.uid2(),rReq.uid1());
	pRsp->set_uid(rReq.uid1());
	return 0;
}



int GameMysqlWork::MsgSendChatReq(const CSMsgSendChatReq& rReq,CSMsgSendChatRsp* pRsp)
{
	pRsp->set_uid(rReq.uid1());
	pRsp->set_information(rReq.information());
	return 0;
}


int GameMysqlWork::MsgFindNameReq(const CSMsgFindNameReq& rReq,CSMsgFindNameRsp* pRsp)
{
	MYSQLKU->MsgFindName(rReq.name(),pRsp);

	return 0;
}


int GameMysqlWork::MsgChangeStatusReq(const CSMsgChangeStatusReq& rReq)
{
	MYSQLKU->MsgChangeStatus(rReq.uid(),rReq.value(),rReq.type());

	return 0;
}