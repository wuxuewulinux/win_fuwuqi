#include "RegisterLoginHandler.hpp"
#include "quanju.hpp"
#include "RegisterLoginWork.hpp"
#include "enterfunction.hpp"
#include "sendclient.hpp"
#include "ChatHandler.hpp"
#include "SocketMsgClient.hpp"

RegisterLoginHandler::RegisterLoginHandler()
{

}
RegisterLoginHandler::~RegisterLoginHandler()
{

}

int RegisterLoginHandler::OnCheckCSMsg(const CSMsg& rMsg, CSMsgID eMsgId)
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
	const CSRegisterLoginReq& rTmp = rBody.registerloginreq();
	if ( !(rTmp.has_cmd()) || !(rTmp.has_reqparam()))
	{
		return -5;
	}

	return 0;
	//���ϵĴ�����һ�����е��������Ͳ�һ���͸�һ�¾Ϳ����ˡ�
}



int RegisterLoginHandler::OnCheckSSMsg(const SSMsg& rMsg, SSMsgID eMsgId)
{
	if ( !rMsg.has_head())
	{
		return -1;
	}
	const SSMsgHead& rHead= rMsg.head();
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
	const SSMsgBody& rBody = rMsg.body();
	const SSRegisterLoginReq& rTmp = rBody.registerloginreq();
	if ( !(rTmp.has_cmd()) || !(rTmp.has_reqparam()) )
	{
		return -5;
	}
	return 0;
	//���ϵĴ�����һ�����е��������Ͳ�һ���͸�һ�¾Ϳ����ˡ�

}



void* RegisterLoginHandler::OnCSMsg(CSMsg& rMsg, uint64_t Uid, CSMsgID eMsgId, int CmdType)
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
	CSRegisterLoginRsp * pReq = pbody->mutable_registerloginrsp();
	if (!pReq)
	{
		return NULL;
	}
	CSRegisterLoginRspParam * pReqParam = pReq->mutable_rspparam();  //ע��ÿ��ģ��ֻҪ�жϵ��ò�νṹ��������ж��ˣ��Ϳ�ʼ���ж�Ӧָ���ж���
	if (!pReqParam)
	{
		return NULL;
	}

	//�����￪ʼ���ӽṹָ��Ϳ����ˡ�
	//�����жϻ�ȡ��Ӧ���ڴ�ָ���ȥ
	if (CmdType == CSRegisterLoginCmd_Register)
	{
		CSRegisterRsp * pRegisterRsp = pReqParam->mutable_registerrsp();
		return (void*)pRegisterRsp;
	}
	else if (CmdType == CSRegisterLoginCmd_Login)
	{
		CSLoginRsp * pLoginRsp = pReqParam->mutable_loginrsp();
		return (void*)pLoginRsp;
	}
	////////
	return NULL;
}


void* RegisterLoginHandler::OnSSMsg(SSMsg& rMsg, uint64_t Uid, SSMsgID eMsgId, int CmdType)
{
	SSMsgHead * pHead = rMsg.mutable_head();
	if (!pHead)
	{
		return NULL;
	}
	pHead->set_uid(Uid);
	pHead->set_msgid(eMsgId);
	SSMsgBody * pbody = rMsg.mutable_body();
	if (!pbody)
	{
		return NULL;
	}
	SSRegisterLoginReq * pReq = pbody->mutable_registerloginreq();
	if (!pReq)
	{
		return NULL;
	}
	SSRegisterLoginReqParam * pReqParam = pReq->mutable_reqparam();  //ע��ÿ��ģ��ֻҪ�жϵ��ò�νṹ��������ж��ˣ��Ϳ�ʼ���ж�Ӧָ���ж���
	if (!pReqParam)
	{
		return NULL;
	}
	//�����￪ʼ���ӽṹָ��Ϳ����ˡ�
	//�����жϻ�ȡ��Ӧ���ڴ�ָ���ȥ
	if (CmdType == SSRegisterLoginCmd_Register)
	{
		SSRegisterReq * pRegisterReq = pReqParam->mutable_registerreq();
		return (void*)pRegisterReq;
	}
	else if (CmdType == SSRegisterLoginCmd_Login)
	{
		SSLoginReq * pLoginReq = pReqParam->mutable_loginreq();
		return (void*)pLoginReq;
	}
	else if (CmdType == SSRegisterLoginCmd_Quit)
	{
		SSQuitReq * pQuitReq = pReqParam->mutable_quitreq();
		return (void*)pQuitReq;
	}
	
	////////
	return NULL;
}




