
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
	SSRegisterLoginReqParam * pReqParam = pReq->mutable_reqparam();  //ע��ÿ��ģ��ֻҪ�жϵ��ò�νṹ��������ж��ˣ��Ϳ�ʼ���ж�Ӧָ���ж���
	if (!pReqParam)
	{
		return NULL;
	}
	//�����￪ʼ���ӽṹָ��Ϳ����ˡ�

	SSLoginReq * pLoginReq = pReqParam->mutable_loginreq();				//��ȡ��½�ṹָ���ڴ�
	SSRegisterReq * pRegisterReq = pReqParam->mutable_registerreq();	//��ȡע��ṹָ���ڴ�
	SSQuitReq * pQuitReq = pReqParam->mutable_quitreq();				//��ȡ�˳��ṹָ���ڴ��������ݿ�ָ���ڴ�
	/////////////////////////

	//�����жϻ�ȡ��Ӧ���ڴ�ָ���ȥ
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
	pRoleObj->GenDBRoleInfo(&rRoleInfo);			//��role��������ȫ������ȡ��rRoleInfoЭ��ṹ��.
	pQuitReq->set_allocated_role(&rRoleInfo);
		
	
	SendServer(DBCLIENT->GetSocketIo(),&oSSMsg);

	return 0;
}