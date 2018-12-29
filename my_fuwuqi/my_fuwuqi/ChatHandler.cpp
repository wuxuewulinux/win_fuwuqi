
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
	CSChatReqParam * pReqParam = pReq->mutable_reqparam();  //ע��ÿ��ģ��ֻҪ�жϵ��ò�νṹ��������ж��ˣ��Ϳ�ʼ���ж�Ӧָ���ж���
	if (!pReqParam)
	{
		return NULL;
	}
	//�����￪ʼ���ӽṹָ��Ϳ����ˡ�				
	//�����жϻ�ȡ��Ӧ���ڴ�ָ���ȥ
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