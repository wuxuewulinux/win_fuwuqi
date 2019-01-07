
#ifndef _TIMECONFIG_HPP_
#define _TIMECONFIG_HPP_

#include"xmlnodeconfig.hpp"

class TimeConfig
{
public:
	TimeConfig();
	~TimeConfig();

	bool Init(std::string configname);   

	int InitMateFetchConfig(TiXmlElement *RootElement);

	int InitMateShowHeroFetchConfig(TiXmlElement *RootElement);

	int GetMateFetchTime(){return MateFetchTime;}

	int GetMateShowHeroFetchTime(){return MateShowHeroTime;}

private:

	int MateFetchTime;				//保存匹配界面规定的时间
	int MateShowHeroTime;			//保存进入选择英雄界面的时间

};


#endif