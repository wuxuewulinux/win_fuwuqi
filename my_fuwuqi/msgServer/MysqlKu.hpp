
#ifndef _MYSQLKU_HPP_
#define _MYSQLKU_HPP_

#include<time.h>
#include<mysql/mysql.h>
#include<stdio.h>
#include<string>
#include<iostream>
#include<stdlib.h>
#include<set>
#include<vector>
#include "../proto/CSmsg.pb.h"
using namespace std;

enum RoleBase			//���������Ϣ�ֶ������ݿ��е����
{
	UID = 0,
	NAME = 1,
	GRADE = 2,
	RANK = 3,
	STATUS = 4,
	VIP = 5,
	HEAD = 6,
	CHATFRAME = 7,
	FRIENDLIST = 8
};

#define MYSQLKU MysqlKu::Instance()

class MysqlKu
{
public:

	MysqlKu();

	static MysqlKu * Instance();

	int InitMysql(string user,string mima,string databases,vector<string> & tables);  //ָ��mysql�ĵ�¼�û������������ݿ�����

	//�޸����������ĳ���û���״̬(��¼�ɹ�)��(���߹���)

	void MsgLoginSuccess(uint64_t Uid,int Status,CSMsgLoginSuccessRsp * rRsp);

	//���͸��û�����״̬���������ߵĺ��ѣ�(���߹���)

	int SendRoleUpLine(struct RoleInfo* pRole,FriendList* pList,CSMsgLoginSuccessRsp * rRsp);

	//�޸����������ĳ���û���״̬(����)��(���߹���)

	void MsgQuitLine(uint64_t Uid,int Status);

	//���͸��û�����״̬���������ߵĺ��ѣ�(���߹���)

	int SendRoleQuitLine(struct RoleInfo* pRole,FriendList* pList);

	//����µĺ��Ѿ�Ҫ���µĺ����������¸������ݿ�

	int UpFriendList(uint64_t Uid,FriendList* pList);

	//��Ӻ��ѳɹ�����

	int MsgSuccessAddFriend(uint64_t Uid,uint64_t Uid1,CSMsgSuccessAddFriendRsp * rRsp);

	//ɾ�����ѹ���

	void MsgDeleteFriend(uint64_t Uid,uint64_t Uid1);

	//���ĺ����б�ΪNULL״̬

	void ChangeFriendListNULL(uint64_t Uid);

	//�������

	int MsgFindName(const string & name,CSMsgFindNameRsp * pRsp);

	//״̬�����ı��Ҫ�㲥���������

	int SendRoleChangeStatus(struct RoleInfo* pRole,FriendList* pList);

	//���ĳ��״̬�����ı�

	int MsgChangeStatus(uint64_t Uid,uint32_t value,uint32_t type);

	//����ע��ɹ�����Ϸ�ͻ��������������mysql��

	void MsgInsertAccount(uint64_t Uid,const string & name);

	~MysqlKu(){mysql_close(&mysql);}

private:

	vector<string> table;     //���ݿ��ļ���
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	time_t timep;        //��ȡ��ǰϵͳʱ��
	set<int> uid_rongqi; //��������������ҵ�Ψһuid
};

#endif