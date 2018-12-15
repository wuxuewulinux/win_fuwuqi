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
	CSBagRspParam * pReqParam = pReq->mutable_rspparam();  //注：每个模块只要判断到该层次结构就算结束判断了，就开始进行对应指针判断了
	if (!pReqParam)
	{
		return NULL;
	}
	//从这里开始增加结构指针就可以了。

	CSBagFetchRsp * pFetchRsp = pReqParam->mutable_fetchrsp();				//获取界面展示结构指针内存
	//CSRegisterRsp * pRegisterRsp = pReqParam->mutable_registerrsp();		

	/////////////////////////

	//进行判断获取对应的内存指针出去
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
		//日志输出
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
	case CSBagCmd_Use:					//先保留该物品使用按钮功能，需要时在封装他
		{
			iRet = OnUseReq(rCSMsg, iFd);
		}
		break;
	default:
		iRet = -1;
	}
	if (iRet < 0)
	{
		//判断功能逻辑是否正确，不可能会出现负数，出现负数的原因可能是指针为空，或者数据发生错误
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

