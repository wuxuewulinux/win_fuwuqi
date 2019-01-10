
#include "fightServerConfig.hpp"

FightServerConfig::FightServerConfig()
{


}


FightServerConfig::~FightServerConfig()
{

}


bool FightServerConfig::Init(std::string configname)
{
	int iRet;
	TiXmlDocument document;

	if (configname == "" || !document.LoadFile(configname.c_str()))
	{
		std::cout<<"ServerConfig Load FootPrint Error!"<<std::endl;
		return false;
	}

	TiXmlElement *RootElement = document.RootElement();
	if (NULL == RootElement)
	{
		std::cout<<"xml root node error."<<std::endl;
		return false;
	}

	//读取所有战斗服务器信息
	{
		TiXmlElement *element = RootElement->FirstChildElement("FightServer");
		if (NULL == element)
		{
			std::cout<<": no [server]."<<std::endl;
			return false;
		}

		iRet = InitFightServerConfig(element);
		if (iRet)
		{
			std::cout<<"InitFightServerConfig failed : "<<std::cout<<iRet<<std::endl;;
			return false;
		}
	}

	return true;
}

int FightServerConfig::InitFightServerConfig(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		int FightServerId = 0;
		if (!GetSubNodeValue(dataElement, "fight_server_id", FightServerId) || FightServerId < 0 )
		{
			return -1;
		}
		FightServerInfo cfg;
		cfg.UserCount = 0;
		cfg.FightServerId = FightServerId;
		if (!GetSubNodeValue(dataElement, "fight_server_ip", cfg.FightServerIp) || cfg.FightServerIp.empty() )
		{
			return -2;
		}
		if (!GetSubNodeValue(dataElement, "fight_server_port", cfg.FightServerPort) || cfg.FightServerPort < 0 )
		{
			return -3;
		}
		FightServerMap[cfg.FightServerId] = cfg;
		dataElement = dataElement->NextSiblingElement();

	}

	return 0;
}



FightServerInfo* FightServerConfig::GetFightServerById(int iFightServerId)
{
	std::map<int,FightServerInfo>::iterator iter = FightServerMap.find(iFightServerId);
	if (FightServerMap.end() != iter)
	{
		return &iter->second;
	}	
	return NULL;
}



int FightServerConfig::GetEmptyFightServerID()
{
	if (FightServerMap.size() == 1)
	{
		//只有一个战斗服务器
		std::map<int,FightServerInfo>::iterator iter = FightServerMap.begin();
		if (iter == FightServerMap.end())
		{
			return -1;
		}
		return iter->first;
	}
	//多个战斗服务器的情况，就要找出人数最少的哪个战斗服务器


	return 0;
}