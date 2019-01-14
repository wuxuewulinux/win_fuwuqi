
#include "RegisterLoginWork.hpp"
#include "enterfunction.hpp"
#include "epoll_ku.hpp"
#include "../proto/DBmsg.pb.h"
#include "MateWork.hpp"

/*
��½��ע�ỹû��role��ĳ���
int RegisterLoginWork::Register(CRoleObj* pRole,const CSRegisterReq& rReq,CSRegisterRsp* pRsp)
{


	return 0;
}

int RegisterLoginWork::Login(CRoleObj* pRole,const CSLoginReq& rReq,CSLoginRsp* pRsp)
{


	return 0;
}
*/

int RegisterLoginWork::RegisterReq(const CSRegisterReq& rReq,SSRegisterReq* pReq)
{
	pReq->set_account(rReq.account());
	pReq->set_password(rReq.password());
	pReq->set_name(rReq.name());
	return 0;
}

int RegisterLoginWork::LoginReq(const CSLoginReq& rReq,SSLoginReq* pReq)
{
	pReq->set_account(rReq.account());
	pReq->set_password(rReq.password());
	return 0;
}


int RegisterLoginWork::QuitReq(CRoleObj* pRoleObj,SSQuitReq* pReq,int Fd,uint64_t UID)
{
//�˳������������һ���ǻ�û�е�¼��ɫ����Ϸ��һ�����Ѿ���½��ɫ����Ϸ
   if(UID > 0)
  {
	if (0 < GetUserRoom(UID))
	{
		//�ͻ��˻���ƥ������У�����һ��Ҫ�Ƴ�
		MateWork::QuitGame(UID,GetUserRoom(UID));
	}
	DBRoleInfo rRoleInfo;
	pRoleObj->GenDBRoleInfo(&rRoleInfo);			//��role��������ȫ������ȡ��rRoleInfoЭ��ṹ��.
	pReq->set_allocated_role(&rRoleInfo);
	//�Ѹ�roleɾ����
	DeleteRole(pRoleObj->GetUid());
	//��epoll�Ƴ���ͨ�Žṹ
	EPOLLKU->Epoll_ShanChu(Fd);
  }
  else
   {
    //��epoll�Ƴ���ͨ�Žṹ
	EPOLLKU->Epoll_ShanChu(Fd);
	return -1;
   }
	return 0;
}



int RegisterLoginWork::RegisterRsp(const SSRegisterRsp& rRsp,CSRegisterRsp* pRsp)
{
	pRsp->set_type(rRsp.type());

	return 0;
}

int RegisterLoginWork::LoginRsp(const SSLoginRsp& rRsp,CSLoginRsp* pRsp,int SocketIo)
{
	pRsp->set_type(rRsp.type());
	if (rRsp.type() == 3)
	{
		//��½�ɹ���Ҫ��ʼ����role���û����ߵ������У���roleҲ�����ͻ���������ʼ��
		CRoleObj rRole;
		rRole.Init();
		if (rRsp.has_role() && rRsp.role().has_uid())
		{
			rRole.SetRoleInfo(rRsp.role());			//��role��ÿ��ֵ��ֵ
			rRole.SetFd(SocketIo);
			rRole.GetDecorateBagMgr().AllCheckDecorateItemTime();			//ˢ������װ�α���ʱ��
			DBRoleInfo rRoleInfo;
			rRole.GenDBRoleInfo(&rRoleInfo);
			PushRole(rRsp.role().uid(),&rRole);		//�ɹ��İ�һ��roleѹ�뵽������
			pRsp->set_allocated_role(&rRoleInfo);
		}
	}
	return 0;
}
