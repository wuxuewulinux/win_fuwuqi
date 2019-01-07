
#include "MateHandler.hpp"
#include "quanju.hpp"
#include "MateWork.hpp"
#include "sendclient.hpp"

MateHandler::MateHandler()
{

}

MateHandler::~MateHandler()
{

}


int MateHandler::OnServerMsg(const SSMsg& rSSMsg)
{


	return 0;
}


int MateHandler::OnClientMsg(const CSMsg& rCSMsg, int iFd)
{
	int iRet = -1;
	iRet = OnCheckCSMsg(rCSMsg, CS_MSGID_Mate);
	if (iRet < 0)
	{
		//��־���
		printf("MateHandler CheckCSMsg : %d",iRet);
		MYLOG.sprintf(BUFF,"MateHandler CheckCSMsg : %d",iRet);
		return -1;
	}

	const CSMateReq &req = rCSMsg.body().matereq();
	switch (req.cmd())
	{
	
	case CSMateCmd_MateFetch:
		{
			iRet = OnMateFetchReq(rCSMsg, iFd);
		}
		break;
	case CSMateCmd_QuitMateFetch:							//����ƥ��״̬��������ͻȻ����˳�
		{
			iRet = OnQuitMateFetchReq(rCSMsg, iFd);		
		}
		break;
	case CSMateCmd_NotButtonMateFetch:						//�ɹ�ƥ�乻��������ͬ��;ܾ���ť���������ѡ��ܾ���ť
		{
			iRet = OnNotButtonMateFetchReq(rCSMsg, iFd);		
		}
		break;
	case CSMateCmd_SuccessButtonMateFetch:					//�ɹ�ƥ�乻��������ͬ��;ܾ���ť���������ѡ��ͬ�ⰴť
		{
			iRet = OnSuccessButtonMateFetchReq(rCSMsg, iFd);		
		}
		break;
	case CSMateCmd_EnterHeroShowBag:						//ѡ�����װ�α���ѡ����ЧID��������ը������������
		{
			iRet = OnEnterHeroShowBagReq(rCSMsg, iFd);		
		}
		break;
	case CSMateCmd_ShowZhaDanBag:							//���ѡ��ը��
		{
			iRet = OnShowZhaDanBagReq(rCSMsg, iFd);		
		}
		break;
	default:
		iRet = -1;
	}
	if (iRet < 0)
	{
		printf("MateHandler OnClientMsg error : %d",iRet);
		MYLOG.sprintf(BUFF,"MateHandler OnClientMsg error : %d",iRet);
	}

	return 0;
}


void* MateHandler::OnSSMsg(SSMsg& rMsg, uint64_t Uid, SSMsgID eMsgId, int CmdType)
{


	return NULL;
}


void* MateHandler::OnCSMsg(CSMsg& rMsg, uint64_t Uid, CSMsgID eMsgId, int CmdType)
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
	//�����￪ʼ���ӽṹָ��Ϳ����ˡ�
	//�����жϻ�ȡ��Ӧ���ڴ�ָ���ȥ
	
	if (CmdType == CSMateCmd_EnterHeroShow)
	{
		CSEnterHeroShowRsp * pHeroShowRsp = pReqParam->mutable_enterheroshowrsp();
		return (void*)pHeroShowRsp;
	}
	else if (CmdType == CSMateCmd_SendUserHeroShow)
	{
		CSSendUserHeroShowRsp * pSendUserHeroShowRsp = pReqParam->mutable_senduserheroshowrsp();
		return (void*)pSendUserHeroShowRsp;
	}
	else if (CmdType == CSMateCmd_ShowZhaDanBag)
	{
		CSShowZhaDanBagRsp * pShowZhaDanBagRsp = pReqParam->mutable_showzhadanbagrsp();
		return (void*)pShowZhaDanBagRsp;
	}
	return NULL;
}


int MateHandler::OnCheckSSMsg(const SSMsg& rMsg, SSMsgID eMsgId)
{


	return 0;
}


