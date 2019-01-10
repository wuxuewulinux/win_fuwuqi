
#include "MapConfig.hpp"

MapConfig::MapConfig()
{

}

MapConfig::~MapConfig()
{

}


bool MapConfig::Init(std::string configname)
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

	//读取所有匹配地图
	{
		TiXmlElement *element = RootElement->FirstChildElement("MateMap");
		if (NULL == element)
		{
			std::cout<<": no [server]."<<std::endl;
			return false;
		}

		iRet = InitMapConfig(element);
		if (iRet)
		{
			std::cout<<"InitMapConfig failed : "<<std::cout<<iRet<<std::endl;;
			return false;
		}
	}
	
	return true;
}

int MapConfig::InitMapConfig(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		int MapId = 0;
		if (!GetSubNodeValue(dataElement, "map_id", MapId) || MapId <= 0 )
		{
			return -1;
		}
		MapInfo cfg;
		cfg.MapId = MapId;
		if (!GetSubNodeValue(dataElement, "map_name", cfg.MapName) || cfg.MapName.empty() )
		{
			return -2;
		}
		MapList[cfg.MapId] = cfg;
		dataElement = dataElement->NextSiblingElement();

	}

	return 0;
}