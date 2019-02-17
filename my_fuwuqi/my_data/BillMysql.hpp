

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

	int InitBillMysql(string user,string mima,string databases,vector<string> & tables);  //指定mysql的登录用户与密码与数据库名称

	//插入一条钻石消费记录

	void InsertDiamondBill(uint64_t Uid,int Type,int BeginDiamond,int lastDiamond);

	//插入一条金币消费记录

	void InsertGoldBill(uint64_t Uid,int Type,int BeginGold,int lastGold);


	~BillMysqlKu(){mysql_close(&mysql);}

private:

	vector<string> table;     //数据库表的集合
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

};




#endif