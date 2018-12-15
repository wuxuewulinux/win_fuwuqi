#ifndef __HANLERLIST_HPP__
#define __HANLERLIST_HPP__

#include "Handler.hpp"
#include "RegisterLoginHandler.hpp"
#include "BagHandler.hpp"
#include "DecorateBagHandler.hpp"

#define HANDLERLIST CHandlerList::Instance()

const int CS_MSGID_SIZE = CS_MSGID_MAX;
const int SS_MSGID_SIZE = SS_MSGID_MAX - SS_MSGID_MIN;

class CHandlerList
{
public:

	static CHandlerList * Instance();

	static int Init();

	static IHandler* GetHandler(uint32_t uiMsgID);

private:
	static int RegisterHandler(uint32_t uiMsgID, IHandler* pHandler);

	//游戏服务与客户端服务
	static IHandler* m_apCSHandler[CS_MSGID_SIZE];
	
	//数据库服务与游戏服务
	static IHandler* m_apSSHandler[SS_MSGID_SIZE];
	

	//把你模块的Handler类添加到这里
	
	static RegisterLoginHandler m_oRegisterLoginHandler;					//登陆注册模块handler
	static CBagHandler m_oBagHandler;										//背包模块handler
	static DecorateBagHandler m_oDecorateBagHandler;						//装饰背包handler

};



#endif // !__HANLERLIST_HPP__