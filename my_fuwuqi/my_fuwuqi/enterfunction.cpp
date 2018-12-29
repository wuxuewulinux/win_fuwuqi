
#include "enterfunction.hpp"
#include "lognamespace.hpp"

CRoleObj * GetRole(uint64_t Uid)
{
	std::map<int, CRoleObj>::iterator iter = mRoleList.find(Uid);
	if (mRoleList.end() != iter)
	{
		return &iter->second;
	}	

	return  NULL;
}

void PushRole(uint64_t Uid,CRoleObj* rRole)
{
	if (rRole)
	{
		mRoleList[Uid] = *rRole;
	}

	return;
}

void DeleteRole(uint64_t Uid)
{
	mRoleList.erase(Uid);
}



bool LogInit(std::string path)    //启动所有日志文件
{
	//启动麻将模块的日志记录
	{
		if (!logquanju::majiangLog.Init(path + "log/majiangLog.txt"))
		{
			return false;
		}
	}
	//以后还有别的模块日志可以在这里添加

	return true;

}