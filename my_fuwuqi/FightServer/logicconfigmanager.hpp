

/*

������Ϸ�е� ���� ��Ϣ��д�� xml�ļ��У�ʹ��tinyxml����������ж�ȡ xml�е����ýڵ�


*/



#include<string>
#include"serverconfig.hpp"

/*
<?xml version="1.0" encoding="gb2312"?> //ʹ��gb2312�����ʽ����Ȼ��ȡ�����ַ�����������
*/

#ifndef __LOGIC_CONFIG_MANAGER_HPP__
#define __LOGIC_CONFIG_MANAGER_HPP__

#define LOGIC_CONFIG LogicConfigManager::Instance()

//LogicConfigManager����ר�Ź�����������ģ��

class LogicConfigManager
{
public:
	static LogicConfigManager * Instance();

	bool Init(std::string Path);

	ServerConfig & GetFightServerConfig(){return rFigServerConfig;}

private:
	LogicConfigManager();
	~LogicConfigManager();

	ServerConfig rFigServerConfig;       //��������ݿ������

	//�����������ģ�������������������

};

#endif