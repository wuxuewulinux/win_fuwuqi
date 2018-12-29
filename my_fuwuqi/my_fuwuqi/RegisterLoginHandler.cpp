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
	//以上的代码差不多一样，有的数据类型不一样就改一下就可以了。
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
	//以上的代码差不多一样，有的数据类型不一样就改一下就可以了。

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
	CSRegisterLoginRspParam * pReqParam = pReq->mutable_rspparam();  //注：每个模块只要判断到该层次结构就算结束判断了，就开始进行对应指针判断了
	if (!pReqParam)
	{
		return NULL;
	}

	//从这里开始增加结构指针就可以了。
	//进行判断获取对应的内存指针出去
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
	SSRegisterLoginReqParam * pReqParam = pReq->mutable_reqparam();  //注：每个模块只要判断到该层次结构就算结束判断了，就开始进行对应指针判断了
	if (!pReqParam)
	{
		return NULL;
	}
	//从这里开始增加结构指针就可以了。
	//进行判断获取对应的内存指针出去
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
		//日志输出
		printf("RegisterLoginHandler CheckSSMsg : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginHandler CheckSSMsg : %d",iRet);
		return -1;
	}
	/*
	上面的代码每个模块的Handler类的OnClientMsg函数都一样
	*/
	const SSRegisterLoginRsp &rRsp = rSSMsg.body().registerloginrsp();
	switch (rRsp.cmd())
	{
	case SSRegisterLoginCmd_Register:					//注册功能
		{
			iRet = OnRegisterRsp(rSSMsg);
		}
		break;
	case SSRegisterLoginCmd_Login:						//登陆功能
		{
			iRet = OnLoginRsp(rSSMsg);
		}
		break;
	default:
		iRet = -1;
	}
	if (iRet < 0)
	{
		//判断功能逻辑是否正确，不可能会出现负数，出现负数的原因可能是指针为空，或者数据发生错误
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
		//日志输出
		printf("RegisterLoginHandler CheckCSMsg : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginHandler CheckCSMsg : %d",iRet);
		return -1;
	}
	/*
	上面的代码每个模块的Handler类的OnClientMsg函数都一样
	*/
	const CSRegisterLoginReq &req = rCSMsg.body().registerloginreq();
	switch (req.cmd())
	{
	case CSRegisterLoginCmd_Register:					//注册功能
		{
			iRet = OnRegisterReq(rCSMsg, iFd);
		}
		break;
	case CSRegisterLoginCmd_Login:						//登陆功能
		{
			iRet = OnLoginReq(rCSMsg, iFd);
		}
		break;
	case CSRegisterLoginCmd_Quit:						//游戏退出功能
		{
			iRet = OnQuitReq(rCSMsg, iFd);
		}
		break;
	default:
		iRet = -1;
	}
	if (iRet < 0)
	{
		//判断功能逻辑是否正确，不可能会出现负数，出现负数的原因可能是指针为空，或者数据发生错误
		printf("RegisterLoginHandler OnClientMsg error : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginHandler OnClientMsg error : %d",iRet);
	}
	/*
	等有数据发生改变时就必须更新数据库，想屏蔽掉，如果数据库需要更新就照这样的模式去书写
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
	//想把IO存下来
	DBCLIENT->PushIo(iFd);
	SSMsg oSSMsg;
	//获取要给客户端的数据内存地址
	SSRegisterReq* pRegisterReq = static_cast<SSRegisterReq*>(OnSSMsg(oSSMsg, DBCLIENT->GetMapSize(), SS_MSGID_GameMysql, SSRegisterLoginCmd_Register)); 
	HANDCHECH_P(pRegisterReq,-1);
	/*
	获取该客户端的Role类 注册还没有role类的出现
	CRoleObj* pRoleObj = GetRole(rCSMsg.head().uid());
	HANDCHECH_P(pRoleObj,-2);
	*/
	//开始执行功能逻辑
	int iRet = RegisterLoginWork::RegisterReq(rRegisterReq,pRegisterReq);
	//如果是小于0证明数据错误，不可以发送,输入日志里
	if (iRet < 0)
	{
		printf("RegisterLoginWork Register error : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginWork Register error : %d",iRet);
		return -3;
	}
	//开始发送数据给客户端
	SendServer(DBCLIENT->GetSocketIo(),&oSSMsg);
	return 0;
}


int RegisterLoginHandler::OnLoginReq(const CSMsg& rCSMsg, int iFd)
{
	const CSLoginReq & rLoginReq = rCSMsg.body().registerloginreq().reqparam().loginreq();
	//想把IO存下来
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
	//想把IO存下来
	DBCLIENT->PushIo(iFd);

	SSMsg oSSMsg;
	SSQuitReq* pQuitReq = static_cast<SSQuitReq*>(OnSSMsg(oSSMsg, rCSMsg.head().uid(), SS_MSGID_GameMysql, SSRegisterLoginCmd_Quit)); 
	HANDCHECH_P(pQuitReq,-1);

	CRoleObj* pRoleObj = GetRole(rCSMsg.head().uid());
	HANDCHECH_P(pRoleObj,-2);
	
	int iRet = RegisterLoginWork::QuitReq(pRoleObj,pQuitReq,iFd,rCSMsg.head().uid());
	
	{
		//向聊天服务器发送用户离线协议
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
	//获取要给客户端的数据内存地址
	CSRegisterRsp* pRegisterRsp = static_cast<CSRegisterRsp*>(OnCSMsg(oCSMsg, rSSMsg.head().uid(), CS_MSGID_RegisterLogin, CSRegisterLoginCmd_Register)); 
	HANDCHECH_P(pRegisterRsp,-1);
	//注册成功就要向聊天服务器发送消息
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
	//如果是小于0证明数据错误，不可以发送,输入日志里
	if (iRet < 0)
	{
		printf("RegisterLoginWork Register error : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginWork Register error : %d",iRet);
		return -3;
	}
	//开始发送数据给客户端
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
	//向聊天服务器发送登录成功协议
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