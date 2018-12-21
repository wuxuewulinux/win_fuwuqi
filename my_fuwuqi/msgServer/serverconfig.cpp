
#include"serverconfig.hpp"

ServerConfig::ServerConfig()
{

}

ServerConfig::~ServerConfig()
{

}

bool ServerConfig::Init(std::string configname)
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

	//��ȡ���ݿ��������Ϣ
	{
		TiXmlElement *mysql_element = RootElement->FirstChildElement("mysql");
		if (NULL == mysql_element)
		{
			std::cout<<": no [mysql_element]."<<std::endl;
			return false;
		}

		iRet = InitMysqlConfigg(mysql_element);
		if (iRet)
		{
			std::cout<<"InitMysqlConfigg failed : "<<std::cout<<iRet<<std::endl;;
			
			return false;
		}
	}
	

	//��ȡ���ݿ����еı�
	{
		TiXmlElement *mysqltable_element = RootElement->FirstChildElement("mysqltable");
		if (NULL == mysqltable_element)
		{
			std::cout<<": no [mysqltable_element]."<<std::endl;
			return false;
		}

		iRet = InitMysqlTableConfigg(mysqltable_element);
		if (iRet)
		{
			std::cout<<"InitMysqlTableConfigg failed : "<<std::cout<<iRet<<std::endl;;
			
			return false;
		}
	}
	
	//��ȡmysql��������IP��ַ�Ͷ˿ں�
	{
		TiXmlElement *server_element = RootElement->FirstChildElement("mysqlserver");
		if (NULL == server_element)
		{
			std::cout<<": no [server]."<<std::endl;
			return false;
		}

		iRet = InitMysqlServerConfigg(server_element);
		if (iRet)
		{
			std::cout<<"InitMysqlServerConfigg failed : "<<std::cout<<iRet<<std::endl;;

			return false;
		}
	}

	//����Ժ��ģ����������������Ϣ�������������չ

	return true;
}




int ServerConfig::InitMysqlServerConfigg(TiXmlElement *RootElement)
{

	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		
		if (!GetSubNodeValue(dataElement, "ip", mMysqlServer.ip) || mMysqlServer.ip.empty())
		{
			return -1;
		}

		if (!GetSubNodeValue(dataElement, "port", mMysqlServer.port) || mMysqlServer.port < 0 )
		{
			return -2;
		}

		dataElement = dataElement->NextSiblingElement();

	}
	return 0;

}



int ServerConfig::InitMysqlConfigg(TiXmlElement *RootElement)
{
		//��ʼ�������ݿ�xml�ļ��ڵ������

	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		if (!GetSubNodeValue(dataElement, "mysql_user", mysql.user) || mysql.user.empty())
		{
			return -1;
		}
		
		if (!GetSubNodeValue(dataElement, "mysql_mima", mysql.mima) || mysql.mima.empty() )
		{
			return -2;
		}
		
		if (!GetSubNodeValue(dataElement, "mysql_database", mysql.database) || mysql.database.empty() )
		{
			return -3;
		}

		dataElement = dataElement->NextSiblingElement();
	
	}
		return 0;

}
	




int ServerConfig::InitMysqlTableConfigg(TiXmlElement *RootElement)
{

	//��ʼ�������ݿ�xml�ļ��ڵ������

	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		std::string tablename;
		if (!GetSubNodeValue(dataElement, "table", tablename) ||tablename.empty())
		{
			return -1;
		}
		mysql.table.push_back(tablename);

		dataElement = dataElement->NextSiblingElement();
	
	}

	return 0;
}