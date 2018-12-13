
#ifndef _MINGCHENGCONFIG_HPP_
#define _MINGCHENGCONFIG_HPP_

#include<map>
#include"xmlnodeconfig.hpp"


struct nameconfig
{
	int grade;                  //�ƺŵȼ�

	int shengchang;             //ʤ����

	std::string name;           //��õ���Ϸ�ƺ�

};

class MingChengConfig
{
public:
	MingChengConfig();
	~MingChengConfig();

	bool Init(std::string configname);     //����������������඼������һ����

	int InitMingChengConfig(TiXmlElement *RootElement);    //��ȡ�ù��ܵ�������Ϣ

	const nameconfig * GetNameGradeCfg(int grade);         //���Ҷ�Ӧ�ȼ��ĳƺ���Ϣ

private:

	std::map<int,nameconfig> name_map;                     //��������������Ϣ��һ��map������
};

#endif