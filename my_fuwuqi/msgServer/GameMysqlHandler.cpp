
#include "GameMysqlHandler.hpp"
#include "GameMysqlWork.hpp"
#include "sendclient.hpp"
#include "quanju.hpp"
#include "enterfunction.hpp"

GameMysqlHandler::GameMysqlHandler()
{

}

GameMysqlHandler::~GameMysqlHandler()
{

}


int GameMysqlHandler::OnClientMsg(const CSMsg& rCSMsg, int iFd)
{


	return 0;
}



int GameMysqlHandler::OnServerMsg(const CSMsg& rCSMsg, int iFd)
{
	int iRet = -1;
	iRet = OnCheckCSMsg(rCSMsg, CS_MSGID_Chat);
	if (iRet < 0)
	{
		//��־���
		printf("GameMysqlHandler OnCheckSSMsg : %d",iRet);
		MYLOG.sprintf(BUFF,"GameMysqlHandler CheckCSMsg : %d",iRet);
		return -1;
	}
	/*
	����Ĵ���ÿ��ģ���Handler���OnClientMsg������һ��
	*/
	const CSChatReq &req = rCSMsg.body().chatreq();
	switch (req.cmd())
	{
	case CSMsgServer_RegisterSuccess:					//����������û�ע��ɹ�����
		{
			iRet = OnMsgRegisterReq(rCSMsg, iFd);
		}
		break;
	case CSMsgServer_LoginSuccess:						//����������û���½����
		{
			iRet = OnMsgLoginReq(rCSMsg, iFd);
		}
		break;
	case CSMsgServer_Quit:								//����������˳�(����)��Ϸ����
		{
			iRet = OnMsgQuitReq(rCSMsg, iFd);
		}
		break;
	case CSMsgServer_AskAddFriend:						//������������������Ӻ���
		{
			iRet = OnAskAddFriendReq(rCSMsg, iFd);
		}
		break;
	case CSMsgServer_SuccessAddFriend:					//�����������ҳɹ���Ӻ���
		{
			iRet = OnSuccessAddFriendReq(rCSMsg, iFd);
		}
		break;
	case CSMsgServer_DeleteFriend:						//������������ɾ������
		{
			iRet = OnDeleteFriendReq(rCSMsg, iFd);
		}
		break;
	case CSMsgServer_SendChat:							//��������������������
		{
			iRet = OnSendChatReq(rCSMsg, iFd);
		}
		break;
	case CSMsgServer_FindName:							//�������������������Ʋ����û�
		{
			iRet = OnFindNameReq(rCSMsg, iFd);
		}
		break;
	case CSMsgServer_ChangeStatus:						//����������������ı�ĳ��״̬
		{
			iRet = OnChangeStatusReq(rCSMsg, iFd);
		}
		break;
	case CSMsgServer_ConnectSuccess:					//�ͻ�����������������ɹ�����UIDѹ��ͨ��������
		{
			iRet = OnConnectSuccessReq(rCSMsg, iFd);
		}
		break;
	default:
		iRet = -1;
	}
	if (iRet < 0)
	{
		//�жϹ����߼��Ƿ���ȷ�������ܻ���ָ��������ָ�����ԭ�������ָ��Ϊ�գ��������ݷ�������
		printf("GameMysqlHandler error : %d",iRet);
		MYLOG.sprintf(BUFF,"GameMysqlHandler error : %d",iRet);
	}
	
	return 0;
}


