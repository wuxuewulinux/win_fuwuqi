
#ifndef _GAMEMYSQLHANDLER_HPP_
#define _GAMEMYSQLHANDLER_HPP_

#include "Handler.hpp"
#include "../proto/DBmsg.pb.h"
#include "../proto/SSmsg.pb.h"


class GameMysqlHandler : public IHandler
{
public:
	GameMysqlHandler();
	virtual ~GameMysqlHandler();

	virtual int OnServerMsg(const SSMsg& rSSMsg, int iFd);//iFdΪio�׽���

private:
	//����һ���ṹ�ڴ��ַ������eMsgIdΪ��һ��Э��ţ�CmdTypeΪ��2��Э��ţ�pMsgObj����һ���ṹ�Ŀյ�ַ����������ÿյ�ַ����ֵ
	//�ú���ÿ��ģ���handler���������������ÿ��ģ�����Լ��ú����ķ�װ

	void* OnSSMsg(SSMsg& rMsg, uint64_t Uid, SSMsgID eMsgId, int CmdType); //�����������������

	//���ͻ��˵����ݰ��Ƿ���ڣ���ȷ�ͷ���0������ȷ�ͷ��ظ���
	int OnCheckSSMsg(const SSMsg& rMsg, SSMsgID eMsgId);

	// ע: OnSSMsg �� OnCheckSSMsg �� OnServerMsg ����������ÿ��ģ���Handler�����Ը��ķ�װ������Ҫ�е�


	//����������ģ�鹦�ܵ����

	int OnRegisterReq(const SSMsg& rSSMsg, int iFd);							//ע�Ṧ��
	int OnLoginReq(const SSMsg& rSSMsg, int iFd);								//��½����
	int OnQuitReq(const SSMsg& rSSMsg, int iFd);								//�˳�����	
	int OnUpdateDatabaseReq(const SSMsg& rSSMsg, int iFd);						//���¸�role�����ݿ����������
};


#endif