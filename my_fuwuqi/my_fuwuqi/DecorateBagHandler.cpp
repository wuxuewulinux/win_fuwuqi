#include "DecorateBagHandler.hpp"
#include "quanju.hpp"
#include "enterfunction.hpp"
#include "DecorateBagWork.hpp"
#include "sendclient.hpp"

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
			iRet = OnFetchReq(rCSMsg, iFd);						//����������ʾ����
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

	CSDecorateBagFetchRsp * pFetchRsp = pReqParam->mutable_fetchrsp();				//��ȡ����չʾ�ṹָ���ڴ�
			

	/////////////////////////

	//�����жϻ�ȡ��Ӧ���ڴ�ָ���ȥ
	if (CmdType == CSDecorateBagCmd_Fetch)
	{
		return (void*)pFetchRsp;
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
	int iRet = DecorateBagWork::GenCSBagInfo( pRoleObj,rFetchReq, *pCSBagInfo);
	if (iRet == 0)
	{
		SendClient(iFd,&oCSMsg);
	}
	return 0;
}