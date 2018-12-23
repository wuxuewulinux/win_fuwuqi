
#ifndef _UPDATEDATA_HPP_
#define _UPDATEDATA_HPP_

#include "RoleObj.hpp"
#include "../proto/SSmsg.pb.h"
#include "../proto/DBmsg.pb.h"

class UpdateData
{
public:
	UpdateData();
	~UpdateData();

	static void* OnSSMsg(SSMsg& rMsg, uint64_t Uid, SSMsgID eMsgId, int CmdType); 

	//int Status表示向聊天服务器发送某个状态的更改
	static int UpdateDatabase(CRoleObj* pRoleObj,int Status);			//更新该用户的数据库所有数据(及时保存数据到数据库)

private:

};





#endif