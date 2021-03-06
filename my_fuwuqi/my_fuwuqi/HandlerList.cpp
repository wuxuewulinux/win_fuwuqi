
#include "HandlerList.hpp"

IHandler* CHandlerList::m_apCSHandler[CS_MSGID_SIZE];

IHandler* CHandlerList::m_apSSHandler[SS_MSGID_SIZE];


RegisterLoginHandler CHandlerList::m_oRegisterLoginHandler;					//初始化登陆注册handler
CBagHandler CHandlerList::m_oBagHandler;									//初始化背包handler
DecorateBagHandler CHandlerList::m_oDecorateBagHandler;						//初始化装饰背包handler
ChatHandler CHandlerList::m_oChatHandler;									//聊天模块Handler
MateHandler CHandlerList::m_oMateHandler;									//匹配模块Handler


CHandlerList* gm_hand = NULL;

CHandlerList * CHandlerList::Instance()
{
	if (NULL == gm_hand)
	{
		gm_hand = new CHandlerList();
	}
	return gm_hand;
}


int CHandlerList::Init()  //把Handler压到m_apCSHandler数组中。
{
	//客户端与服务器
	RegisterHandler(CS_MSGID_RegisterLogin,		&m_oRegisterLoginHandler);
	RegisterHandler(CS_MSGID_BAG,		&m_oBagHandler);
	RegisterHandler(CS_MSGID_DecorateBAG,&m_oDecorateBagHandler);
	RegisterHandler(CS_MSGID_Chat,&m_oChatHandler);
	RegisterHandler(CS_MSGID_Mate,&m_oMateHandler);
	//服务器与服务器
	RegisterHandler(SS_MSGID_GameMysql,&m_oRegisterLoginHandler);
	RegisterHandler(SS_MSGID_FightServer,&m_oMateHandler);
	return 0;
}


int CHandlerList::RegisterHandler(uint32_t uiMsgID, IHandler* pHandler)
{
	//CS_MSGID_MAX和CS_MSGID_MIN是在协议文件CSMsgID枚举中，最大值和最小模块号

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


//获取触发需要调用的Handler模块

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