

/*

������Ϸ�е� ���� ��Ϣ��д�� xml�ļ��У�ʹ��tinyxml����������ж�ȡ xml�е����ýڵ�


*/



#include<string>
#include"mingchengconfig.hpp"
#include"serverconfig.hpp"
#include "TimeConfig.hpp"
#include "MapConfig.hpp"
#include "fightServerConfig.hpp"

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

	MingChengConfig & GetMingChengConfig(){return mingcheng;} 

	ServerConfig & GetServerMysqlConfig(){return server_mysql;}

	TimeConfig & GetTimeConfig(){return m_oTimeConfig;}

	MapConfig &	GetMapConfig(){return m_oMapConfig;}

	FightServerConfig & GetFightServerConfig(){return m_oFightServerConfig;}

private:
	LogicConfigManager();
	~LogicConfigManager();

	ServerConfig server_mysql;       //��������ݿ������

	MingChengConfig mingcheng;		//��Ϸ�ƺŵ�����

	TimeConfig m_oTimeConfig;		//������Ϸ����ʱ��(ƥ��ʱ�䣬ѡ��Ӣ�۽���ʱ��)

	MapConfig m_oMapConfig;			//��ͼ����ģ��

	FightServerConfig m_oFightServerConfig;  //����ս������������

	//�����������ģ�������������������

};

#endif