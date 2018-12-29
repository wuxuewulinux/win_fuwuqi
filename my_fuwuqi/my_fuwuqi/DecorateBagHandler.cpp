#include "DecorateBagHandler.hpp"
#include "quanju.hpp"
#include "enterfunction.hpp"
#include "DecorateBagWork.hpp"
#include "sendclient.hpp"
#include "UpdateData.hpp"

DecorateBagHandler::DecorateBagHandler()
{


}
DecorateBagHandler::~DecorateBagHandler()
{


}


int DecorateBagHandler::OnClientMsg(const CSMsg& rCSMsg, int iFd)
{
	int iRet = -1;
	iRet = OnCheckCSMsg(rCSMsg, CS_MSGID_DecorateBAG);
	if (iRet < 0)
	{
		//��־���
		printf("DecorateBagHandler CheckCSMsg : %d",iRet);
		MYLOG.sprintf(BUFF,"DecorateBagHandler CheckCSMsg : %d",iRet);
		return -1;
	}

	const CSDecorateBagReq &req = rCSMsg.body().decoratebagreq();
	switch (req.cmd())
	{
	case CSDecorateBagCmd_Fetch:
		{
			iRet = OnFetchReq(rCSMsg, iFd);							//װ�α���������ʾ����
		}
		break;
	case CSDecorateBagCmd_ShowSet:
		{
			iRet = OnShowSetReq(rCSMsg, iFd);						//װ�α�������Ĭ����ʾ
		}
		break;
	case CSDecorateBagCmd_VipFetch:
		{
			iRet = OnVIPFetchReq(rCSMsg, iFd);						//VIPװ�α���������ʾ����
		}
		break;
	default:
		iRet = -1;
	}

	if (iRet < 0)
	{
		//�жϹ����߼��Ƿ���ȷ�������ܻ���ָ��������ָ�����ԭ�������ָ��Ϊ�գ��������ݷ�������
		printf("DecorateBagHandler OnClientMsg error : %d",iRet);
		MYLOG.sprintf(BUFF,"DecorateBagHandler OnClientMsg error : %d",iRet);
	}

	return 0;	
}



int DecorateBagHandler::OnServerMsg(const SSMsg& rCSMsg )
{


	return 0;
}


int DecorateBagHandler::OnCheckCSMsg(const CSMsg& rMsg, CSMsgID eMsgId)
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
	const CSDecorateBagReq& rTmp = rBody.decoratebagreq();
	if ( !(rTmp.has_cmd()) || !(rTmp.has_reqparam()))
	{
		return -5;
	}

	return 0;
}



void* DecorateBagHandler::OnCSMsg(CSMsg& rMsg, uint64_t Uid, CSMsgID eMsgId, int CmdType)
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
	CSDecorateBagRsp * pReq = pbody->mutable_decoratebagrsp();
	if (!pReq)
	{
		return NULL;
	}
	CSDecorateBagRspParam * pReqParam = pReq->mutable_rspparam();  //ע��ÿ��ģ��ֻҪ�жϵ��ò�νṹ��������ж��ˣ��Ϳ�ʼ���ж�Ӧָ���ж���
	if (!pReqParam)
	{
		return NULL;
	}

	//�����￪ʼ���ӽṹָ��Ϳ����ˡ�
	//�����жϻ�ȡ��Ӧ���ڴ�ָ���ȥ
	if (CmdType == CSDecorateBagCmd_Fetch)
	{
		CSDecorateBagFetchRsp * pFetchRsp = pReqParam->mutable_fetchrsp();
		return (void*)pFetchRsp;
	}
	else if (CmdType == CSDecorateBagCmd_VipFetch)
	{
		CSDecorateBagVIPFetchRsp * pVIPFetchRsp	= pReqParam->mutable_vipfetchrsp();
		return (void*)pVIPFetchRsp;
	}
	return NULL;
}


int DecorateBagHandler::OnFetchReq(const CSMsg& rCSMsg, int iFd)
{
	const CSDecorateBagFetchReq rFetchReq = rCSMsg.body().decoratebagreq().reqparam().fetchreq();

	CRoleObj* pRoleObj = GetRole(rCSMsg.head().uid());
	HANDCHECH_P(pRoleObj, -1);

	CSMsg oCSMsg;
	CSDecorateBagFetchRsp* pFetchRsp = static_cast<CSDecorateBagFetchRsp*>(OnCSMsg(oCSMsg, rCSMsg.head().uid(), CS_MSGID_DecorateBAG, CSDecorateBagCmd_Fetch));
	HANDCHECH_P(pFetchRsp, -2);

	pFetchRsp->set_type(rFetchReq.type());
	CSDecorateBagInfo* pCSBagInfo = pFetchRsp->mutable_decoratebaginfo();
	int iRet = DecorateBagWork::GenCSBagInfo( pRoleObj,rFetchReq, *pCSBagInfo,pFetchRsp);
	if (iRet == 0)
	{
		SendClient(iFd,&oCSMsg);
	}
	return 0;
}



int DecorateBagHandler::OnShowSetReq(const CSMsg& rCSMsg, int iFd)
{
	const CSDecorateBagShowSetReq rReq = rCSMsg.body().decoratebagreq().reqparam().showsetreq();

	CRoleObj* pRoleObj = GetRole(rCSMsg.head().uid());
	HANDCHECH_P(pRoleObj, -1);
	int iRet = DecorateBagWork::OnBagShowSet( pRoleObj,rReq);
	if(iRet == 0)
	{
		//�ж��Ƿ���VIP ͷ�� �����
		if (rReq.type() == VIP_TYPE)
		{
			UpdateData::UpMsgStatus(rCSMsg.head().uid(),rReq.id(),VIP);
		}
		if (rReq.type() == HEAD_TYPE)
		{
			UpdateData::UpMsgStatus(rCSMsg.head().uid(),rReq.id(),HEAD);
		}
		if (rReq.type() == CHATFRAME_TYPE)
		{
			UpdateData::UpMsgStatus(rCSMsg.head().uid(),rReq.id(),CHATFRAME);
		}
	}
	return iRet;
}



int DecorateBagHandler::OnVIPFetchReq(const CSMsg& rCSMsg, int iFd)
{
	const CSDecorateBagVIPFetchReq rFetchReq = rCSMsg.body().decoratebagreq().reqparam().vipfetchreq();

	CRoleObj* pRoleObj = GetRole(rCSMsg.head().uid());
	HANDCHECH_P(pRoleObj, -1);

	CSMsg oCSMsg;
	CSDecorateBagVIPFetchRsp* pFetchRsp = static_cast<CSDecorateBagVIPFetchRsp*>(OnCSMsg(oCSMsg, rCSMsg.head().uid(), CS_MSGID_DecorateBAG, CSDecorateBagCmd_VipFetch));
	HANDCHECH_P(pFetchRsp, -2);

	pFetchRsp->set_type(rFetchReq.type());
	int iRet = DecorateBagWork::GenCSVIPBagInfo( pRoleObj,rFetchReq, *pFetchRsp);
	if (iRet == 0)
	{
		SendClient(iFd,&oCSMsg);
	}

	return 0;
}