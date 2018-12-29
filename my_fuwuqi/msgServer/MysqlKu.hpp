
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

enum RoleBase			//人物基本信息字段在数据库中的序号
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

	int InitMysql(string user,string mima,string databases,vector<string> & tables);  //指定mysql的登录用户与密码与数据库名称

	//修改聊天服务器某个用户的状态(登录成功)，(上线功能)

	void MsgLoginSuccess(uint64_t Uid,int Status,CSMsgLoginSuccessRsp * rRsp);

	//发送该用户上线状态给正在上线的好友，(上线功能)

	int SendRoleUpLine(struct RoleInfo* pRole,FriendList* pList,CSMsgLoginSuccessRsp * rRsp);

	//修改聊天服务器某个用户的状态(离线)，(离线功能)

	void MsgQuitLine(uint64_t Uid,int Status);

	//发送该用户离线状态给正在上线的好友，(离线功能)

	int SendRoleQuitLine(struct RoleInfo* pRole,FriendList* pList);

	//添加新的好友就要把新的好友数组重新更新数据库

	int UpFriendList(uint64_t Uid,FriendList* pList);

	//添加好友成功功能

	int MsgSuccessAddFriend(uint64_t Uid,uint64_t Uid1,CSMsgSuccessAddFriendRsp * rRsp);

	//删除好友功能

	void MsgDeleteFriend(uint64_t Uid,uint64_t Uid1);

	//更改好友列表为NULL状态

	void ChangeFriendListNULL(uint64_t Uid);

	//查找玩家

	int MsgFindName(const string & name,CSMsgFindNameRsp * pRsp);

	//状态发生改变就要广播给所有玩家

	int SendRoleChangeStatus(struct RoleInfo* pRole,FriendList* pList);

	//玩家某个状态发生改变

	int MsgChangeStatus(uint64_t Uid,uint32_t value,uint32_t type);

	//插入注册成功的游戏客户到聊天服务器的mysql中

	void MsgInsertAccount(uint64_t Uid,const string & name);

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