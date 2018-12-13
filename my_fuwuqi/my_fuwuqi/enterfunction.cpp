
#include "enterfunction.hpp"

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