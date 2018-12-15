#include "BagHandler.hpp"
#include "enterfunction.hpp"
#include "sendclient.hpp"
#include "BagWork.hpp"

CBagHandler::CBagHandler()
{

}

CBagHandler::~CBagHandler()
{

}

int CBagHandler::OnCheckCSMsg(const CSMsg& rMsg, CSMsgID eMsgId)
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
	const CSBagReq& rTmp = rBody.bagreq();
	if ( !(rTmp.has_cmd()) || !(rTmp.has_reqparam()))
	{
		return -5;
	}

	return 0;
}



void* CBagHandler::OnCSMsg(CSMsg& rMsg, uint64_t Uid, CSMsgID eMsgId, int CmdType)
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
	CSBagRsp * pReq = pbody->mutable_bagrsp();
	if (!pReq)
	{
		return NULL;
	}
	CSBagRspParam * pReqParam = pReq->mutable_rspparam();  //ע��ÿ��ģ��ֻҪ�жϵ��ò�νṹ��������ж��ˣ��Ϳ�ʼ���ж�Ӧָ���ж���
	if (!pReqParam)
	{
		return NULL;
	}
	//�����￪ʼ���ӽṹָ��Ϳ����ˡ�

	CSBagFetchRsp * pFetchRsp = pReqParam->mutable_fetchrsp();				//��ȡ����չʾ�ṹָ���ڴ�
	//CSRegisterRsp * pRegisterRsp = pReqParam->mutable_registerrsp();		

	/////////////////////////

	//�����жϻ�ȡ��Ӧ���ڴ�ָ���ȥ
	if (CmdType == CSBagCmd_Fetch)
	{
		return (void*)pFetchRsp;
	}
	/*
	else if (CmdType == CSBagCmd_Use)
	{
		return (void*)pLoginRsp;
	}
	*/
	return NULL;
}


int CBagHandler::OnClientMsg(const CSMsg& rCSMsg, int iFd)
{
	int iRet = -1;
	iRet = OnCheckCSMsg(rCSMsg, CS_MSGID_BAG);
	if (iRet < 0)
	{
		//��־���
		printf("CBagHandler CheckCSMsg : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginHandler CheckCSMsg : %d",iRet);
		return -1;
	}

	const CSBagReq &req = rCSMsg.body().bagreq();
	switch (req.cmd())
	{
	case CSBagCmd_Fetch:
		{
			iRet = OnFetchReq(rCSMsg, iFd);
		}
		break;
	case CSBagCmd_Use:					//�ȱ�������Ʒʹ�ð�ť���ܣ���Ҫʱ�ڷ�װ��
		{
			iRet = OnUseReq(rCSMsg, iFd);
		}
		break;
	default:
		iRet = -1;
	}
	if (iRet < 0)
	{
		//�жϹ����߼��Ƿ���ȷ�������ܻ���ָ��������ָ�����ԭ�������ָ��Ϊ�գ��������ݷ�������
		printf("CBagHandler OnClientMsg error : %d",iRet);
		MYLOG.sprintf(BUFF,"CBagHandler OnClientMsg error : %d",iRet);
	}

	return 0;	
}

int CBagHandler::OnServerMsg(const SSMsg& rSSMsg)
{
	return 0;
}

int CBagHandler::OnFetchReq(const CSMsg& rCSMsg, int iFd)
{
	CRoleObj* pRoleObj = GetRole(rCSMsg.head().uid());
	HANDCHECH_P(pRoleObj, -1);
		
	CSMsg oCSMsg;
	CSBagFetchRsp* pFetchRsp = static_cast<CSBagFetchRsp*>(OnCSMsg(oCSMsg, rCSMsg.head().uid(), CS_MSGID_BAG, CSBagCmd_Fetch));
	HANDCHECH_P(pFetchRsp, -2);

	CSBagBagInfo* pCSBagInfo = pFetchRsp->mutable_baginfo();
	int iRet = CBagFramework::GenCSBagInfo( pRoleObj, *pCSBagInfo );
	if (iRet == 0)
	{
		SendClient(iFd,&oCSMsg);
	}
	
	return 0;
}

int CBagHandler::OnUseReq(const CSMsg& rCSMsg, int iFd)
{
	
	return 0;
}

