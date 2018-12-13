
#include"mingchengconfig.hpp"

MingChengConfig::MingChengConfig()
{

}

MingChengConfig::~MingChengConfig()
{

}

bool MingChengConfig::Init(std::string configname)
{
	int iRet;
	TiXmlDocument document;

	if (configname == "" || !document.LoadFile(configname.c_str()))
	{
		std::cout<<"MingChengConfig Load FootPrint Error!"<<std::endl;
		return false;
	}

	TiXmlElement *RootElement = document.RootElement();
	if (NULL == RootElement)
	{
		std::cout<<"xml root node error."<<std::endl;
		return false;
	}

	//读取称号配置信息
	{
		TiXmlElement *upgrade_element = RootElement->FirstChildElement("name");
		if (NULL == upgrade_element)
		{
			std::cout<<": no [upgrade]."<<std::endl;
			return false;
		}

		iRet = InitMingChengConfig(upgrade_element);
		if (iRet)
		{
			std::cout<<"InitMingChengConfig failed : "<<std::cout<<iRet<<std::endl;;
			
			return false;
		}
	}
	
	//如果以后该模块增加其他配置信息就在这里进行扩展

	return true;
}





int MingChengConfig::InitMingChengConfig(TiXmlElement *RootElement)
{
	//开始读取xml文件节点的数据

	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		short grade = 0;
		if (!GetSubNodeValue(dataElement, "grade", grade) || grade < 0 )
		{
			return -1;
		}
		nameconfig cfg;
		cfg.grade = grade;
		if (!GetSubNodeValue(dataElement, "shengchang", cfg.shengchang) || cfg.shengchang < 0 )
		{
			return -2;
		}
		if (!GetSubNodeValue(dataElement, "mingcheng", cfg.name) || cfg.name.empty() )
		{
			return -3;
		}
		name_map[cfg.grade] = cfg;
		dataElement = dataElement->NextSiblingElement();
	
	}

	return 0;
}


const nameconfig * MingChengConfig::GetNameGradeCfg(int grade)
{
	std::map<int, nameconfig>::const_iterator iter = name_map.find(grade);
	if (name_map.end() != iter)
	{
		return &iter->second;
	}	
	return NULL;
}