#include"logicconfigmanager.hpp"

LogicConfigManager::LogicConfigManager()
{

}

LogicConfigManager::~LogicConfigManager()
{

}

LogicConfigManager *gs_logicconfig_manager = NULL;

LogicConfigManager * LogicConfigManager::Instance()
{
	if (NULL == gs_logicconfig_manager)
	{
		gs_logicconfig_manager = new LogicConfigManager();
	}

	return gs_logicconfig_manager;
}


bool LogicConfigManager::Init(std::string Path)
{
	
	//��ȡ�����������ݿ�����
	{
		if (!rFigServerConfig.Init(Path + "FightServer.xml"))   
		{
			return false;
		}
	}

	//�����������ģ������ÿ�����������չ����

	return true;
}