void* GameMysqlHandler::OnCSMsg(CSMsg& rMsg, uint64_t Uid, CSMsgID eMsgId, int CmdType)
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
	CSChatRsp * pReq = pbody->mutable_chatrsp();
	if (!pReq)
	{
		return NULL;
	}
	CSChatRspParam * pReqParam = pReq->mutable_rspparam();  //ע��ÿ��ģ��ֻҪ�жϵ��ò�νṹ��������ж��ˣ��Ϳ�ʼ���ж�Ӧָ���ж���
	if (!pReqParam)
	{
		return NULL;
	}
	
	//�����жϻ�ȡ��Ӧ���ڴ�ָ���ȥ
	if (CmdType == CSMsgServer_FriendLine)
	{
		CSMsgRoleInfo * pRoleInfoRsp = pReqParam->mutable_roleinfo();
		return (void*)pRoleInfoRsp;
	}
	else if (CmdType == CSMsgServer_LoginSuccess)
	{
		CSMsgLoginSuccessRsp* pFriendRoleInfo = pReqParam->mutable_msgloginsuccess();
		return (void*)pFriendRoleInfo;
	}
	else if (CmdType == CSMsgServer_Quit)
	{
		CSMsgRoleInfo * pRoleQuit = pReqParam->mutable_rolequit();
		return (void*)pRoleQuit;
	}
	else if (CmdType == CSMsgServer_AskAddFriend)
	{
		CSMsgAskAddFriendRsp * pAskAddFriend = pReqParam->mutable_askaddfriendrsp();
		return (void*)pAskAddFriend;
	}
	else if (CmdType == CSMsgServer_SuccessAddFriend)
	{
		CSMsgSuccessAddFriendRsp * pSuccessAddFriend = pReqParam->mutable_successaddfriendrsp();
		return (void*)pSuccessAddFriend;
	}
	else if (CmdType == CSMsgServer_DeleteFriend)
	{
		CSMsgDeleteFriendRsp * pDeleteFriend = pReqParam->mutable_deletefriendrsp();
		return (void*)pDeleteFriend;
	}
	else if (CmdType == CSMsgServer_SendChat)
	{
		CSMsgSendChatRsp * pSendChat = pReqParam->mutable_sendchatrsp();
		return (void*)pSendChat;
	}
	else if (CmdType == CSMsgServer_FindName)
	{
		CSMsgFindNameRsp * pFindName = pReqParam->mutable_findnamersp();
		return (void*)pFindName;
	}
	else if (CmdType == CSMsgServer_ChangeStatus)
	{
		CSMsgChangeStatusRsp * pChangeStatus = pReqParam->mutable_changestatusrsp();
		return (void*)pChangeStatus;
	}
	////////
	return NULL;

}


int GameMysqlHandler::OnCheckCSMsg(const CSMsg& rMsg, CSMsgID eMsgId)
{
	if ( !rMsg.has_head())
	{
		return -1;
	}
	const CSMsgHead& rHead= rMsg.head();
	if ( ! (rHead.has_uid()) || ! rHead.has_msgid())
	{
		return -2;
	}
	if ( (!rMsg.has_body()) )
	{
		return -3;
	}
	if (rHead.has_msgid() != eMsgId)
	{
		return -4;
	}
	const CSMsgBody& rBody = rMsg.body();
	const CSChatReq& rTmp = rBody.chatreq();
	if ( !(rTmp.has_cmd()) || !(rTmp.has_reqparam()) )
	{
		return -5;
	}

	return 0;
	//���ϵĴ�����һ�����е��������Ͳ�һ���͸�һ�¾Ϳ����ˡ�

}



int GameMysqlHandler::OnMsgRegisterReq(const CSMsg& rCSMsg, int iFd)
{
	const CSMsgRegisterSuccessReq & rRegisterReq = rCSMsg.body().chatreq().reqparam().registersuccessreq();

	//��ʼִ�й����߼�
	int iRet = GameMysqlWork::MsgRegister(rRegisterReq);
	//�����С��0֤�����ݴ��󣬲����Է���,������־��
	if (iRet < 0)
	{
		printf("GameMysqlWork MsgRegister error : %d",iRet);
		MYLOG.sprintf(BUFF,"GameMysqlWork MsgRegister error : %d",iRet);
		return -3;
	}

	return 0;

}



int GameMysqlHandler::OnMsgLoginReq(const CSMsg& rCSMsg, int iFd)
{
	const CSMsgLoginSuccessReq & rLoginReq = rCSMsg.body().chatreq().reqparam().loginsuccessreq();
	CSMsg oCSMsg;
	CSMsgLoginSuccessRsp* pFriendInfoListRsp = static_cast<CSMsgLoginSuccessRsp*>(OnCSMsg(oCSMsg, rLoginReq.uid(), CS_MSGID_Chat, CSMsgServer_LoginSuccess)); 
	HANDCHECH_P(pFriendInfoListRsp,-1);

	int iRet = GameMysqlWork::MsgLogin(rLoginReq,pFriendInfoListRsp);

	if (iRet < 0)
	{
		printf("GameMysqlWork  MsgLogin error : %d",iRet);
		MYLOG.sprintf(BUFF,"GameMysqlWork  MsgLogin error : %d",iRet);
		return -3;
	}
	//����Э����ͻ���
	int ConnIo = GetRoleIo(rLoginReq.uid());
	if (ConnIo != 0)
	{
		SendClient(ConnIo,&oCSMsg);
	}

	return 0;

}


