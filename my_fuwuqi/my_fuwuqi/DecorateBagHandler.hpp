
#ifndef _DECORATEBAGHANDLER_HPP_
#define _DECORATEBAGHANDLER_HPP_

#include "../proto/SSmsg.pb.h"
#include "../proto/CSmsg.pb.h"
#include "Handler.hpp"

enum BagType
{
	VIP_TYPE = 1,
	HEAD_TYPE = 2,
	CHATFRAME_TYPE = 3,
	ZHADAN_TYPE = 4,
	DIPAN_TYPE = 5,
	ZHANJI_TYPE = 6,
	SKILLONE_TYPE = 7,
	SKILLTWO_TYPE = 8,
	MAX_TYPE = 9
};

class DecorateBagHandler : public IHandler
{
public:
	DecorateBagHandler();
	virtual ~DecorateBagHandler();

	virtual int OnClientMsg(const CSMsg& rCSMsg, int iFd);
	virtual int OnServerMsg(const SSMsg& rCSMsg );

private:

	int OnCheckCSMsg(const CSMsg& rMsg, CSMsgID eMsgId);								//检查客户端协议

	void* OnCSMsg(CSMsg& rMsg, uint64_t Uid, CSMsgID eMsgId, int CmdType);				//获取协议内存指针

private:
	int OnFetchReq(const CSMsg& rCSMsg, int iFd);										//装饰背包界面显示

	int OnShowSetReq(const CSMsg& rCSMsg, int iFd);										//装饰背包设置界面默认显示

	int OnVIPFetchReq(const CSMsg& rCSMsg, int iFd);									//VIP装饰背包界面显示



};



#endif