int RegisterLoginHandler::OnServerMsg(const SSMsg& rSSMsg)
{

	int iRet = -1;
	iRet = OnCheckSSMsg(rSSMsg, SS_MSGID_GameMysql);
	if (iRet < 0)
	{
		//��־���
		printf("RegisterLoginHandler CheckSSMsg : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginHandler CheckSSMsg : %d",iRet);
		return -1;
	}
	/*
	����Ĵ���ÿ��ģ���Handler���OnClientMsg������һ��
	*/
	const SSRegisterLoginRsp &rRsp = rSSMsg.body().registerloginrsp();
	switch (rRsp.cmd())
	{
	case SSRegisterLoginCmd_Register:					//ע�Ṧ��
		{
			iRet = OnRegisterRsp(rSSMsg);
		}
		break;
	case SSRegisterLoginCmd_Login:						//��½����
		{
			iRet = OnLoginRsp(rSSMsg);
		}
		break;
	default:
		iRet = -1;
	}
	if (iRet < 0)
	{
		//�жϹ����߼��Ƿ���ȷ�������ܻ���ָ��������ָ�����ԭ�������ָ��Ϊ�գ��������ݷ�������
		printf("RegisterLoginHandler OnServerMsg error : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginHandler OnServerMsg error : %d",iRet);
	}

	return 0;
}




int RegisterLoginHandler::OnClientMsg(const CSMsg& rCSMsg, int iFd)
{

	int iRet = -1;
	iRet = OnCheckCSMsg(rCSMsg, CS_MSGID_RegisterLogin);
	if (iRet < 0)
	{
		//��־���
		printf("RegisterLoginHandler CheckCSMsg : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginHandler CheckCSMsg : %d",iRet);
		return -1;
	}
	/*
	����Ĵ���ÿ��ģ���Handler���OnClientMsg������һ��
	*/
	const CSRegisterLoginReq &req = rCSMsg.body().registerloginreq();
	switch (req.cmd())
	{
	case CSRegisterLoginCmd_Register:					//ע�Ṧ��
		{
			iRet = OnRegisterReq(rCSMsg, iFd);
		}
		break;
	case CSRegisterLoginCmd_Login:						//��½����
		{
			iRet = OnLoginReq(rCSMsg, iFd);
		}
		break;
	case CSRegisterLoginCmd_Quit:						//��Ϸ�˳�����
		{
			iRet = OnQuitReq(rCSMsg, iFd);
		}
		break;
	default:
		iRet = -1;
	}
	if (iRet < 0)
	{
		//�жϹ����߼��Ƿ���ȷ�������ܻ���ָ��������ָ�����ԭ�������ָ��Ϊ�գ��������ݷ�������
		printf("RegisterLoginHandler OnClientMsg error : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginHandler OnClientMsg error : %d",iRet);
	}
	/*
	�������ݷ����ı�ʱ�ͱ���������ݿ⣬�����ε���������ݿ���Ҫ���¾���������ģʽȥ��д
	if ( req.cmd() == CSCardCmd_SaveDeck ||  req.cmd() == CSCardCmd_UpHeroLv ||
		req.cmd() == CSCardCmd_UpHeroStar ||  req.cmd() == CSCardCmd_UpMagicLv ||
		req.cmd() == CSCardCmd_SetUsingDeck || req.cmd() == CSCardCmd_CrystalDraw ||
		req.cmd() == CSCardCmd_UpRuneStep || req.cmd() == CSCardCmd_UpRuneAdvance || 
		req.cmd() == CSCardCmd_UpStep || req.cmd() == CSCardCmd_SaveTalentAttr)
	{
		if ( 0 == iRet )
		{
			CRoleObj * pRoleObj = CMemPoolMgr::GetRoleObjPool().GetByKey( rCSMsg.head().uid() );
			LOG_ASSERT_RET_INT( pRoleObj, -1 );
			CSnapHandler::UpdateSnap(pRoleObj);
		}

	}
	*/
	return 0;
}



