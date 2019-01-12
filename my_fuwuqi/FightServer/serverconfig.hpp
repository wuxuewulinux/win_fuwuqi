#ifndef _SERVERCONFIG_HPP_
#define _SERVERCONFIG_HPP_

#include<vector>
#include"xmlnodeconfig.hpp"


struct FigthServer				//ս������������
{
	std::string ip;             //��ȡ������IP��ַ

	int port;					//��ȡ�������˿ں�

};


struct GameServer				//��Ϸ����������
{
	std::string ip;             //��ȡ������IP��ַ

	int port;					//��ȡ�������˿ں�
};

class ServerConfig
{
public:
	ServerConfig();
	~ServerConfig();

	bool Init(std::string configname);										//����������������඼������һ����

	int InitFightServerConfig(TiXmlElement *RootElement);					//��ȡս��������������Ϣ

	int InitGameServerConfig(TiXmlElement *RootElement);					//��ȡ��Ϸ������������Ϣ

	struct FigthServer * GetFightServerConfig(){return &m_FigthServer;}		//��ȡս��������������Ϣ(IP��ַ�Ͷ˿ںţ�

	struct GameServer * GetGameServerConfig(){return &m_GameServer;}		//��ȡ��Ϸ������(IP��ַ�Ͷ˿ںţ�

private:

	struct FigthServer m_FigthServer;
	struct GameServer m_GameServer;
};


#endif