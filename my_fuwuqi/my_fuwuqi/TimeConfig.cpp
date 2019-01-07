
#include "TimeConfig.hpp"

TimeConfig::TimeConfig()
{

}

TimeConfig::~TimeConfig()
{

}


int TimeConfig::InitMateFetchConfig(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		if (!GetSubNodeValue(dataElement, "MateFetchTime", MateFetchTime) || MateFetchTime < 0 )
		{
			return -1;
		}

		dataElement = dataElement->NextSiblingElement();
	}
	return 0;
}


int TimeConfig::InitMateShowHeroFetchConfig(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		if (!GetSubNodeValue(dataElement, "MateShowHeroFetch", MateShowHeroTime) || MateShowHeroTime < 0 )
		{
			return -1;
		}

		dataElement = dataElement->NextSiblingElement();
	}
	return 0;
}


bool TimeConfig::Init(std::string configname)
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

	//读取规定匹配界面时间触发
	{
		TiXmlElement *element = RootElement->FirstChildElement("MateFetch");
		if (NULL == element)
		{
			std::cout<<": no [server]."<<std::endl;
			return false;
		}

		iRet = InitMateFetchConfig(element);
		if (iRet)
		{
			std::cout<<"InitMateFetchConfig failed : "<<std::cout<<iRet<<std::endl;;
			return false;
		}
	}
	//读取规定选择英雄界面时间触发
	{
		TiXmlElement *element = RootElement->FirstChildElement("MateShowHeroFetch");
		if (NULL == element)
		{
			std::cout<<": no [server]."<<std::endl;
			return false;
		}

		iRet = InitMateShowHeroFetchConfig(element);
		if (iRet)
		{
			std::cout<<"InitMateShowHeroFetchConfig failed : "<<std::cout<<iRet<<std::endl;;
			return false;
		}
	}


	return true;
}