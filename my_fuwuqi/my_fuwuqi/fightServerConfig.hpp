
#ifndef _FIGHTSERVERCONFIG_HPP_
#define _FIGHTSERVERCONFIG_HPP_

#include"xmlnodeconfig.hpp"
#include <map>

struct FightServerInfo
{
	int FightServerId;
	int FightServerPort;
	std::string FightServerIp;
	uint32_t UserCount;				//用户连接人数，多少个人正在连接该战斗服务器
};


class FightServerConfig
{
public:
	FightServerConfig();
	~FightServerConfig();

	bool Init(std::string configname);   

	int InitFightServerConfig(TiXmlElement *RootElement);

	FightServerInfo* GetFightServerById(int iFightServerId);	//按战斗服务器ID获取该战斗服务器信息

	int GetEmptyFightServerID();								//获取一个空闲的战斗服务器ID给战斗房间的玩家.

private:

	std::map<int,FightServerInfo> FightServerMap;

};


#endif