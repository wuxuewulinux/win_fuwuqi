
#include "../proto/CSmsg.pb.h"
#include "../proto/SSmsg.pb.h"
#include "RoleObj.hpp"

class RegisterLoginWork
{

public:

	//ÿ��ģ�鶼������Work�࣬������ִ��ģ�鹦�ܵ��߼�
	/*
	�����ε� ��û�и�role��ĳ���
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