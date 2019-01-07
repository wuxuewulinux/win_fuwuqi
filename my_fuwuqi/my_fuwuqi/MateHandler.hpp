
#ifndef _MATEHANDLER_HPP_
#define _MATEHANDLER_HPP_

#include "Handler.hpp"
#include "../proto/CSmsg.pb.h"
#include "../proto/SSmsg.pb.h"

class MateHandler : public IHandler
{
public:
	MateHandler();
	virtual ~MateHandler();

	virtual int OnClientMsg(const CSMsg& rCSMsg, int iFd); //iFdÎªioÌ×½Ó×Ö
	virtual int OnServerMsg(const SSMsg& rSSMsg);
	static void* OnCSMsg(CSMsg& rMsg, uint64_t Uid, CSMsgID eMsgId, int CmdType); 
private:

	void* OnSSMsg(SSMsg& rMsg, uint64_t Uid, SSMsgID eMsgId, int CmdType); 

	int OnCheckCSMsg(const CSMsg& rMsg, CSMsgID eMsgId);

	int OnCheckSSMsg(const SSMsg& rMsg, SSMsgID eMsgId);

	
private:

	int OnMateFetchReq(const CSMsg& rCSMsg, int iFd);
	int OnQuitMateFetchReq(const CSMsg& rCSMsg, int iFd);
	int OnNotButtonMateFetchReq(const CSMsg& rCSMsg, int iFd);
	int OnSuccessButtonMateFetchReq(const CSMsg& rCSMsg, int iFd);
	int OnEnterHeroShowBagReq(const CSMsg& rCSMsg, int iFd);
	int OnShowZhaDanBagReq(const CSMsg& rCSMsg, int iFd);
	
};





#endif