int GameMysqlHandler::OnMsgQuitReq(const CSMsg& rCSMsg, int iFd)
{
	const CSMsgQuitReq & rQuitReq = rCSMsg.body().chatreq().reqparam().msgquitreq();

	int iRet = GameMysqlWork::MsgQuit(rQuitReq);

	if (iRet < 0)
	{
		printf("GameMysqlWork  MsgQuit error : %d",iRet);
		MYLOG.sprintf(BUFF,"GameMysqlWork  MsgQuit error : %d",iRet);
		return -3;
	}
	
	return 0;
}


int GameMysqlHandler::OnAskAddFriendReq(const CSMsg& rCSMsg, int iFd)
{
	const CSMsgAskAddFriendReq & Req = rCSMsg.body().chatreq().reqparam().askaddfriendreq();

	CSMsg oCSMsg;
	CSMsgAskAddFriendRsp* Rsp = static_cast<CSMsgAskAddFriendRsp*>(OnCSMsg(oCSMsg, Req.uid(), CS_MSGID_Chat, CSMsgServer_AskAddFriend));
	HANDCHECH_P(Rsp,-1);

	int iRet = GameMysqlWork::MsgAskAddFriendReq(Req,Rsp,rCSMsg.head().uid());

	if (iRet < 0)
	{
		printf("GameMysqlWork  MsgAskAddFriendReq error : %d",iRet);
		MYLOG.sprintf(BUFF,"GameMysqlWork  MsgAskAddFriendReq error : %d",iRet);
		return -3;
	}
	int SockIo = GetRoleIo(Req.uid());
	if (SockIo != 0)
	{
		SendClient(SockIo,&oCSMsg);
	}
	return 0;
}


int GameMysqlHandler::OnSendRoleUpLine(RoleInfo * pRoleInfo,uint64_t rUid)
{
	HANDCHECH_P(pRoleInfo,-1);
	CSMsg oCSMsg;
	CSMsgRoleInfo* pRoleInfoRsp = static_cast<CSMsgRoleInfo*>(OnCSMsg(oCSMsg, rUid, CS_MSGID_Chat, CSMsgServer_FriendLine)); 
	HANDCHECH_P(pRoleInfoRsp,-1);
	pRoleInfoRsp->set_name(pRoleInfo->Name);
	pRoleInfoRsp->set_uid(pRoleInfo->Uid);
	pRoleInfoRsp->set_grade(pRoleInfo->Grade);
	pRoleInfoRsp->set_status(pRoleInfo->Rank);
	pRoleInfoRsp->set_vip(pRoleInfo->Vip);
	pRoleInfoRsp->set_head(pRoleInfo->Head);
	pRoleInfoRsp->set_chatframe(pRoleInfo->ChatFrame);
	int ConnIo = GetRoleIo(rUid);
	if (ConnIo != 0)
	{
		SendClient(ConnIo,&oCSMsg);
	}

	return 0;
}


int GameMysqlHandler::OnSendRoleQuitLine(RoleInfo * pRoleInfo,uint64_t rUid)
{
	HANDCHECH_P(pRoleInfo,-1);
	CSMsg oCSMsg;
	CSMsgRoleInfo* pRoleInfoRsp = static_cast<CSMsgRoleInfo*>(OnCSMsg(oCSMsg, rUid, CS_MSGID_Chat, CSMsgServer_Quit)); 
	HANDCHECH_P(pRoleInfoRsp,-1);
	pRoleInfoRsp->set_uid(pRoleInfo->Uid);
	pRoleInfoRsp->set_name(pRoleInfo->Name);
	pRoleInfoRsp->set_grade(pRoleInfo->Grade);
	pRoleInfoRsp->set_status(pRoleInfo->Rank);
	pRoleInfoRsp->set_vip(pRoleInfo->Vip);
	pRoleInfoRsp->set_head(pRoleInfo->Head);
	pRoleInfoRsp->set_chatframe(pRoleInfo->ChatFrame);
	int ConnIo = GetRoleIo(rUid);
	if (ConnIo != 0)
	{
		SendClient(ConnIo,&oCSMsg);
	}

	return 0;
}



