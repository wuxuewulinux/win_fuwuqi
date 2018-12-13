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
	//��ȡ��Ϸ�ƺ�����
	{
		if (!mingcheng.Init(Path + "mingcheng.xml"))   
		{
			return false;
		}
	}

	//��ȡ�����������ݿ�����
	{
		if (!server_mysql.Init(Path + "server_mysql.xml"))   
		{
			return false;
		}
	}

	//�����������ģ������ÿ�����������չ����

	return true;
}