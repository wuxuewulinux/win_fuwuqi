
#ifndef __IHANDLER_HPP__
#define __IHANDLER_HPP__

#include "../proto/CSmsg.pb.h"
#include "../proto/SSmsg.pb.h"

class IHandler
{
public:
	virtual ~IHandler();

	virtual int OnClientMsg(const CSMsg& rCSMsg, int iFd);
	virtual int OnServerMsg(const SSMsg& rSSMsg);

};

#endif // !__IHANDLER_HPP__