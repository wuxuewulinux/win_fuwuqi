
#include "HandlerList.hpp"

IHandler* CHandlerList::m_apSSHandler;

FightServerHandler CHandlerList::m_oFightServerHandler;      //��ʼ�����ݿ�ģ��handler

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
	m_apSSHandler = &m_oFightServerHandler;
	return 0;
}


//��ȡ������Ҫ���õ�Handlerģ��

IHandler* CHandlerList::GetHandler()
{
	return m_apSSHandler;
}