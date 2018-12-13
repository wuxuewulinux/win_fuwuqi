
#ifndef _MINGCHENGCONFIG_HPP_
#define _MINGCHENGCONFIG_HPP_

#include<map>
#include"xmlnodeconfig.hpp"


struct nameconfig
{
	int grade;                  //称号等级

	int shengchang;             //胜场数

	std::string name;           //获得的游戏称号

};

class MingChengConfig
{
public:
	MingChengConfig();
	~MingChengConfig();

	bool Init(std::string configname);     //这个函数所有配置类都会声明一样的

	int InitMingChengConfig(TiXmlElement *RootElement);    //读取该功能的配置信息

	const nameconfig * GetNameGradeCfg(int grade);         //查找对应等级的称号信息

private:

	std::map<int,nameconfig> name_map;                     //保存所有配置信息在一个map容器中
};

#endif