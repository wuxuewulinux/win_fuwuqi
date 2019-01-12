#ifndef _SERVERCONFIG_HPP_
#define _SERVERCONFIG_HPP_

#include<vector>
#include"xmlnodeconfig.hpp"

struct my_server
{
	std::string ip;             //获取服务器IP地址

	int port;                  //获取服务器端口号

};


struct msgServer
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

	bool Init(std::string configname);										//这个函数所有配置类都会声明一样的

	int InitMsgServerConfigg(TiXmlElement *RootElement);					//读取mysql服务器配置信息

	int InitMysqlConfigg(TiXmlElement *RootElement);						//读取数据库的配置信息
	
	int InitMysqlTableConfigg(TiXmlElement *RootElement);					//读取数据库中所有表的配置信息

	struct my_mysql * GetMysqlConfig(){return &mysql;}						//获取数据库所有配置信息（表，用户名，密码等等的配置)

	struct msgServer * GetMsgServerConfig(){return &m_msgServer;}			//获取聊天服务器配置信息(IP地址和端口号）

	


private:

	struct my_mysql mysql;

	struct msgServer m_msgServer;
	
};


#endif