
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

	//读取战斗服务器的IP地址和端口号
	{
		TiXmlElement *server_element = RootElement->FirstChildElement("FightServer");
		if (NULL == server_element)
		{
			std::cout<<": no [server]."<<std::endl;
			return false;
		}

		iRet = InitFightServerConfig(server_element);
		if (iRet)
		{
			std::cout<<"InitFightServerConfig failed : "<<std::cout<<iRet<<std::endl;;

			return false;
		}
	}

	//读取游戏服务器的IP地址和端口号
	{
		TiXmlElement *server_element = RootElement->FirstChildElement("GameServer");
		if (NULL == server_element)
		{
			std::cout<<": no [server]."<<std::endl;
			return false;
		}

		iRet = InitGameServerConfig(server_element);
		if (iRet)
		{
			std::cout<<"InitGameServerConfig failed : "<<std::cout<<iRet<<std::endl;;

			return false;
		}
	}

	//如果以后该模块增加其他配置信息就在这里进行扩展

	return true;
}




int ServerConfig::InitFightServerConfig(TiXmlElement *RootElement)
{

	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{
		
		if (!GetSubNodeValue(dataElement, "ip", m_FigthServer.ip) || m_FigthServer.ip.empty())
		{
			return -1;
		}

		if (!GetSubNodeValue(dataElement, "port", m_FigthServer.port) || m_FigthServer.port < 0 )
		{
			return -2;
		}

		dataElement = dataElement->NextSiblingElement();

	}
	return 0;

}


int ServerConfig::InitGameServerConfig(TiXmlElement *RootElement)
{
	TiXmlElement *dataElement = RootElement->FirstChildElement("data");
	while (NULL != dataElement)
	{

		if (!GetSubNodeValue(dataElement, "ip", m_GameServer.ip) || m_GameServer.ip.empty())
		{
			return -1;
		}

		if (!GetSubNodeValue(dataElement, "port", m_GameServer.port) || m_GameServer.port < 0 )
		{
			return -2;
		}

		dataElement = dataElement->NextSiblingElement();

	}

	return 0;
}







