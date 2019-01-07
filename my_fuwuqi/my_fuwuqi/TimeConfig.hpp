
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

	int MateFetchTime;				//����ƥ�����涨��ʱ��
	int MateShowHeroTime;			//�������ѡ��Ӣ�۽����ʱ��

};


#endif