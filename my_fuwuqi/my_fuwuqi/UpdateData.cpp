
#include "UpdateData.hpp"
#include "quanju.hpp"
#include "SocketDBClient.hpp"
#include "sendclient.hpp"

UpdateData::UpdateData()
{


}


UpdateData::~UpdateData()
{


}


void* UpdateData::OnSSMsg(SSMsg& rMsg, uint64_t Uid, SSMsgID eMsgId, int CmdType)
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

	SSLoginReq * pLoginReq = pReqParam->mutable_loginreq();				//获取登陆结构指针内存
	SSRegisterReq * pRegisterReq = pReqParam->mutable_registerreq();	//获取注册结构指针内存
	SSQuitReq * pQuitReq = pReqParam->mutable_quitreq();				//获取退出结构指针内存或更新数据库指针内存
	/////////////////////////

	//进行判断获取对应的内存指针出去
	if (CmdType == SSRegisterLoginCmd_Register)
	{
		return (void*)pRegisterReq;
	}
	else if (CmdType == SSRegisterLoginCmd_Login)
	{
		return (void*)pLoginReq;
	}
	else if (CmdType == SSRegisterLoginCmd_Quit || CmdType == SSRegisterLoginCmd_UpdateDatabase)
	{
		return (void*)pQuitReq;
	}
	////////
	return NULL;
}



int UpdateData::UpdateDatabase(CRoleObj* pRoleObj,int Status)
{
	SSMsg oSSMsg;
	SSQuitReq* pQuitReq = static_cast<SSQuitReq*>(OnSSMsg(oSSMsg, pRoleObj->GetUid(), SS_MSGID_GameMysql, SSRegisterLoginCmd_UpdateDatabase)); 
	HANDCHECH_P(pQuitReq,-1);

	DBRoleInfo rRoleInfo;
	pRoleObj->GenDBRoleInfo(&rRoleInfo);			//把role来的数据全部给拉取到rRoleInfo协议结构中.
	pQuitReq->set_allocated_role(&rRoleInfo);
		
	
	SendServer(DBCLIENT->GetSocketIo(),&oSSMsg);

	return 0;
}