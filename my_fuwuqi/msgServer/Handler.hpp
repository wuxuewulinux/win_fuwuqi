
#ifndef __IHANDLER_HPP__
#define __IHANDLER_HPP__

#include "../proto/CSmsg.pb.h"

class IHandler
{
public:
	virtual ~IHandler();

	virtual int OnClientMsg(const CSMsg& rCSMsg, int iFd);
	virtual int OnServerMsg(const CSMsg& rCSMsg, int iFd);

};

#endif // !__IHANDLER_HPP__