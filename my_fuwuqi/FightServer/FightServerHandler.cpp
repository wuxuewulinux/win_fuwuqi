
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
		//日志输出
		printf("FightServerHandler OnCheckCSMsg : %d",iRet);
		MYLOG.sprintf(BUFF,"FightServerHandler CheckCSMsg : %d",iRet);
		return -1;
	}
	/*
	上面的代码每个模块的Handler类的OnClientMsg函数都一样
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
		//判断功能逻辑是否正确，不可能会出现负数，出现负数的原因可能是指针为空，或者数据发生错误
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
		//日志输出
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
	CSMateRspParam * pReqParam = pReq->mutable_rspparam();  //注：每个模块只要判断到该层次结构就算结束判断了，就开始进行对应指针判断了
	if (!pReqParam)
	{
		return NULL;
	}
	
	//进行判断获取对应的内存指针出去
	
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
	SSFightServerRspParam * pReqParam = pReq->mutable_rspparam();  //注：每个模块只要判断到该层次结构就算结束判断了，就开始进行对应指针判断了
	if (!pReqParam)
	{
		return NULL;
	}

	//进行判断获取对应的内存指针出去

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
	//以上的代码差不多一样，有的数据类型不一样就改一下就可以了。

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


































