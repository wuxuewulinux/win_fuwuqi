
#include "HandlerList.hpp"

IHandler* CHandlerList::m_apSSHandler[SS_MSGID_SIZE];

/*
��ʼ�������о�̬Handler��
CLoginoutHandler	CHandlerList::m_oLoginoutHandler;
CCardHandler		CHandlerList::m_oCardHandler;
CGMHandler			CHandlerList::m_oGMHandler;

*/
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

int CHandlerList::Init()  //��Handlerѹ��m_apCSHandler�����С�
{

	/*
	�����ε���CS_MSGID_LOGINOUT_REQ��CSmsgЭ���ļ���CSMsgIDö�ٺţ�ÿ�ж��ٸ�ģ���Ҫ��������ȥ���

	RegisterHandler(CS_MSGID_LOGINOUT_REQ,		&m_oRegisterLoginHandler);
	RegisterHandler(CS_MSGID_CARD_REQ,			&m_oCardHandler );
	RegisterHandler(CS_MSGID_GM_REQ,			&m_oGMHandler );
	*/
	//�����ε����ݿ�������ĵ�½ģ�飬������ʵ����
	RegisterHandler(SS_MSGID_GameMysql, &m_oGameMysqlHandler);

	return 0;
}


int CHandlerList::RegisterHandler(uint32_t uiMsgID, IHandler* pHandler)
{
	//CS_MSGID_MAX��CS_MSGID_MIN����Э���ļ�CSMsgIDö���У����ֵ����Сģ���

	if (uiMsgID < SS_MSGID_MAX && uiMsgID > SS_MSGID_MIN)
	{
		int iMsgIdx = uiMsgID - SS_MSGID_MIN;
		m_apSSHandler[iMsgIdx] = pHandler;
	}
	return 0;
}


//��ȡ������Ҫ���õ�Handlerģ��

IHandler* CHandlerList::GetHandler(uint32_t uiMsgID)
{
	if (uiMsgID < SS_MSGID_MAX && uiMsgID > SS_MSGID_MIN)
	{
		int iMsgIdx = uiMsgID - SS_MSGID_MIN;
		return m_apSSHandler[iMsgIdx];
	}
	return NULL;
}