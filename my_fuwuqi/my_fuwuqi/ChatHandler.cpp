
#include "ChatHandler.hpp"

ChatHandler::ChatHandler()
{


}

ChatHandler::~ChatHandler()
{


}



 int ChatHandler::OnClientMsg(const CSMsg& rCSMsg, int iFd)
 {


	 return 0;
 }


 int ChatHandler::OnServerMsg(const SSMsg& rSSMsg)
 {



	 return 0;
 }



void* ChatHandler::OnCSMsg(CSMsg& rMsg, uint64_t Uid, CSMsgID eMsgId, int CmdType)
{
	CSMsgHead * pHead = rMsg.mutable_head();
	if (!pHead)
	{
		return NULL;
	}
	pHead->set_uid(Uid);
	pHead->set_msgid(eMsgId);
	CSMsgBody * pbody = rMsg.mutable_body();
	if (!pbody)
	{
		return NULL;
	}
	CSChatReq * pReq = pbody->mutable_chatreq();
	if (!pReq)
	{
		return NULL;
	}
	CSChatReqParam * pReqParam = pReq->mutable_reqparam();  //注：每个模块只要判断到该层次结构就算结束判断了，就开始进行对应指针判断了
	if (!pReqParam)
	{
		return NULL;
	}
	//从这里开始增加结构指针就可以了。				
	//进行判断获取对应的内存指针出去
	if (CmdType == CSMsgServer_RegisterSuccess)
	{
		CSMsgRegisterSuccessReq * pRegisterRsp = pReqParam->mutable_registersuccessreq();
		return (void*)pRegisterRsp;
	}
	else if (CmdType == CSMsgServer_LoginSuccess)
	{
		CSMsgLoginSuccessReq * pLoginRsp = pReqParam->mutable_loginsuccessreq();
		return (void*)pLoginRsp;
	}
	else if (CmdType == CSMsgServer_Quit)
	{
		CSMsgQuitReq * pQuitRsp = pReqParam->mutable_msgquitreq();
		return (void*)pQuitRsp;
	}
	else if (CmdType == CSMsgServer_ChangeStatus)
	{
		CSMsgChangeStatusReq * pChangeStatusRsp = pReqParam->mutable_changestatusreq();
		return (void*)pChangeStatusRsp;
	}
	////////

	return NULL;
}