
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

	//��ȡ����ս����������Ϣ
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
	if (FightServerMap.size() == 0)
	{
		return -1;
	}
	std::map<int,FightServerInfo>::iterator iter;
	if (FightServerMap.size() == 1)
	{
		//ֻ��һ��ս��������
		iter = FightServerMap.begin();
		if (iter == FightServerMap.end())
		{
			return -2;
		}
		return iter->first;
	}
	//���ս�����������������Ҫ�ҳ��������ٵ��ĸ�ս��������
	iter = FightServerMap.begin();
	uint32_t iUserCount = iter->second.UserCount;
	int Id = iter->second.FightServerId;
	++iter;
	while (iter != FightServerMap.end())
	{
		if (iter->second.UserCount < iUserCount )
		{
			iUserCount = iter->second.UserCount;
			Id = iter->second.FightServerId;
		}
		++iter;
	}

	return Id;
}



void FightServerConfig::AddFightServerUserCount(int iId,uint32_t iCount)
{
	std::map<int,FightServerInfo>::iterator iter = FightServerMap.find(iId);
	if (FightServerMap.end() == iter)
	{
		return;
	}	
	iter->second.UserCount = iter->second.UserCount + iCount;
	return;
}


void FightServerConfig::DownFightServerUserCount(int iId,uint32_t iCount)
{
	std::map<int,FightServerInfo>::iterator iter = FightServerMap.find(iId);
	if (FightServerMap.end() == iter)
	{
		return;
	}	
	iter->second.UserCount = iter->second.UserCount - iCount;
	return;
}