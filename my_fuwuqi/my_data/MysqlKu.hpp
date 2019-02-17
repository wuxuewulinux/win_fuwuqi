
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
using namespace std;

#define MYSQLKU MysqlKu::Instance()

class MysqlKu
{
public:

	MysqlKu();

	static MysqlKu * Instance();

	int InitMysql(string user,string mima,string databases,vector<string> & tables);  //ָ��mysql�ĵ�¼�û������������ݿ�����

	//�жϿͻ��˵�¼�������������1��ʾ�˻������ڣ�����2��ʾ������󣬷���3��ʾ��¼�ɹ�.����4��ʾ�û��ظ���¼

	int Login(const string & zhanghu,const string & mima);  

	//����mysql���ݿⷵ�ظ��û���Ϸ���� 

	string GetName(uint64_t Uid);

	//��ȡrole�ṹ

	string GetDBRole(uint64_t Uid);

	//�޸�ĳ���û���role�ṹ

	void ChangeDBRole(string & rDBRole,uint64_t Uid);

	//�޸���ʯ����

	void ChangeDiamond(int Diamond ,uint64_t Uid);

	//�޸Ľ������

	void ChangeGole(int Gold ,uint64_t Uid);

	//�ı��û��ĵ�¼ʱ�䣬Type��Ϊ����״̬������1����2��ע��ɹ�ʱ��Ҫ���øú���Ȼ��type����1.��¼�ɹ��͵��øú���type����2

	void ChangeLoginTime(uint64_t Uid,int Type);

	//�жϿͻ���ע���������������1��ʾ�˻����ڣ�����2��ʾ���ִ��ڣ�����3��ʾע��ɹ�.Ȼ�󱣴浽mysql���ݿ�

	int Register(const string & zhanghu,const string & mingzi);

	//����ע��ɹ�����Ϸ�ͻ���mysql��

	void InsertAccount(const string & zhanghu,const string & mingzi,const string & mima,const string & DBRole);

	
	//����һ���û�uid��uid������

	void InsertUid(uint64_t userid){uid_rongqi.insert(userid);}

	//��uid���������Ƿ���ڸ��û�uid��Ϊ���ж��Ƿ��ظ���¼

	bool FindUid(uint64_t userid);

	//��uid������ɾ��һ���û�uid

	void PopUid(uint64_t userid);

	//��ȡ�û�uid

	uint64_t GetUid(const string & zhanghu);


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