
#ifndef __IHANDLER_HPP__
#define __IHANDLER_HPP__

#include "../proto/SSmsg.pb.h"


class IHandler
{
public:
	virtual ~IHandler();

	//virtual int OnClientMsg(const SSMsg& rSSMsg, int iFd);
	virtual int OnServerMsg(const SSMsg& rSSMsg, int iFd);

};

#endif // !__IHANDLER_HPP__