
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

	virtual int OnClientMsg(const CSMsg& rCSMsg, int iFd); //iFdΪio�׽���
	virtual int OnServerMsg(const SSMsg& rSSMsg);
	static void* OnCSMsg(CSMsg& rMsg, uint64_t Uid, CSMsgID eMsgId, int CmdType); 
	static void* OnSSMsg(SSMsg& rMsg, uint64_t Uid, SSMsgID eMsgId, int CmdType); 

private:

	int OnCheckCSMsg(const CSMsg& rMsg, CSMsgID eMsgId);

	int OnCheckSSMsg(const SSMsg& rMsg, SSMsgID eMsgId);

	
private:

	//�ͻ�������Ϸ������
	int OnMateFetchReq(const CSMsg& rCSMsg, int iFd);
	int OnQuitMateFetchReq(const CSMsg& rCSMsg, int iFd);
	int OnNotButtonMateFetchReq(const CSMsg& rCSMsg, int iFd);
	int OnSuccessButtonMateFetchReq(const CSMsg& rCSMsg, int iFd);
	int OnEnterHeroShowBagReq(const CSMsg& rCSMsg, int iFd);
	int OnShowZhaDanBagReq(const CSMsg& rCSMsg, int iFd);

	//��Ϸ��������ս��������

	int OnFightEndReq(const SSMsg& rSSMsg);
	
};





#endif