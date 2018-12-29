
#include "HandlerList.hpp"

IHandler* CHandlerList::m_apSSHandler;

GameMysqlHandler CHandlerList::m_oGameMysqlHandler;      //初始化数据库模块handler

CHandlerList* gm_hand = NULL;

CHandlerList * CHandlerList::Instance()
{
	if (NULL == gm_hand)
	{
		gm_hand = new CHandlerList();
	}
	return gm_hand;
}

int CHandlerList::Init() 
{
	m_apSSHandler = &m_oGameMysqlHandler;
	return 0;
}


//获取触发需要调用的Handler模块

IHandler* CHandlerList::GetHandler()
{
	return m_apSSHandler;
}