
#ifndef _UPDATEDATA_HPP_
#define _UPDATEDATA_HPP_

#include "RoleObj.hpp"
#include "../proto/SSmsg.pb.h"
#include "../proto/DBmsg.pb.h"
#include "../proto/CSmsg.pb.h"

enum MsgStatusType
{
	GRADE = 1,
	RANK = 2,
	STATUS = 3,
	VIP = 4,
	HEAD = 5,
	CHATFRAME =6

};

class UpdateData
{
public:
	UpdateData();
	~UpdateData();

	static void* OnSSMsg(SSMsg& rMsg, uint64_t Uid, SSMsgID eMsgId, int CmdType); 

	//int Status表示向聊天服务器发送某个状态的更改
	static int UpdateDatabase(CRoleObj* pRoleObj,int Status);			//更新该用户的数据库所有数据(及时保存数据到数据库)

	//玩家状态(等级，段位，状态(战斗状态)，vip，头像ID，聊天ID)发生改变就要把该玩家发送给服务器

	static int UpMsgStatus(uint64_t Uid,uint32_t Value,uint32_t Type);	

private:

};





#endif