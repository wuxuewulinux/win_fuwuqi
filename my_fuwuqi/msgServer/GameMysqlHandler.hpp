
#ifndef _GAMEMYSQLHANDLER_HPP_
#define _GAMEMYSQLHANDLER_HPP_

#include "Handler.hpp"
#include "../proto/CSmsg.pb.h"
#include "quanju.hpp"

class GameMysqlHandler : public IHandler
{
public:
	GameMysqlHandler();
	virtual ~GameMysqlHandler();

	virtual int OnServerMsg(const CSMsg& rCSMsg, int iFd);//iFdΪio�׽���

	virtual int OnClientMsg(const CSMsg& rCSMsg, int iFd);

	static int OnSendRoleUpLine(RoleInfo * pRoleInfo,uint64_t rUid);		//����ѷ������߹㲥

	static int OnSendRoleQuitLine(RoleInfo * pRoleInfo,uint64_t rUid);		//����ѷ������߹㲥

	static int OnSendChangeStatus(RoleInfo * pRoleInfo,uint64_t rUid);		//����ѷ���״̬�ı�㲥


private:
	//����һ���ṹ�ڴ��ַ������eMsgIdΪ��һ��Э��ţ�CmdTypeΪ��2��Э��ţ�pMsgObj����һ���ṹ�Ŀյ�ַ����������ÿյ�ַ����ֵ
	//�ú���ÿ��ģ���handler���������������ÿ��ģ�����Լ��ú����ķ�װ

	 static void* OnCSMsg(CSMsg& rMsg, uint64_t Uid, CSMsgID eMsgId, int CmdType); //�����������������

	//���ͻ��˵����ݰ��Ƿ���ڣ���ȷ�ͷ���0������ȷ�ͷ��ظ���
	int OnCheckCSMsg(const CSMsg& rMsg, CSMsgID eMsgId);

	// ע: OnSSMsg �� OnCheckSSMsg �� OnServerMsg ����������ÿ��ģ���Handler�����Ը��ķ�װ������Ҫ�е�


	//����������ģ�鹦�ܵ����

	int OnMsgRegisterReq(const CSMsg& rCSMsg, int iFd);							
	int OnMsgLoginReq(const CSMsg& rCSMsg, int iFd);								
	int OnMsgQuitReq(const CSMsg& rCSMsg, int iFd);									
	int OnAskAddFriendReq(const CSMsg& rCSMsg, int iFd);						
	int OnSuccessAddFriendReq(const CSMsg& rCSMsg, int iFd);
	int OnDeleteFriendReq(const CSMsg& rCSMsg, int iFd);
	int OnSendChatReq(const CSMsg& rCSMsg, int iFd);
	int OnFindNameReq(const CSMsg& rCSMsg, int iFd);
	int OnChangeStatusReq(const CSMsg& rCSMsg, int iFd);
	int OnConnectSuccessReq(const CSMsg& rCSMsg, int iFd);

};


#endif