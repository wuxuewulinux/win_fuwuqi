
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
		MYSQLKU->InsertAccount(rReq.account(),rReq.password(),rReq.name(),rRole);	//插入用户到数据库
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
		MYSQLKU->InsertUid((int)Uid);								//把上线的Uid插入到容器中
	 if(!rDBrole.empty())
	 {//登陆成功就要取出角色数据
		//把role字符串反序列化成DBRoleInfo数据出来
		const char* pTemp = rDBrole.c_str();
		if (!rRoleInfo.ParseFromArray(pTemp, strlen(pTemp)))
		{
			std::cout << "DBRoleInfo Deserialization Fail ！" << std::endl;
			MYLOG.printflog("DBRoleInfo Deserialization Fail ！");     
			return -1;
		}
	
	}
	    rRoleInfo.set_uid(Uid);
	    rRoleInfo.set_name(name);
		pRsp->set_allocated_role(&rRoleInfo);	//结构赋值就使用set_allocated函数来操作
		pRsp->set_type(3);
	}
	if (Type == Login_Repeat)
	{
		pRsp->set_type(4);
	}
	return 0;
}




