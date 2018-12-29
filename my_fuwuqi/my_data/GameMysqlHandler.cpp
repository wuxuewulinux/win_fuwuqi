
#include "GameMysqlHandler.hpp"
#include "GameMysqlWork.hpp"
#include "sendclient.hpp"
#include "quanju.hpp"

GameMysqlHandler::GameMysqlHandler()
{

}

GameMysqlHandler::~GameMysqlHandler()
{

}

int GameMysqlHandler::OnServerMsg(const SSMsg& rSSMsg, int iFd)
{
	int iRet = -1;
	iRet = OnCheckSSMsg(rSSMsg, SS_MSGID_GameMysql);
	if (iRet < 0)
	{
		//��־���
		printf("RegisterLoginHandler OnCheckSSMsg : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginHandler CheckCSMsg : %d",iRet);
		return -1;
	}
	/*
	����Ĵ���ÿ��ģ���Handler���OnClientMsg������һ��
	*/
	const SSRegisterLoginReq &req = rSSMsg.body().registerloginreq();
	switch (req.cmd())
	{
	case SSRegisterLoginCmd_Register:					//ע�Ṧ��
		{
			iRet = OnRegisterReq(rSSMsg, iFd);
		}
		break;
	case SSRegisterLoginCmd_Login:						//��½����
		{
			iRet = OnLoginReq(rSSMsg, iFd);
		}
		break;
	case SSRegisterLoginCmd_Quit:						//�˳���Ϸ����
		{
			iRet = OnQuitReq(rSSMsg, iFd);
		}
		break;
	case SSRegisterLoginCmd_UpdateDatabase:						//�������ݿ⹦��
		{
			iRet = OnUpdateDatabaseReq(rSSMsg, iFd);
		}
		break;
	default:
		iRet = -1;
	}
	if (iRet < 0)
	{
		//�жϹ����߼��Ƿ���ȷ�������ܻ���ָ��������ָ�����ԭ�������ָ��Ϊ�գ��������ݷ�������
		printf("RegisterLoginHandler error : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginHandler error : %d",iRet);
	}
	
	return 0;
}


void* GameMysqlHandler::OnSSMsg(SSMsg& rMsg, uint64_t Uid, SSMsgID eMsgId, int CmdType)
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
	SSRegisterLoginRsp * pReq = pbody->mutable_registerloginrsp();
	if (!pReq)
	{
		return NULL;
	}
	SSRegisterLoginRspParam * pReqParam = pReq->mutable_rspparam();  //ע��ÿ��ģ��ֻҪ�жϵ��ò�νṹ��������ж��ˣ��Ϳ�ʼ���ж�Ӧָ���ж���
	if (!pReqParam)
	{
		return NULL;
	}
	
	//�����жϻ�ȡ��Ӧ���ڴ�ָ���ȥ
	if (CmdType == SSRegisterLoginCmd_Register)
	{
		SSRegisterRsp * pRegisterRsp = pReqParam->mutable_registerrsp();
		return (void*)pRegisterRsp;
	}
	else if (CmdType == SSRegisterLoginCmd_Login)
	{
		SSLoginRsp * pLoginRsp = pReqParam->mutable_loginrsp();
		return (void*)pLoginRsp;
	}
	////////
	return NULL;

}


int GameMysqlHandler::OnCheckSSMsg(const SSMsg& rMsg, SSMsgID eMsgId)
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



int GameMysqlHandler::OnRegisterReq(const SSMsg& rSSMsg, int iFd)
{
	const SSRegisterReq & rRegisterReq = rSSMsg.body().registerloginreq().reqparam().registerreq();

	SSMsg oSSMsg;
	//��ȡҪ���ͻ��˵������ڴ��ַ
	SSRegisterRsp* pRegisterRsp = static_cast<SSRegisterRsp*>(OnSSMsg(oSSMsg, rSSMsg.head().uid(), SS_MSGID_GameMysql, SSRegisterLoginCmd_Register)); 
	HANDCHECH_P(pRegisterRsp,-1);

	//��ʼִ�й����߼�
	int iRet = GameMysqlWork::Register(rRegisterReq,pRegisterRsp);
	//�����С��0֤�����ݴ��󣬲����Է���,������־��
	if (iRet < 0)
	{
		printf("RegisterLoginWork Register error : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginWork Register error : %d",iRet);
		return -3;
	}
	//��ʼ�������ݸ��ͻ���
	SendServer(iFd,&oSSMsg);
	return 0;

}



int GameMysqlHandler::OnLoginReq(const SSMsg& rSSMsg, int iFd)
{
	const SSLoginReq & rLoginReq = rSSMsg.body().registerloginreq().reqparam().loginreq();

	SSMsg oSSMsg;
	SSLoginRsp* pLoginRsp = static_cast<SSLoginRsp*>(OnSSMsg(oSSMsg,rSSMsg.head().uid(), SS_MSGID_GameMysql, SSRegisterLoginCmd_Login)); 
	HANDCHECH_P(pLoginRsp,-1);

	int iRet = GameMysqlWork::Login(rLoginReq,pLoginRsp);

	if (iRet < 0)
	{
		printf("RegisterLoginWork  Login error : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginWork Login error : %d",iRet);
		return -3;
	}
	SendServer(iFd,&oSSMsg);
	return 0;

}


int GameMysqlHandler::OnQuitReq(const SSMsg& rSSMsg, int iFd)
{
	const SSQuitReq & rQuitReq = rSSMsg.body().registerloginreq().reqparam().quitreq();
	//���л�role����Ȼ�󱣴������ݿ���
	int len = rQuitReq.ByteSize();
	char buff[len+1];
	if (!rQuitReq.SerializeToArray(buff,len)) 
	{ 
		std::cout << "OnQuitReq serialize Fail!" << std::endl; 
		MYLOG.printflog("OnQuitReq serialize Fail��"); 
		return -1;
	}
	std::string sRole(buff);
	MYSQLKU->ChangeDBRole(sRole,(int)rSSMsg.head().uid());
	MYSQLKU->PopUid((int)rSSMsg.head().uid());									//�������а����ߵ�UID��ȥ����ȥ
	return 0;
}


int GameMysqlHandler::OnUpdateDatabaseReq(const SSMsg& rSSMsg, int iFd)
{
	const SSQuitReq & rQuitReq = rSSMsg.body().registerloginreq().reqparam().quitreq();
	//���л�role����Ȼ�󱣴������ݿ���
	int len = rQuitReq.ByteSize();
	char buff[len+1];
	if (!rQuitReq.SerializeToArray(buff,len)) 
	{ 
		std::cout << "OnQuitReq serialize Fail!" << std::endl; 
		MYLOG.printflog("OnQuitReq serialize Fail��"); 
		return -1;
	}
	std::string sRole(buff);
	MYSQLKU->ChangeDBRole(sRole,(int)rSSMsg.head().uid());
	
	return 0;
}