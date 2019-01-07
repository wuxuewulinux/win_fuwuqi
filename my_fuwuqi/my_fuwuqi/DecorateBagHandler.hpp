
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

	int OnCheckCSMsg(const CSMsg& rMsg, CSMsgID eMsgId);								//���ͻ���Э��

	void* OnCSMsg(CSMsg& rMsg, uint64_t Uid, CSMsgID eMsgId, int CmdType);				//��ȡЭ���ڴ�ָ��

private:
	int OnFetchReq(const CSMsg& rCSMsg, int iFd);										//װ�α���������ʾ

	int OnShowSetReq(const CSMsg& rCSMsg, int iFd);										//װ�α������ý���Ĭ����ʾ

	int OnVIPFetchReq(const CSMsg& rCSMsg, int iFd);									//VIPװ�α���������ʾ



};



#endif