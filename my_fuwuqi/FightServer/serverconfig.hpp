#ifndef _SERVERCONFIG_HPP_
#define _SERVERCONFIG_HPP_

#include<vector>
#include"xmlnodeconfig.hpp"


struct FigthServer				//战斗服务器配置
{
	std::string ip;             //获取服务器IP地址

	int port;					//获取服务器端口号

};


struct GameServer				//游戏服务器配置
{
	std::string ip;             //获取服务器IP地址

	int port;					//获取服务器端口号
};

class ServerConfig
{
public:
	ServerConfig();
	~ServerConfig();

	bool Init(std::string configname);										//这个函数所有配置类都会声明一样的

	int InitFightServerConfig(TiXmlElement *RootElement);					//读取战斗服务器配置信息

	int InitGameServerConfig(TiXmlElement *RootElement);					//读取游戏服务器配置信息

	struct FigthServer * GetFightServerConfig(){return &m_FigthServer;}		//获取战斗服务器配置信息(IP地址和端口号）

	struct GameServer * GetGameServerConfig(){return &m_GameServer;}		//获取游戏服务器(IP地址和端口号）

private:

	struct FigthServer m_FigthServer;
	struct GameServer m_GameServer;
};


#endif