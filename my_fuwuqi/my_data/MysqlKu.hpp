
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

	string GetName(int Uid);

	//��ȡrole�ṹ

	string GetDBRole(int Uid);

	//�޸�ĳ���û���role�ṹ

	void ChangeDBRole(string & rDBRole,int Uid);

	//�жϿͻ���ע���������������1��ʾ�˻����ڣ�����2��ʾ���ִ��ڣ�����3��ʾע��ɹ�.Ȼ�󱣴浽mysql���ݿ�

	int Register(const string & zhanghu,const string & mingzi);

	//����mysql���ݿⷵ�ظ��û���Ϸ����

	//int GetJiFen(const string & zhanghu);

	//����ע��ɹ�����Ϸ�ͻ���mysql��

	void InsertAccount (const string & zhanghu,const string & mingzi,const string & mima,const string & DBRole);

	//�޸�mysql���ݿ��е���Ϸ�û�����

	//void ChangeJiFen(int jifen,const string & zhanghu);

	//��������־��¼�����ݿ���

	//void InsertGoldLog(const struct goldlog & gold_log); 

	//����һ���û�uid��uid������

	void InsertUid(int userid){uid_rongqi.insert(userid);}

	//��uid���������Ƿ���ڸ��û�uid��Ϊ���ж��Ƿ��ظ���¼

	bool FindUid(int userid);

	//��uid������ɾ��һ���û�uid

	void PopUid(int userid);

	//��ȡ�û�uid

	uint64_t GetUid(const string & zhanghu);


	//��ȡ�ֶε����Բ���

	//void cheshi();

	//void cheshitable(); 

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