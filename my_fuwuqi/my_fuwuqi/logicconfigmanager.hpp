

/*

所有游戏中的 配置 信息都写在 xml文件中，使用tinyxml第三方库进行读取 xml中的配置节点


*/



#include<string>
#include"mingchengconfig.hpp"
#include"serverconfig.hpp"
#include "TimeConfig.hpp"
#include "MapConfig.hpp"
#include "fightServerConfig.hpp"

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

	MingChengConfig & GetMingChengConfig(){return mingcheng;} 

	ServerConfig & GetServerMysqlConfig(){return server_mysql;}

	TimeConfig & GetTimeConfig(){return m_oTimeConfig;}

	MapConfig &	GetMapConfig(){return m_oMapConfig;}

	FightServerConfig & GetFightServerConfig(){return m_oFightServerConfig;}

private:
	LogicConfigManager();
	~LogicConfigManager();

	ServerConfig server_mysql;       //服务器与据库的配置

	MingChengConfig mingcheng;		//游戏称号的配置

	TimeConfig m_oTimeConfig;		//配置游戏触发时间(匹配时间，选择英雄界面时间)

	MapConfig m_oMapConfig;			//地图配置模块

	FightServerConfig m_oFightServerConfig;  //所有战斗服务器配置

	//如果还有其他模块的配置类就在以下添加

};

#endif