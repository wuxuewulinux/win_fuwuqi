
#include "HandlerList.hpp"

IHandler* CHandlerList::m_apSSHandler[SS_MSGID_SIZE];

/*
初始化你所有静态Handler类
CLoginoutHandler	CHandlerList::m_oLoginoutHandler;
CCardHandler		CHandlerList::m_oCardHandler;
CGMHandler			CHandlerList::m_oGMHandler;

*/
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

int CHandlerList::Init()  //把Handler压到m_apCSHandler数组中。
{

	/*
	先屏蔽掉，CS_MSGID_LOGINOUT_REQ是CSmsg协议文件的CSMsgID枚举号，每有多少个模块就要按照这样去添加

	RegisterHandler(CS_MSGID_LOGINOUT_REQ,		&m_oRegisterLoginHandler);
	RegisterHandler(CS_MSGID_CARD_REQ,			&m_oCardHandler );
	RegisterHandler(CS_MSGID_GM_REQ,			&m_oGMHandler );
	*/
	//先屏蔽掉数据库服务器的登陆模块，等下在实现他
	RegisterHandler(SS_MSGID_GameMysql, &m_oGameMysqlHandler);

	return 0;
}


int CHandlerList::RegisterHandler(uint32_t uiMsgID, IHandler* pHandler)
{
	//CS_MSGID_MAX和CS_MSGID_MIN是在协议文件CSMsgID枚举中，最大值和最小模块号

	if (uiMsgID < SS_MSGID_MAX && uiMsgID > SS_MSGID_MIN)
	{
		int iMsgIdx = uiMsgID - SS_MSGID_MIN;
		m_apSSHandler[iMsgIdx] = pHandler;
	}
	return 0;
}


//获取触发需要调用的Handler模块

IHandler* CHandlerList::GetHandler(uint32_t uiMsgID)
{
	if (uiMsgID < SS_MSGID_MAX && uiMsgID > SS_MSGID_MIN)
	{
		int iMsgIdx = uiMsgID - SS_MSGID_MIN;
		return m_apSSHandler[iMsgIdx];
	}
	return NULL;
}