

/*

所有游戏中的 配置 信息都写在 xml文件中，使用tinyxml第三方库进行读取 xml中的配置节点


*/



#include<string>
#include"serverconfig.hpp"

/*
<?xml version="1.0" encoding="gb2312"?> //使用gb2312编码格式，不然读取中文字符串出现乱码
*/

#ifndef __LOGIC_CONFIG_MANAGER_HPP__
#define __LOGIC_CONFIG_MANAGER_HPP__

#define LOGIC_CONFIG LogicConfigManager::Instance()

//LogicConfigManager类是专门管理所有配置模块

class LogicConfigManager
{
public:
	static LogicConfigManager * Instance();

	bool Init(std::string Path);

	ServerConfig & GetFightServerConfig(){return rFigServerConfig;}

private:
	LogicConfigManager();
	~LogicConfigManager();

	ServerConfig rFigServerConfig;       //服务器与据库的配置

	//如果还有其他模块的配置类就在以下添加

};

#endif