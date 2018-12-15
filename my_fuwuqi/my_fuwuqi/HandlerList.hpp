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

	//��Ϸ������ͻ��˷���
	static IHandler* m_apCSHandler[CS_MSGID_SIZE];
	
	//���ݿ��������Ϸ����
	static IHandler* m_apSSHandler[SS_MSGID_SIZE];
	

	//����ģ���Handler����ӵ�����
	
	static RegisterLoginHandler m_oRegisterLoginHandler;					//��½ע��ģ��handler
	static CBagHandler m_oBagHandler;										//����ģ��handler
	static DecorateBagHandler m_oDecorateBagHandler;						//װ�α���handler

};



#endif // !__HANLERLIST_HPP__