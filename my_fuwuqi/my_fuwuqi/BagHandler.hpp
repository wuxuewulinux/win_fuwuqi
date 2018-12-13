#ifndef __BAGHANDLR_HPP__
#define __BAGHANDLR_HPP__

#include "../proto/SSmsg.pb.h"
#include "../proto/CSmsg.pb.h"
#include "Handler.hpp"

class CBagHandler : public IHandler
{
public:
	CBagHandler();
	virtual ~CBagHandler();
	
	virtual int OnClientMsg(const CSMsg& rCSMsg, int iFd);
	virtual int OnServerMsg(const SSMsg& rCSMsg );

private:

int OnCheckCSMsg(const CSMsg& rMsg, CSMsgID eMsgId);								//���ͻ���Э��

void* OnCSMsg(CSMsg& rMsg, uint64_t Uid, CSMsgID eMsgId, int CmdType);				//��ȡЭ���ڴ�ָ��

private:
	int OnFetchReq(const CSMsg& rCSMsg, int iFd);

	int OnUseReq(const CSMsg& rCSMsg, int iFd);
	
};

#endif // !__BAGHANDLR_HPP__
	
