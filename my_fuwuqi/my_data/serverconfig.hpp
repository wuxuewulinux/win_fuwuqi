#ifndef _SERVERCONFIG_HPP_
#define _SERVERCONFIG_HPP_

#include<vector>
#include"xmlnodeconfig.hpp"

struct my_server
{
	std::string ip;             //��ȡ������IP��ַ

	int port;                  //��ȡ�������˿ں�

};


struct MysqlServer
{
	std::string ip;             //��ȡ������IP��ַ

	int port;                  //��ȡ�������˿ں�

};

struct my_mysql
{
	std::string user;                      //ʹ�����ݿ���û�

	std::string mima;                      //��¼���ݿ������

	std::string database;                  //�������ݿ�����

	std::vector<std::string> table;        //���ݿ�����б����ƶ������ڸ�������

};

class ServerConfig
{
public:
	ServerConfig();
	~ServerConfig();

	bool Init(std::string configname);     //����������������඼������һ����

	int InitMysqlServerConfigg(TiXmlElement *RootElement);    //��ȡmysql������������Ϣ

	int InitMysqlConfigg(TiXmlElement *RootElement);    //��ȡ���ݿ��������Ϣ
	
	int InitMysqlTableConfigg(TiXmlElement *RootElement);    //��ȡ���ݿ������б��������Ϣ

	struct my_mysql * GetMysqlConfig(){return &mysql;}    //��ȡ���ݿ�����������Ϣ�����û���������ȵȵ�����)

	struct MysqlServer * GetMysqlServerConfig(){return &mMysqlServer;}    //��ȡmysql������������Ϣ(IP��ַ�Ͷ˿ںţ�

	


private:

	struct my_mysql mysql;

	struct MysqlServer mMysqlServer;
	
};


#endif