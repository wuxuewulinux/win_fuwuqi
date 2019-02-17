#ifndef _SERVERCONFIG_HPP_
#define _SERVERCONFIG_HPP_

#include<vector>
#include"xmlnodeconfig.hpp"


struct MysqlServer
{
	std::string ip;             //��ȡ������IP��ַ

	int port;                  //��ȡ�������˿ں�

};

struct RdisServerConfig			//redis���ݿ�������Ϣ��Ip��ַ�Ͷ˿ںţ�
{
	std::string ip;             //��ȡ������IP��ַ

	int port;					//��ȡ�������˿ں�

};

//�����û����ݵ����ݿ��������Ϣ
struct my_mysql
{
	std::string user;                      //ʹ�����ݿ���û�

	std::string mima;                      //��¼���ݿ������

	std::string database;                  //�������ݿ�����

	std::vector<std::string> table;        //�û����ݵ����ݿ�����б����ƶ������ڸ�������

};

//������ˮ���ݿ��������Ϣ
struct BillMysql
{
	std::string user;                      //ʹ�����ݿ���û�

	std::string mima;                      //��¼���ݿ������

	std::string database;                  //�������ݿ�����

	std::vector<std::string> BillTable;        //��ˮ�˵����ݿ�����б����ƶ������ڸ�������

};

class ServerConfig
{
public:
	ServerConfig();
	~ServerConfig();

	bool Init(std::string configname);							//����������������඼������һ����

	int InitMysqlServerConfigg(TiXmlElement *RootElement);		//��ȡmysql������������Ϣ

	int InitRedisServerConfig(TiXmlElement *RootElement);		//��ȡRedis������������Ϣ

	int InitMysqlConfigg(TiXmlElement *RootElement);			//��ȡ�û����ݿ��������Ϣ

	int InitBillMysqlConfigg(TiXmlElement *RootElement);		//��ȡ�û���ˮ���ݿ��������Ϣ
	
	int InitMysqlTableConfigg(TiXmlElement *RootElement);		//��ȡ���ݿ������б��������Ϣ

	int InitBillMysqlTableConfigg(TiXmlElement *RootElement);	//��ȡ�û���ˮ���ݿ������б��������Ϣ

	struct my_mysql * GetMysqlConfig(){return &mysql;}			//��ȡ���ݿ�����������Ϣ�����û���������ȵȵ�����)

	struct BillMysql * GetBillMysqlConfig(){return &mBillMysql;}			//��ȡ�˵���ˮ���ݿ�����������Ϣ�����û���������ȵȵ�����)

	struct MysqlServer * GetMysqlServerConfig(){return &mMysqlServer;}		//��ȡmysql������������Ϣ(IP��ַ�Ͷ˿ںţ�

	struct RdisServerConfig * GetRedisServerConfig(){return &mRdisServer;}	//��ȡredis������������Ϣ


private:

	struct my_mysql mysql;

	struct BillMysql mBillMysql;

	struct MysqlServer mMysqlServer;

	struct RdisServerConfig mRdisServer;
	
};


#endif