#ifndef __HANLERLIST_HPP__
#define __HANLERLIST_HPP__

#include "Handler.hpp"
#include "FightServerHandler.hpp"

#define HANDLERLIST CHandlerList::Instance()

class CHandlerList
{
public:

	static CHandlerList * Instance();

	static int Init();

	static IHandler* GetHandler();

private:
	
	static IHandler* m_apSSHandler;
	
	static FightServerHandler m_oFightServerHandler;    //战斗服务器所有功能都在该handler中（匹配)
};



#endif // !__HANLERLIST_HPP__