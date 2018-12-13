
#include "HandlerList.hpp"

IHandler* CHandlerList::m_apCSHandler[CS_MSGID_SIZE];

IHandler* CHandlerList::m_apSSHandler[SS_MSGID_SIZE];


RegisterLoginHandler CHandlerList::m_oRegisterLoginHandler;					//��ʼ����½ע��handler
CBagHandler CHandlerList::m_oBagHandler;									//��ʼ������handler

CHandlerList* gm_hand = NULL;

CHandlerList * CHandlerList::Instance()
{
	if (NULL == gm_hand)
	{
		gm_hand = new CHandlerList();
	}
	return gm_hand;
}


int CHandlerList::Init()  //��Handlerѹ��m_apCSHandler�����С�
{
	RegisterHandler(CS_MSGID_RegisterLogin,		&m_oRegisterLoginHandler);
	RegisterHandler(CS_MSGID_BAG,		&m_oBagHandler);

	return 0;
}


int CHandlerList::RegisterHandler(uint32_t uiMsgID, IHandler* pHandler)
{
	//CS_MSGID_MAX��CS_MSGID_MIN����Э���ļ�CSMsgIDö���У����ֵ����Сģ���

	if (uiMsgID < CS_MSGID_MAX && uiMsgID > CS_MSGID_MIN)
	{
		m_apCSHandler[uiMsgID] = pHandler;
	}
	else if (uiMsgID < SS_MSGID_MAX && uiMsgID > SS_MSGID_MIN)
	{
		int iMsgIdx = uiMsgID - SS_MSGID_MIN;
		m_apSSHandler[iMsgIdx] = pHandler;
	}
	return 0;
}


//��ȡ������Ҫ���õ�Handlerģ��

IHandler* CHandlerList::GetHandler(uint32_t uiMsgID)
{
	if (uiMsgID < CS_MSGID_MAX && uiMsgID > CS_MSGID_MIN)
	{
		return m_apCSHandler[uiMsgID];
	}
	else if (uiMsgID < SS_MSGID_MAX && uiMsgID > SS_MSGID_MIN)
	{
		int iMsgIdx = uiMsgID - SS_MSGID_MIN;
		return m_apSSHandler[iMsgIdx];
	}
	return NULL;
}