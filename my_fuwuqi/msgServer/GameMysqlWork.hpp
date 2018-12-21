
#ifndef _GAMEMYSQLWORK_HPP_
#define _GAMEMYSQLWORK_HPP_

#include "../proto/DBmsg.pb.h"
#include "../proto/SSmsg.pb.h"
#include "MysqlKu.hpp"

enum eRegister
{
	Register_Account_Fail = 1,
	Register_Name_Fail,
	Register_Success
};

enum eLogin
{
	Login_Account_Fail = 1,
	Login_Password_Fail,
	Login_Success,
	Login_Repeat
};

class GameMysqlWork
{
public:
	GameMysqlWork();
	~GameMysqlWork();

	//所有对数据库的操作逻辑都会在这里实现

	static int Register(const SSRegisterReq& rReq,SSRegisterRsp* pRsp);

	static int Login(const SSLoginReq& rReq,SSLoginRsp* pRsp);

private:


};


#endif