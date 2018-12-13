


#ifndef _REGISTERLOGINHANDLER_HPP_
#define _REGISTERLOGINHANDLER_HPP_

#include "Handler.hpp"
#include "../proto/CSmsg.pb.h"
#include "../proto/SSmsg.pb.h"
#include "SocketDBClient.hpp"

class RegisterLoginHandler : public IHandler
{
public:
	RegisterLoginHandler();
	virtual ~RegisterLoginHandler();

	virtual int OnClientMsg(const CSMsg& rCSMsg, int iFd); //iFdΪio�׽���
	virtual int OnServerMsg(const SSMsg& rSSMsg);

private:
	//����һ���ṹ�ڴ��ַ������eMsgIdΪ��һ��Э��ţ�CmdTypeΪ��2��Э��ţ�pMsgObj����һ���ṹ�Ŀյ�ַ����������ÿյ�ַ����ֵ
	//�ú���ÿ��ģ���handler���������������ÿ��ģ�����Լ��ú����ķ�װ

	void* OnCSMsg(CSMsg& rMsg, uint64_t Uid, CSMsgID eMsgId, int CmdType);  //�ͻ��������������

	void* OnSSMsg(SSMsg& rMsg, uint64_t Uid, SSMsgID eMsgId, int CmdType); //�����������������

	//���ͻ��˵����ݰ��Ƿ���ڣ���ȷ�ͷ���0������ȷ�ͷ��ظ���
	int OnCheckCSMsg(const CSMsg& rMsg, CSMsgID eMsgId);

	//������ݿ�����������������ݿ���Ƿ���ȷ
	int OnCheckSSMsg(const SSMsg& rMsg, SSMsgID eMsgId);

	// ע: OnCSMsg �� OnCheckCSMsg �� OnClientMsg ����������ÿ��ģ���Handler�����Ը��ķ�װ������Ҫ�е�

	
private:
	//����������ģ�鹦�ܵ����

	int OnRegisterReq(const CSMsg& rCSMsg, int iFd);							//ע�Ṧ��(����ͻ���)
	int OnLoginReq(const CSMsg& rCSMsg, int iFd);								//��½����(����ͻ���)
	int OnQuitReq(const CSMsg& rCSMsg, int iFd);								//��Ϸ�˳�����(����ͻ���)

	int OnRegisterRsp(const SSMsg& rSSMsg);							//ע�Ṧ��(���ؿͻ���)
	int OnLoginRsp(const SSMsg& rSSMsg);								//��½����(���ؿͻ���)
};


#endif // !_REGISTERLOGINHANDLER_HPP_
