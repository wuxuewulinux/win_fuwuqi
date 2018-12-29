
#include "GameMysqlWork.hpp"
#include "quanju.hpp"

GameMysqlWork::GameMysqlWork()
{

}
GameMysqlWork::~GameMysqlWork()
{

}



int GameMysqlWork::Register(const SSRegisterReq& rReq,SSRegisterRsp* pRsp)
{
	int Type = MYSQLKU->Register(rReq.account(),rReq.name());
	if (Type == Register_Account_Fail)
	{
		pRsp->set_type(1);
	}
	if (Type == Register_Name_Fail)
	{
		pRsp->set_type(2);
	}
	if (Type == Register_Success)
	{
		pRsp->set_type(3);
		std::string rRole = "";
		MYSQLKU->InsertAccount(rReq.account(),rReq.password(),rReq.name(),rRole);	//�����û������ݿ�
		uint64_t Uid = MYSQLKU->GetUid(rReq.account());
		pRsp->set_uid(Uid);
		pRsp->set_name(MYSQLKU->GetName(Uid));
	}
	return 0;
}

int GameMysqlWork::Login(const SSLoginReq& rReq,SSLoginRsp* pRsp)
{
	int Type = MYSQLKU->Login(rReq.account(),rReq.password());

	if (Type == Login_Account_Fail)
	{
		pRsp->set_type(1);
	}
	if (Type == Login_Password_Fail)
	{
		pRsp->set_type(2);
	}
	if (Type == Login_Success)
	{
		DBRoleInfo rRoleInfo;
		uint64_t Uid = MYSQLKU->GetUid(rReq.account());
		std::string name = MYSQLKU->GetName(Uid);
		std::string rDBrole = MYSQLKU->GetDBRole(Uid);
		MYSQLKU->InsertUid((int)Uid);								//�����ߵ�Uid���뵽������
	 if(!rDBrole.empty())
	 {//��½�ɹ���Ҫȡ����ɫ����
		//��role�ַ��������л���DBRoleInfo���ݳ���
		const char* pTemp = rDBrole.c_str();
		if (!rRoleInfo.ParseFromArray(pTemp, strlen(pTemp)))
		{
			std::cout << "DBRoleInfo Deserialization Fail ��" << std::endl;
			MYLOG.printflog("DBRoleInfo Deserialization Fail ��");     
			return -1;
		}
	
	}
	    rRoleInfo.set_uid(Uid);
	    rRoleInfo.set_name(name);
		pRsp->set_allocated_role(&rRoleInfo);	//�ṹ��ֵ��ʹ��set_allocated����������
		pRsp->set_type(3);
	}
	if (Type == Login_Repeat)
	{
		pRsp->set_type(4);
	}
	return 0;
}




