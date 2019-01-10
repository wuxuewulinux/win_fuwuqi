
#ifndef _FIGHTSERVERCONFIG_HPP_
#define _FIGHTSERVERCONFIG_HPP_

#include"xmlnodeconfig.hpp"
#include <map>

struct FightServerInfo
{
	int FightServerId;
	int FightServerPort;
	std::string FightServerIp;
	uint32_t UserCount;				//�û��������������ٸ����������Ӹ�ս��������
};


class FightServerConfig
{
public:
	FightServerConfig();
	~FightServerConfig();

	bool Init(std::string configname);   

	int InitFightServerConfig(TiXmlElement *RootElement);

	FightServerInfo* GetFightServerById(int iFightServerId);	//��ս��������ID��ȡ��ս����������Ϣ

	int GetEmptyFightServerID();								//��ȡһ�����е�ս��������ID��ս����������.

private:

	std::map<int,FightServerInfo> FightServerMap;

};


#endif