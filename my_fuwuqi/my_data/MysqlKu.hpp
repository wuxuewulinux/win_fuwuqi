
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

	int InitMysql(string user,string mima,string databases,vector<string> & tables);  //指定mysql的登录用户与密码与数据库名称

	//判断客户端登录情况：函数返回1表示账户不存在，返回2表示密码错误，返回3表示登录成功.返回4表示用户重复登录

	int Login(const string & zhanghu,const string & mima);  

	//调用mysql数据库返回该用户游戏名字 

	string GetName(int Uid);

	//获取role结构

	string GetDBRole(int Uid);

	//修改某个用户的role结构

	void ChangeDBRole(string & rDBRole,int Uid);

	//判断客户端注册情况：函数返回1表示账户存在，返回2表示名字存在，返回3表示注册成功.然后保存到mysql数据库

	int Register(const string & zhanghu,const string & mingzi);

	//调用mysql数据库返回该用户游戏积分

	//int GetJiFen(const string & zhanghu);

	//插入注册成功的游戏客户到mysql中

	void InsertAccount (const string & zhanghu,const string & mingzi,const string & mima,const string & DBRole);

	//修改mysql数据库中的游戏用户积分

	//void ChangeJiFen(int jifen,const string & zhanghu);

	//插入金币日志记录到数据库中

	//void InsertGoldLog(const struct goldlog & gold_log); 

	//插入一个用户uid到uid容器中

	void InsertUid(int userid){uid_rongqi.insert(userid);}

	//在uid容器查找是否存在该用户uid，为了判断是否重复登录

	bool FindUid(int userid);

	//从uid容器中删除一个用户uid

	void PopUid(int userid);

	//获取用户uid

	uint64_t GetUid(const string & zhanghu);


	//获取字段的属性测试

	//void cheshi();

	//void cheshitable(); 

	~MysqlKu(){mysql_close(&mysql);}

private:

	vector<string> table;     //数据库表的集合
	MYSQL mysql;
	MYSQL_RES *res;
	MYSQL_ROW row;

	time_t timep;        //获取当前系统时间
	set<int> uid_rongqi; //保存所有上线玩家的唯一uid
};

#endif