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

struct MsgServer
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

	int InitServerConfigg(TiXmlElement *RootElement);    //��ȡ������������Ϣ

	int InitMysqlServerConfigg(TiXmlElement *RootElement);    //��ȡmysql������������Ϣ

	int InitMysqlConfigg(TiXmlElement *RootElement);    //��ȡ���ݿ��������Ϣ
	
	int InitMysqlTableConfigg(TiXmlElement *RootElement);    //��ȡ���ݿ������б��������Ϣ

	int InitMsgServerConfigg(TiXmlElement *RootElement);	//��ȡ�����������IP,�˿�)������Ϣ

	struct my_server * GetServerConfig(){return &server;}    //��ȡ��������������Ϣ��ip��˿ڣ�

	struct my_mysql * GetMysqlConfig(){return &mysql;}    //��ȡ���ݿ�����������Ϣ

	struct MysqlServer * GetMysqlServerConfig(){return &mMysqlServer;}    //��ȡmysql������������Ϣ

	struct MsgServer * GetMsgServerConfig(){return &mMsgServer;}

	


private:

	struct my_server server;

	struct my_mysql mysql;

	struct MysqlServer mMysqlServer;

	struct MsgServer mMsgServer;
	
};


#endif