int MateHandler::OnCheckCSMsg(const CSMsg& rMsg, CSMsgID eMsgId)
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
	if ( !(rTmp.has_cmd()) || !(rTmp.has_reqparam()))
	{
		return -5;
	}

	return 0;
}


int MateHandler::OnMateFetchReq(const CSMsg& rCSMsg, int iFd)
{
	const CSMateFetchReq& Req = rCSMsg.body().matereq().reqparam().matefetchreq();
	int iRet = MateWork::MateFetch(Req);
	if (iRet < 0)
	{
		printf("MateWork MateFetch error : %d",iRet);
		MYLOG.sprintf(BUFF,"MateWork MateFetch error : %d",iRet);
		return -3;
	}
	return iRet;
}


int MateHandler::OnQuitMateFetchReq(const CSMsg& rCSMsg, int iFd)
{
	const CSQuitMateFetchReq& Req = rCSMsg.body().matereq().reqparam().quitmatefetchreq();
	int iRet = MateWork::QuitMateFetch(Req);
	if (iRet < 0)
	{
		printf("MateWork QuitMateFetch error : %d",iRet);
		MYLOG.sprintf(BUFF,"MateWork QuitMateFetch error : %d",iRet);
		return -3;
	}

	return iRet;
}



int MateHandler::OnNotButtonMateFetchReq(const CSMsg& rCSMsg, int iFd)
{
	const CSNotButtonMateFetchReq& Req = rCSMsg.body().matereq().reqparam().notbuttonmatefetchreq();
	int iRet = MateWork::NotButtonMateFetch(Req);
	if (iRet < 0)
	{
		printf("MateWork NotButtonMateFetch error : %d",iRet);
		MYLOG.sprintf(BUFF,"MateWork NotButtonMateFetch error : %d",iRet);
		return -3;
	}

	return iRet;
}


int MateHandler::OnSuccessButtonMateFetchReq(const CSMsg& rCSMsg, int iFd)
{
	const CSSuccessButtonMateFetchReq& Req = rCSMsg.body().matereq().reqparam().successbuttonmatefetchreq();
	int iRet = MateWork::SuccessButtonMateFetch(Req);
	if (iRet < 0)
	{
		printf("MateWork SuccessButtonMateFetch error : %d",iRet);
		MYLOG.sprintf(BUFF,"MateWork SuccessButtonMateFetch error : %d",iRet);
		return -3;
	}
	return iRet;
}


int MateHandler::OnEnterHeroShowBagReq(const CSMsg& rCSMsg, int iFd)
{
	const CSEnterHeroShowBagReq& Req = rCSMsg.body().matereq().reqparam().enterheroshowbagreq();
	int iRet = MateWork::EnterHeroShowBag(Req);
	if (iRet < 0)
	{
		printf("MateWork EnterHeroShowBag error : %d",iRet);
		MYLOG.sprintf(BUFF,"MateWork EnterHeroShowBag error : %d",iRet);
		return -3;
	}
	return iRet;
}



int MateHandler::OnShowZhaDanBagReq(const CSMsg& rCSMsg, int iFd)
{
	const CSShowZhaDanBagReq& Req = rCSMsg.body().matereq().reqparam().showzhadanbagreq();

	CSMsg oCSMsg;
	CSShowZhaDanBagRsp* Rsp = static_cast<CSShowZhaDanBagRsp*>(MateHandler::OnCSMsg(oCSMsg, rCSMsg.head().uid(), CS_MSGID_Mate, CSMateCmd_ShowZhaDanBag));
	HANDCHECH_P(Rsp, -1);
	int iRet = MateWork::ShowZhaDanBag(Req,Rsp);
	if (iRet < 0)
	{
		printf("MateWork ShowZhaDanBag error : %d",iRet);
		MYLOG.sprintf(BUFF,"MateWork ShowZhaDanBag error : %d",iRet);
		return -3;
	}
	if (iRet == 0)
	{
		SendClient(iFd,&oCSMsg);
	}
	return iRet;
}