
#include "../proto/CSmsg.pb.h"
#include "../proto/SSmsg.pb.h"
#include "RoleObj.hpp"

class RegisterLoginWork
{

public:

	//每个模块都有他的Work类，该类是执行模块功能的逻辑
	/*
	想屏蔽掉 还没有该role类的出现
	static int Register(CRoleObj* pRole,const CSRegisterReq& rReq,CSRegisterRsp* pRsp);

	static int Login(CRoleObj* pRole,const CSLoginReq& rReq,CSLoginRsp* pRsp);
	*/
	static int RegisterReq(const CSRegisterReq& rReq,SSRegisterReq* pReq);

	static int LoginReq(const CSLoginReq& rReq,SSLoginReq* pReq);

	static int QuitReq(CRoleObj* pRoleObj,SSQuitReq* pReq,int Fd,uint64_t UID);

	static int RegisterRsp(const SSRegisterRsp& rRsp,CSRegisterRsp* pRsp);

	static int LoginRsp(const SSLoginRsp& rRsp,CSLoginRsp* pRsp,int SocketIo);

private: 

};