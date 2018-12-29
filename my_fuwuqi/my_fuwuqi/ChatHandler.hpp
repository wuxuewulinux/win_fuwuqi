
#ifndef _CHATHANDLER_HPP_
#define _CHATHANDLER_HPP_

#include "Handler.hpp"
#include "../proto/CSmsg.pb.h"
#include "../proto/SSmsg.pb.h"

class ChatHandler :  public IHandler
{
public:
	ChatHandler();
	virtual ~ChatHandler();

	virtual int OnClientMsg(const CSMsg& rCSMsg, int iFd); 
	virtual int OnServerMsg(const SSMsg& rSSMsg);

	static void* OnCSMsg(CSMsg& rMsg, uint64_t Uid, CSMsgID eMsgId, int CmdType);

private:

};





#endif