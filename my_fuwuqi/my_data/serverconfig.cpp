
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

	//��ȡ�û����ݿ��������Ϣ
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

	//��ȡ�û���ˮ�˵����ݿ��������Ϣ
	{
		TiXmlElement *bill_mysql_element = RootElement->FirstChildElement("BillMysql");
		if (NULL == bill_mysql_element)
		{
			std::cout<<": no [bill_mysql_element]."<<std::endl;
			return false;
		}

		iRet = InitBillMysqlConfigg(bill_mysql_element);
		if (iRet)
		{
			std::cout<<"InitBillMysqlConfigg failed : "<<std::cout<<iRet<<std::endl;;

			return false;
		}
	}

	//��ȡ�û���ˮ�˵����ݿ����еı�
	{
		TiXmlElement *billmysqltable_element = RootElement->FirstChildElement("bill_mysql_table");
		if (NULL == billmysqltable_element)
		{
			std::cout<<": no [billmysqltable_element]."<<std::endl;
			return false;
		}

		iRet = InitBillMysqlTableConfigg(billmysqltable_element);
		if (iRet)
		{
			std::cout<<"InitBillMysqlTableConfigg failed : "<<std::cout<<iRet<<std::endl;;

			return false;
		}
	}

	//��ȡRedis��������IP��ַ�Ͷ˿ں�
	{
		TiXmlElement *redis_element = RootElement->FirstChildElement("RedisServer");
		if (NULL == redis_element)
		{
			std::cout<<": no [RedisServer]."<<std::endl;
			return false;
		}

		iRet = InitRedisServerConfig(redis_element);
		if (iRet)
		{
			std::cout<<"InitRedisServerConfig failed : "<<std::cout<<iRet<<std::endl;;

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



int ServerConfig::InitRedisServerConfig(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{

		if (!GetSubNodeValue(dataElement, "ip", mRdisServer.ip) || mRdisServer.ip.empty())
		{
			return -1;
		}

		if (!GetSubNodeValue(dataElement, "port", mRdisServer.port) || mRdisServer.port < 0 )
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
	

int ServerConfig::InitBillMysqlConfigg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		if (!GetSubNodeValue(dataElement, "bill_mysql_user", mBillMysql.user) || mBillMysql.user.empty())
		{
			return -1;
		}

		if (!GetSubNodeValue(dataElement, "bill_mysql_mima", mBillMysql.mima) || mBillMysql.mima.empty() )
		{
			return -2;
		}

		if (!GetSubNodeValue(dataElement, "bill_mysql_database", mBillMysql.database) || mBillMysql.database.empty() )
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


int ServerConfig::InitBillMysqlTableConfigg(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		std::string DiamondBillTable;
		if (!GetSubNodeValue(dataElement, "DiamondBill_table", DiamondBillTable) ||DiamondBillTable.empty())
		{
			return -1;
		}
		mBillMysql.BillTable.push_back(DiamondBillTable);
		std::string GoldBillTable;
		if (!GetSubNodeValue(dataElement, "GoldBill_table", GoldBillTable) ||GoldBillTable.empty())
		{
			return -2;
		}
		mBillMysql.BillTable.push_back(GoldBillTable);
		dataElement = dataElement->NextSiblingElement();

	}

	return 0;
}