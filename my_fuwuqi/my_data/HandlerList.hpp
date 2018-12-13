#ifndef __HANLERLIST_HPP__
#define __HANLERLIST_HPP__

#include "Handler.hpp"
#include "GameMysqlHandler.hpp"

#define HANDLERLIST CHandlerList::Instance()

const int SS_MSGID_SIZE = SS_MSGID_MAX - SS_MSGID_MIN;

class CHandlerList
{
public:

	static CHandlerList * Instance();

	static int Init();

	static IHandler* GetHandler(uint32_t uiMsgID);

private:
	static int RegisterHandler(uint32_t uiMsgID, IHandler* pHandler);

	//数据库服务与游戏服务
	static IHandler* m_apSSHandler[SS_MSGID_SIZE];
	


	/*
	//把你模块的Handler类添加到这里
	static CLoginoutHandler 	m_oLoginoutHandler;  //CLoginoutHandler为模块Handler的类
	static CCardHandler 		m_oCardHandler;
	static CGMHandler 			m_oGMHandler;
	*/

	//先屏蔽掉数据库服务的登陆注册模块，等下在实现他
	static GameMysqlHandler m_oGameMysqlHandler;    //数据库模块handler
};



#endif // !__HANLERLIST_HPP__