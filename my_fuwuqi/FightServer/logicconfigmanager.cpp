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
	
	//读取服务器与数据库配置
	{
		if (!rFigServerConfig.Init(Path + "FightServer.xml"))   
		{
			return false;
		}
	}

	//如果还有其他模块的配置可以在这里扩展代码

	return true;
}