int GameMysqlHandler::OnSendChangeStatus(RoleInfo * pRoleInfo,uint64_t rUid)
{
	HANDCHECH_P(pRoleInfo,-1);
	CSMsg oCSMsg;
	CSMsgChangeStatusRsp* pChangeStatusRsp = static_cast<CSMsgChangeStatusRsp*>(OnCSMsg(oCSMsg, rUid, CS_MSGID_Chat, CSMsgServer_ChangeStatus)); 
	HANDCHECH_P(pChangeStatusRsp,-1);
	CSMsgRoleInfo* pRoleInfoRsp = pChangeStatusRsp->mutable_roleinfo();
	HANDCHECH_P(pRoleInfoRsp,-2);
	pRoleInfoRsp->set_uid(pRoleInfo->Uid);
	pRoleInfoRsp->set_name(pRoleInfo->Name);
	pRoleInfoRsp->set_grade(pRoleInfo->Grade);
	pRoleInfoRsp->set_status(pRoleInfo->Rank);
	pRoleInfoRsp->set_vip(pRoleInfo->Vip);
	pRoleInfoRsp->set_head(pRoleInfo->Head);
	pRoleInfoRsp->set_chatframe(pRoleInfo->ChatFrame);
	int ConnIo = GetRoleIo(rUid);
	if (ConnIo != 0)
	{
		SendClient(ConnIo,&oCSMsg);
	}
	return 0;
}



int GameMysqlHandler::OnSuccessAddFriendReq(const CSMsg& rCSMsg, int iFd)
{
	const CSMsgSuccessAddFriendReq & Req = rCSMsg.body().chatreq().reqparam().successaddfriendreq();

	//������һ����Һ���
	{
		CSMsg oCSMsg;
		CSMsgSuccessAddFriendRsp* Rsp = static_cast<CSMsgSuccessAddFriendRsp*>(OnCSMsg(oCSMsg, Req.uid2(), CS_MSGID_Chat, CSMsgServer_SuccessAddFriend));
		HANDCHECH_P(Rsp,-1);
		int iRet = GameMysqlWork::MsgSuccessAddOneFriendReq(Req,Rsp);

		if (iRet < 0)
		{
			printf("GameMysqlWork  MsgSuccessAddOneFriendReq error : %d",iRet);
			MYLOG.sprintf(BUFF,"GameMysqlWork  MsgSuccessAddOneFriendReq error : %d",iRet);
			return -3;
		}
		int SockIo = GetRoleIo(Req.uid2());
		if (SockIo != 0)
		{
			SendClient(SockIo,&oCSMsg);
		}
	}
	//�����ڶ�����Һ���
	{
		CSMsg oCSMsg;
		CSMsgSuccessAddFriendRsp* Rsp = static_cast<CSMsgSuccessAddFriendRsp*>(OnCSMsg(oCSMsg, Req.uid1(), CS_MSGID_Chat, CSMsgServer_SuccessAddFriend));
		HANDCHECH_P(Rsp,-1);
		int iRet = GameMysqlWork::MsgSuccessAddTwoFriendReq(Req,Rsp);

		if (iRet < 0)
		{
			printf("GameMysqlWork  MsgSuccessAddTwoFriendReq error : %d",iRet);
			MYLOG.sprintf(BUFF,"GameMysqlWork  MsgSuccessAddTwoFriendReq error : %d",iRet);
			return -3;
		}
		int SockIo = GetRoleIo(Req.uid1());
		if (SockIo != 0)
		{
			SendClient(SockIo,&oCSMsg);
		}
	}

	return 0;
}



