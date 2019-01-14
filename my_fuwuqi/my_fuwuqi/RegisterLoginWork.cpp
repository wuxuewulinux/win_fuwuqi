
#include "RegisterLoginWork.hpp"
#include "enterfunction.hpp"
#include "epoll_ku.hpp"
#include "../proto/DBmsg.pb.h"
#include "MateWork.hpp"

/*
登陆和注册还没有role类的出现
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
//退出有两种情况，一种是还没有登录角色进游戏，一种是已经登陆角色在游戏
   if(UID > 0)
  {
	if (0 < GetUserRoom(UID))
	{
		//客户端还在匹配队列中，所以一定要移除
		MateWork::QuitGame(UID,GetUserRoom(UID));
	}
	DBRoleInfo rRoleInfo;
	pRoleObj->GenDBRoleInfo(&rRoleInfo);			//把role来的数据全部给拉取到rRoleInfo协议结构中.
	pReq->set_allocated_role(&rRoleInfo);
	//把该role删除掉
	DeleteRole(pRoleObj->GetUid());
	//从epoll移除该通信结构
	EPOLLKU->Epoll_ShanChu(Fd);
  }
  else
   {
    //从epoll移除该通信结构
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
		//登陆成功就要开始加入role到用户上线的容器中，把role也发给客户端让他初始化
		CRoleObj rRole;
		rRole.Init();
		if (rRsp.has_role() && rRsp.role().has_uid())
		{
			rRole.SetRoleInfo(rRsp.role());			//给role类每个值赋值
			rRole.SetFd(SocketIo);
			rRole.GetDecorateBagMgr().AllCheckDecorateItemTime();			//刷新所有装饰背包时间
			DBRoleInfo rRoleInfo;
			rRole.GenDBRoleInfo(&rRoleInfo);
			PushRole(rRsp.role().uid(),&rRole);		//成功的把一个role压入到容器中
			pRsp->set_allocated_role(&rRoleInfo);
		}
	}
	return 0;
}
