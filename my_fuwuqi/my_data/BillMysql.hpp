

#ifndef _BILLMYSQL_HPP_
#define _BILLMYSQL_HPP_

#include<time.h>
#include<mysql/mysql.h>
#include<stdio.h>
#include<string>
#include<iostream>
#include<stdlib.h>
#include<set>
#include<vector>
using namespace std;

#define BILLMYSQLKU BillMysqlKu::Instance()

class BillMysqlKu
{
public:

	BillMysqlKu();

	static BillMysqlKu * Instance();

	int InitBillMysql(string user,string mima,string databases,vector<string> & tables);  //ָ��mysql�ĵ�¼�û������������ݿ�����

	//����һ����ʯ���Ѽ�¼

	void InsertDiamondBill(uint64_t Uid,int Type,int BeginDiamond,int lastDiamond);

	//����һ��������Ѽ�¼

	void InsertGoldBill(uint64_t Uid,int Type,int BeginGold,int lastGold);


	~BillMysqlKu(){mysql_close(&mysql);}

private:

	vector<string> table;     //���ݿ��ļ���
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

};




#endif