int RegisterLoginHandler::OnRegisterReq(const CSMsg& rCSMsg, int iFd)
{
	const CSRegisterReq & rRegisterReq = rCSMsg.body().registerloginreq().reqparam().registerreq();
	//���IO������
	DBCLIENT->PushIo(iFd);
	SSMsg oSSMsg;
	//��ȡҪ���ͻ��˵������ڴ��ַ
	SSRegisterReq* pRegisterReq = static_cast<SSRegisterReq*>(OnSSMsg(oSSMsg, DBCLIENT->GetMapSize(), SS_MSGID_GameMysql, SSRegisterLoginCmd_Register)); 
	HANDCHECH_P(pRegisterReq,-1);
	/*
	��ȡ�ÿͻ��˵�Role�� ע�ỹû��role��ĳ���
	CRoleObj* pRoleObj = GetRole(rCSMsg.head().uid());
	HANDCHECH_P(pRoleObj,-2);
	*/
	//��ʼִ�й����߼�
	int iRet = RegisterLoginWork::RegisterReq(rRegisterReq,pRegisterReq);
	//�����С��0֤�����ݴ��󣬲����Է���,������־��
	if (iRet < 0)
	{
		printf("RegisterLoginWork Register error : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginWork Register error : %d",iRet);
		return -3;
	}
	//��ʼ�������ݸ��ͻ���
	SendServer(DBCLIENT->GetSocketIo(),&oSSMsg);
	return 0;
}


int RegisterLoginHandler::OnLoginReq(const CSMsg& rCSMsg, int iFd)
{
	const CSLoginReq & rLoginReq = rCSMsg.body().registerloginreq().reqparam().loginreq();
	//���IO������
	DBCLIENT->PushIo(iFd);

	SSMsg oSSMsg;
	SSLoginReq* pLoginReq = static_cast<SSLoginReq*>(OnSSMsg(oSSMsg,DBCLIENT->GetMapSize(), SS_MSGID_GameMysql, SSRegisterLoginCmd_Login)); 
	HANDCHECH_P(pLoginReq,-1);
	/*
	CRoleObj* pRoleObj = GetRole(rCSMsg.head().uid());
	HANDCHECH_P(pRoleObj,-2);
	*/
	int iRet = RegisterLoginWork::LoginReq(rLoginReq,pLoginReq);
	
	if (iRet < 0)
	{
		printf("RegisterLoginWork  Login error : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginWork Login error : %d",iRet);
		return -3;
	}
	SendServer(DBCLIENT->GetSocketIo(),&oSSMsg);
	return 0;
}


int RegisterLoginHandler::OnQuitReq(const CSMsg& rCSMsg, int iFd)
{
	//���IO������
	DBCLIENT->PushIo(iFd);

	SSMsg oSSMsg;
	SSQuitReq* pQuitReq = static_cast<SSQuitReq*>(OnSSMsg(oSSMsg, rCSMsg.head().uid(), SS_MSGID_GameMysql, SSRegisterLoginCmd_Quit)); 
	HANDCHECH_P(pQuitReq,-1);

	CRoleObj* pRoleObj = GetRole(rCSMsg.head().uid());
	HANDCHECH_P(pRoleObj,-2);
	
	int iRet = RegisterLoginWork::QuitReq(pRoleObj,pQuitReq,iFd,rCSMsg.head().uid());
	
	{
		//����������������û�����Э��
		CSMsg oCSMsg;
		CSMsgQuitReq* pMsgRsp = static_cast<CSMsgQuitReq*>(ChatHandler::OnCSMsg(oCSMsg, 0, CS_MSGID_Chat, CSMsgServer_Quit)); 
		HANDCHECH_P(pMsgRsp,-2);
		pMsgRsp->set_uid(rCSMsg.head().uid());
		SendClient(MSGCLIENT->GetSocketIo(),&oCSMsg);
	}

	if (iRet == 0)
	{
		SendServer(DBCLIENT->GetSocketIo(),&oSSMsg);
	}
	return 0;

}



