
#include "FightServerHandler.hpp"
#include "FightServerWork.hpp"
#include "sendclient.hpp"
#include "quanju.hpp"
#include "enterfunction.hpp"

FightServerHandler::FightServerHandler()
{

}

FightServerHandler::~FightServerHandler()
{

}


int FightServerHandler::OnClientMsg(const CSMsg& rCSMsg, int iFd)
{
	int iRet = -1;
	iRet = OnCheckCSMsg(rCSMsg, CS_MSGID_Mate);
	if (iRet < 0)
	{
		//��־���
		printf("FightServerHandler OnCheckCSMsg : %d",iRet);
		MYLOG.sprintf(BUFF,"FightServerHandler CheckCSMsg : %d",iRet);
		return -1;
	}
	/*
	����Ĵ���ÿ��ģ���Handler���OnClientMsg������һ��
	*/
	const CSMateReq &req = rCSMsg.body().matereq();
	switch (req.cmd())
	{
		/*
	case CSMsgServer_RegisterSuccess:					
		{
			iRet = OnMsgRegisterReq(rCSMsg, iFd);
		}
		break;
		*/
	default:
		iRet = -1;
	}
	if (iRet < 0)
	{
		//�жϹ����߼��Ƿ���ȷ�������ܻ���ָ��������ָ�����ԭ�������ָ��Ϊ�գ��������ݷ�������
		printf("FightServerHandler error : %d",iRet);
		MYLOG.sprintf(BUFF,"FightServerHandler error : %d",iRet);
	}

	return 0;
}



int FightServerHandler::OnServerMsg(const SSMsg& rSSMsg, int iFd)
{
	int iRet = -1;
	iRet = OnCheckSSMsg(rSSMsg, SS_MSGID_FightServer);
	if (iRet < 0)
	{
		//��־���
		printf("FightServerHandler OnCheckSSMsg : %d",iRet);
		MYLOG.sprintf(BUFF,"FightServerHandler OnCheckSSMsg : %d",iRet);
		return -1;
	}
	
	const SSFightServerReq &req = rSSMsg.body().fightserverreq();
	switch (req.cmd())
	{
		/*
	case CSMsgServer_RegisterSuccess:					
		{
			iRet = OnMsgRegisterReq(rCSMsg, iFd);
		}
		break;
		*/
	default:
		iRet = -1;
	}
	if (iRet < 0)
	{
		printf("FightServerHandler error : %d",iRet);
		MYLOG.sprintf(BUFF,"FightServerHandler error : %d",iRet);
	}
	
	return 0;
}


void* FightServerHandler::OnCSMsg(CSMsg& rMsg, uint64_t Uid, CSMsgID eMsgId, int CmdType)
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
	CSMateRsp * pReq = pbody->mutable_matersp();
	if (!pReq)
	{
		return NULL;
	}
	CSMateRspParam * pReqParam = pReq->mutable_rspparam();  //ע��ÿ��ģ��ֻҪ�жϵ��ò�νṹ��������ж��ˣ��Ϳ�ʼ���ж�Ӧָ���ж���
	if (!pReqParam)
	{
		return NULL;
	}
	
	//�����жϻ�ȡ��Ӧ���ڴ�ָ���ȥ
	
	return NULL;

}


void* FightServerHandler::OnSSMsg(SSMsg& rMsg, uint64_t Uid, SSMsgID eMsgId, int CmdType)
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
	SSFightServerRsp * pReq = pbody->mutable_fightserverrsp();
	if (!pReq)
	{
		return NULL;
	}
	SSFightServerRspParam * pReqParam = pReq->mutable_rspparam();  //ע��ÿ��ģ��ֻҪ�жϵ��ò�νṹ��������ж��ˣ��Ϳ�ʼ���ж�Ӧָ���ж���
	if (!pReqParam)
	{
		return NULL;
	}

	//�����жϻ�ȡ��Ӧ���ڴ�ָ���ȥ

	return NULL;
}



int FightServerHandler::OnCheckCSMsg(const CSMsg& rMsg, CSMsgID eMsgId)
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
	const CSMateReq& rTmp = rBody.matereq();
	if ( !(rTmp.has_cmd()) || !(rTmp.has_reqparam()) )
	{
		return -5;
	}

	return 0;
	//���ϵĴ�����һ�����е��������Ͳ�һ���͸�һ�¾Ϳ����ˡ�

}


int FightServerHandler::OnCheckSSMsg(const SSMsg& rMsg, SSMsgID eMsgId)
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
	const SSFightServerReq& rTmp = rBody.fightserverreq();
	if ( !(rTmp.has_cmd()) || !(rTmp.has_reqparam()) )
	{
		return -5;
	}

	return 0;
}


































