
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
		//日志输出
		printf("RegisterLoginHandler OnCheckSSMsg : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginHandler CheckCSMsg : %d",iRet);
		return -1;
	}
	/*
	上面的代码每个模块的Handler类的OnClientMsg函数都一样
	*/
	const SSRegisterLoginReq &req = rSSMsg.body().registerloginreq();
	switch (req.cmd())
	{
	case SSRegisterLoginCmd_Register:					//注册功能
		{
			iRet = OnRegisterReq(rSSMsg, iFd);
		}
		break;
	case SSRegisterLoginCmd_Login:						//登陆功能
		{
			iRet = OnLoginReq(rSSMsg, iFd);
		}
		break;
	case SSRegisterLoginCmd_Quit:						//退出游戏功能
		{
			iRet = OnQuitReq(rSSMsg, iFd);
		}
		break;
	case SSRegisterLoginCmd_UpdateDatabase:						//更新数据库功能
		{
			iRet = OnUpdateDatabaseReq(rSSMsg, iFd);
		}
		break;
	default:
		iRet = -1;
	}
	if (iRet < 0)
	{
		//判断功能逻辑是否正确，不可能会出现负数，出现负数的原因可能是指针为空，或者数据发生错误
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
	SSRegisterLoginRspParam * pReqParam = pReq->mutable_rspparam();  //注：每个模块只要判断到该层次结构就算结束判断了，就开始进行对应指针判断了
	if (!pReqParam)
	{
		return NULL;
	}
	
	//进行判断获取对应的内存指针出去
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
	//以上的代码差不多一样，有的数据类型不一样就改一下就可以了。

}



int GameMysqlHandler::OnRegisterReq(const SSMsg& rSSMsg, int iFd)
{
	const SSRegisterReq & rRegisterReq = rSSMsg.body().registerloginreq().reqparam().registerreq();

	SSMsg oSSMsg;
	//获取要给客户端的数据内存地址
	SSRegisterRsp* pRegisterRsp = static_cast<SSRegisterRsp*>(OnSSMsg(oSSMsg, rSSMsg.head().uid(), SS_MSGID_GameMysql, SSRegisterLoginCmd_Register)); 
	HANDCHECH_P(pRegisterRsp,-1);

	//开始执行功能逻辑
	int iRet = GameMysqlWork::Register(rRegisterReq,pRegisterRsp);
	//如果是小于0证明数据错误，不可以发送,输入日志里
	if (iRet < 0)
	{
		printf("RegisterLoginWork Register error : %d",iRet);
		MYLOG.sprintf(BUFF,"RegisterLoginWork Register error : %d",iRet);
		return -3;
	}
	//开始发送数据给客户端
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
	//序列化role数据然后保存在数据库中
	int len = rQuitReq.ByteSize();
	char buff[len+1];
	if (!rQuitReq.SerializeToArray(buff,len)) 
	{ 
		std::cout << "OnQuitReq serialize Fail!" << std::endl; 
		MYLOG.printflog("OnQuitReq serialize Fail！"); 
		return -1;
	}
	std::string sRole(buff);
	MYSQLKU->ChangeDBRole(sRole,(int)rSSMsg.head().uid());
	MYSQLKU->PopUid((int)rSSMsg.head().uid());									//从容器中把上线的UID给去除出去
	return 0;
}


int GameMysqlHandler::OnUpdateDatabaseReq(const SSMsg& rSSMsg, int iFd)
{
	const SSQuitReq & rQuitReq = rSSMsg.body().registerloginreq().reqparam().quitreq();
	//序列化role数据然后保存在数据库中
	int len = rQuitReq.ByteSize();
	char buff[len+1];
	if (!rQuitReq.SerializeToArray(buff,len)) 
	{ 
		std::cout << "OnQuitReq serialize Fail!" << std::endl; 
		MYLOG.printflog("OnQuitReq serialize Fail！"); 
		return -1;
	}
	std::string sRole(buff);
	MYSQLKU->ChangeDBRole(sRole,(int)rSSMsg.head().uid());
	
	return 0;
}