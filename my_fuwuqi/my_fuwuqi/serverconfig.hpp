#ifndef _SERVERCONFIG_HPP_
#define _SERVERCONFIG_HPP_

#include<vector>
#include"xmlnodeconfig.hpp"

struct my_server
{
	std::string ip;             //获取服务器IP地址

	int port;                  //获取服务器端口号

};


struct MysqlServer
{
	std::string ip;             //获取服务器IP地址

	int port;                  //获取服务器端口号

};

struct MsgServer
{
	std::string ip;             //获取服务器IP地址

	int port;                  //获取服务器端口号

};

struct my_mysql
{
	std::string user;                      //使用数据库的用户

	std::string mima;                      //登录数据库的密码

	std::string database;                  //访问数据库名称

	std::vector<std::string> table;        //数据库的所有表名称都保存在该容器中

};

class ServerConfig
{
public:
	ServerConfig();
	~ServerConfig();

	bool Init(std::string configname);     //这个函数所有配置类都会声明一样的

	int InitServerConfigg(TiXmlElement *RootElement);    //读取服务器配置信息

	int InitMysqlServerConfigg(TiXmlElement *RootElement);    //读取mysql服务器配置信息

	int InitMysqlConfigg(TiXmlElement *RootElement);    //读取数据库的配置信息
	
	int InitMysqlTableConfigg(TiXmlElement *RootElement);    //读取数据库中所有表的配置信息

	int InitMsgServerConfigg(TiXmlElement *RootElement);	//读取聊天服务器（IP,端口)配置信息

	struct my_server * GetServerConfig(){return &server;}    //获取服务器的配置信息（ip与端口）

	struct my_mysql * GetMysqlConfig(){return &mysql;}    //获取数据库所有配置信息

	struct MysqlServer * GetMysqlServerConfig(){return &mMysqlServer;}    //获取mysql服务器配置信息

	struct MsgServer * GetMsgServerConfig(){return &mMsgServer;}

	


private:

	struct my_server server;

	struct my_mysql mysql;

	struct MysqlServer mMysqlServer;

	struct MsgServer mMsgServer;
	
};


#endif