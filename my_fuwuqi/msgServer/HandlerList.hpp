#ifndef __HANLERLIST_HPP__
#define __HANLERLIST_HPP__

#include "Handler.hpp"
#include "GameMysqlHandler.hpp"

#define HANDLERLIST CHandlerList::Instance()

class CHandlerList
{
public:

	static CHandlerList * Instance();

	static int Init();

	static IHandler* GetHandler();

private:
	
	static IHandler* m_apSSHandler;
	
	static GameMysqlHandler m_oGameMysqlHandler;    //聊天服务器所有功能都在该handler中
};



#endif // !__HANLERLIST_HPP__