int GameMysqlHandler::OnDeleteFriendReq(const CSMsg& rCSMsg, int iFd)
{
	const CSMsgDeleteFriendReq & Req = rCSMsg.body().chatreq().reqparam().deletefriendreq();

	//ɾ����һ����Һ���
	{
		CSMsg oCSMsg;
		CSMsgDeleteFriendRsp* Rsp = static_cast<CSMsgDeleteFriendRsp*>(OnCSMsg(oCSMsg, Req.uid1(), CS_MSGID_Chat, CSMsgServer_DeleteFriend));
		HANDCHECH_P(Rsp,-1);
		int iRet = GameMysqlWork::MsgDeleteOneFriendReq(Req,Rsp);

		if (iRet < 0)
		{
			printf("GameMysqlWork  MsgDeleteOneFriendReq error : %d",iRet);
			MYLOG.sprintf(BUFF,"GameMysqlWork  MsgDeleteOneFriendReq error : %d",iRet);
			return -3;
		}
		int SockIo = GetRoleIo(Req.uid1());
		if (SockIo != 0)
		{
			SendClient(SockIo,&oCSMsg);
		}
	}
	//ɾ���ڶ�����Һ���
	{
		CSMsg oCSMsg;
		CSMsgDeleteFriendRsp* Rsp = static_cast<CSMsgDeleteFriendRsp*>(OnCSMsg(oCSMsg, Req.uid2(), CS_MSGID_Chat, CSMsgServer_DeleteFriend));
		HANDCHECH_P(Rsp,-1);
		int iRet = GameMysqlWork::MsgDeleteTwoFriendReq(Req,Rsp);

		if (iRet < 0)
		{
			printf("GameMysqlWork  MsgDeleteTwoFriendReq error : %d",iRet);
			MYLOG.sprintf(BUFF,"GameMysqlWork  MsgDeleteTwoFriendReq error : %d",iRet);
			return -3;
		}
		int SockIo = GetRoleIo(Req.uid2());
		if (SockIo != 0)
		{
			SendClient(SockIo,&oCSMsg);
		}
	}

	return 0;
}



int GameMysqlHandler::OnSendChatReq(const CSMsg& rCSMsg, int iFd)
{
	const CSMsgSendChatReq & Req = rCSMsg.body().chatreq().reqparam().sendchatreq();

	CSMsg oCSMsg;
	CSMsgSendChatRsp* Rsp = static_cast<CSMsgSendChatRsp*>(OnCSMsg(oCSMsg, Req.uid2(), CS_MSGID_Chat, CSMsgServer_SendChat));
	HANDCHECH_P(Rsp,-1);

	int iRet = GameMysqlWork::MsgSendChatReq(Req,Rsp);

	if (iRet < 0)
	{
		printf("GameMysqlWork  MsgSendChatReq error : %d",iRet);
		MYLOG.sprintf(BUFF,"GameMysqlWork  MsgSendChatReq error : %d",iRet);
		return -3;
	}
	int SockIo = GetRoleIo(Req.uid2());
	if (SockIo != 0)
	{
		SendClient(SockIo,&oCSMsg);
	}
	
	return 0;
}




int GameMysqlHandler::OnFindNameReq(const CSMsg& rCSMsg, int iFd)
{
	const CSMsgFindNameReq & Req = rCSMsg.body().chatreq().reqparam().findnamereq();

	CSMsg oCSMsg;
	CSMsgFindNameRsp* Rsp = static_cast<CSMsgFindNameRsp*>(OnCSMsg(oCSMsg, rCSMsg.head().uid(), CS_MSGID_Chat, CSMsgServer_FindName));
	HANDCHECH_P(Rsp,-1);

	int iRet = GameMysqlWork::MsgFindNameReq(Req,Rsp);

	if (iRet < 0)
	{
		printf("GameMysqlWork  MsgFindNameReq error : %d",iRet);
		MYLOG.sprintf(BUFF,"GameMysqlWork  MsgFindNameReq error : %d",iRet);
		return -3;
	}
	if (iRet == 0)
	{
		SendClient(iFd,&oCSMsg);
	}

	return 0;
}


int GameMysqlHandler::OnChangeStatusReq(const CSMsg& rCSMsg, int iFd)
{
	const CSMsgChangeStatusReq & Req = rCSMsg.body().chatreq().reqparam().changestatusreq();

	int iRet = GameMysqlWork::MsgChangeStatusReq(Req);

	if (iRet < 0)
	{
		printf("GameMysqlWork  MsgChangeStatusReq error : %d",iRet);
		MYLOG.sprintf(BUFF,"GameMysqlWork  MsgChangeStatusReq error : %d",iRet);
		return -3;
	}
	
	return iRet;
}



int GameMysqlHandler::OnConnectSuccessReq(const CSMsg& rCSMsg, int iFd)
{
	const CSMsgConnectSuccessReq & Req = rCSMsg.body().chatreq().reqparam().connectsuccessreq();
	PushRoleIo(Req.uid(),iFd);
	return 0;
}