
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

	static int UpdateDatabase(CRoleObj* pRoleObj);			//���¸��û������ݿ���������(��ʱ�������ݵ����ݿ�)

private:

};





#endif