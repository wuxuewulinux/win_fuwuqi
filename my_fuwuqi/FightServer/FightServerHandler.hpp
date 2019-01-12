
#ifndef _GAMEMYSQLHANDLER_HPP_
#define _GAMEMYSQLHANDLER_HPP_

#include "Handler.hpp"
#include "../proto/CSmsg.pb.h"
#include "../proto/SSmsg.pb.h"
#include "quanju.hpp"

class FightServerHandler : public IHandler
{
public:
	FightServerHandler();
	virtual ~FightServerHandler();

	virtual int OnServerMsg(const SSMsg& rSSMsg, int iFd);//iFdΪio�׽���

	virtual int OnClientMsg(const CSMsg& rCSMsg, int iFd);


private:
	
	 static void* OnCSMsg(CSMsg& rMsg, uint64_t Uid, CSMsgID eMsgId, int CmdType);

	 static void* OnSSMsg(SSMsg& rMsg, uint64_t Uid, SSMsgID eMsgId, int CmdType);

	int OnCheckCSMsg(const CSMsg& rMsg, CSMsgID eMsgId);

	int OnCheckSSMsg(const SSMsg& rMsg, SSMsgID eMsgId);

	//����������ģ�鹦�ܵ����

	

};


#endif