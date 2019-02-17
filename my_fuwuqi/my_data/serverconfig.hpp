#ifndef _SERVERCONFIG_HPP_
#define _SERVERCONFIG_HPP_

#include<vector>
#include"xmlnodeconfig.hpp"


struct MysqlServer
{
	std::string ip;             //获取服务器IP地址

	int port;                  //获取服务器端口号

};

struct RdisServerConfig			//redis数据库配置信息（Ip地址和端口号）
{
	std::string ip;             //获取服务器IP地址

	int port;					//获取服务器端口号

};

//保存用户数据的数据库的配置信息
struct my_mysql
{
	std::string user;                      //使用数据库的用户

	std::string mima;                      //登录数据库的密码

	std::string database;                  //访问数据库名称

	std::vector<std::string> table;        //用户数据的数据库的所有表名称都保存在该容器中

};

//保存流水数据库的配置信息
struct BillMysql
{
	std::string user;                      //使用数据库的用户

	std::string mima;                      //登录数据库的密码

	std::string database;                  //访问数据库名称

	std::vector<std::string> BillTable;        //流水账单数据库的所有表名称都保存在该容器中

};

class ServerConfig
{
public:
	ServerConfig();
	~ServerConfig();

	bool Init(std::string configname);							//这个函数所有配置类都会声明一样的

	int InitMysqlServerConfigg(TiXmlElement *RootElement);		//读取mysql服务器配置信息

	int InitRedisServerConfig(TiXmlElement *RootElement);		//读取Redis服务器配置信息

	int InitMysqlConfigg(TiXmlElement *RootElement);			//读取用户数据库的配置信息

	int InitBillMysqlConfigg(TiXmlElement *RootElement);		//读取用户流水数据库的配置信息
	
	int InitMysqlTableConfigg(TiXmlElement *RootElement);		//读取数据库中所有表的配置信息

	int InitBillMysqlTableConfigg(TiXmlElement *RootElement);	//读取用户流水数据库中所有表的配置信息

	struct my_mysql * GetMysqlConfig(){return &mysql;}			//获取数据库所有配置信息（表，用户名，密码等等的配置)

	struct BillMysql * GetBillMysqlConfig(){return &mBillMysql;}			//获取账单流水数据库所有配置信息（表，用户名，密码等等的配置)

	struct MysqlServer * GetMysqlServerConfig(){return &mMysqlServer;}		//获取mysql服务器配置信息(IP地址和端口号）

	struct RdisServerConfig * GetRedisServerConfig(){return &mRdisServer;}	//获取redis服务器配置信息


private:

	struct my_mysql mysql;

	struct BillMysql mBillMysql;

	struct MysqlServer mMysqlServer;

	struct RdisServerConfig mRdisServer;
	
};


#endif