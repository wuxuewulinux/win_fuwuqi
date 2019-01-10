
#ifndef _MAPCONFIG_HPP_
#define _MAPCONFIG_HPP_

#include"xmlnodeconfig.hpp"
#include <map>

struct MapInfo 
{
	int MapId;
	std::string MapName;
};

class MapConfig
{
public:
	MapConfig();
	~MapConfig();

	bool Init(std::string configname);   

	int InitMapConfig(TiXmlElement *RootElement);

	int GetMaxValue(){return MapList.size();}


private:

	std::map<int,MapInfo> MapList;

};


#endif