int RegisterLoginHandler::OnRegisterRsp(const SSMsg& rSSMsg)
{
	const SSRegisterRsp & rRegisterRsp = rSSMsg.body().registerloginrsp().rspparam().registerrsp();

	CSMsg oCSMsg;
	//��ȡҪ���ͻ��˵������ڴ��ַ
	CSRegisterRsp* pRegisterRsp = static_cast<CSRegisterRsp*>(OnCSMsg(oCSMsg, rSSMsg.head().uid(), CS_MSGID_RegisterLogin, CSRegisterLoginCmd_Register)); 
	HANDCHECH_P(pRegisterRsp,-1);
	//ע��ɹ���Ҫ�����������������Ϣ
	if (rRegisterRsp.type() == 3)
	{
		CSMsg oCSMsg;
		CSMsgRegisterSuccessReq* pMsgRsp = static_cast<CSMsgRegisterSuccessReq*>(ChatHandler::OnCSMsg(oCSMsg, 0, CS_MSGID_Chat, CSMsgServer_RegisterSuccess)); 
		HANDCHECH_P(pMsgRsp,-2);
		pMsgRsp->set_uid(rRegisterRsp.uid());
		pMsgRsp->set_name(rRegisterRsp.name());
		SendClient(MSGCLIENT->GetSocketIo(),&oCSMsg);
	}
	int iRet = RegisterLoginWork::RegisterRsp(rRegisterRsp,pRegisterRsp);
	//�����С��0֤�����ݴ��󣬲����Է���,������־��
	if (iRet < 0)
	{
		printf("RegisterLoginWork Register error : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginWork Register error : %d",iRet);
		return -3;
	}
	//��ʼ�������ݸ��ͻ���
	SendClient(DBCLIENT->GetMapIo((int)rSSMsg.head().uid()),&oCSMsg);
	DBCLIENT->DeleteIO((int)rSSMsg.head().uid());
	return 0;
}

int RegisterLoginHandler::OnLoginRsp(const SSMsg& rSSMsg)
{
	const SSLoginRsp & rLoginRsp = rSSMsg.body().registerloginrsp().rspparam().loginrsp();

	CSMsg oCSMsg;
	CSLoginRsp* pLoginRsp = static_cast<CSLoginRsp*>(OnCSMsg(oCSMsg,rSSMsg.head().uid(), CS_MSGID_RegisterLogin, CSRegisterLoginCmd_Login)); 
	HANDCHECH_P(pLoginRsp,-1);
	//��������������͵�¼�ɹ�Э��
	if (rLoginRsp.type() == 3)
	{
		CSMsg oCSMsg;
		CSMsgLoginSuccessReq* pMsgRsp = static_cast<CSMsgLoginSuccessReq*>(ChatHandler::OnCSMsg(oCSMsg, 0, CS_MSGID_Chat, CSMsgServer_LoginSuccess)); 
		HANDCHECH_P(pMsgRsp,-2);
		pMsgRsp->set_uid(rLoginRsp.role().uid());
		SendClient(MSGCLIENT->GetSocketIo(),&oCSMsg);
	}

	int iRet = RegisterLoginWork::LoginRsp(rLoginRsp,pLoginRsp,DBCLIENT->GetMapIo((int)rSSMsg.head().uid()));
	
	if (iRet < 0)
	{
		printf("RegisterLoginWork  Login error : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginWork Login error : %d",iRet);
		return -3;
	}
	SendClient(DBCLIENT->GetMapIo((int)rSSMsg.head().uid()),&oCSMsg);
	DBCLIENT->DeleteIO((int)rSSMsg.head().uid());

	return 0;
}