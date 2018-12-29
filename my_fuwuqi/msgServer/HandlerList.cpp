
#include "HandlerList.hpp"

IHandler* CHandlerList::m_apSSHandler;

GameMysqlHandler CHandlerList::m_oGameMysqlHandler;      //��ʼ�����ݿ�ģ��handler

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


//��ȡ������Ҫ���õ�Handlerģ��

IHandler* CHandlerList::GetHandler()
{
	return m_apSSHandler;
}