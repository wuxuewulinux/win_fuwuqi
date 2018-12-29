
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

	//int Status��ʾ���������������ĳ��״̬�ĸ���
	static int UpdateDatabase(CRoleObj* pRoleObj,int Status);			//���¸��û������ݿ���������(��ʱ�������ݵ����ݿ�)

	//���״̬(�ȼ�����λ��״̬(ս��״̬)��vip��ͷ��ID������ID)�����ı��Ҫ�Ѹ���ҷ��͸�������

	static int UpMsgStatus(uint64_t Uid,uint32_t Value,uint32_t Type);	

private:

};





#endif