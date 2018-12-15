
#ifndef _DECORATEBAGHANDLER_HPP_
#define _DECORATEBAGHANDLER_HPP_

#include "../proto/SSmsg.pb.h"
#include "../proto/CSmsg.pb.h"
#include "Handler.hpp"

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


};



#endif