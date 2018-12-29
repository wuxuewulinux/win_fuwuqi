
#include "enterfunction.hpp"
#include "lognamespace.hpp"

int GetRoleIo(uint64_t Uid)
{
	std::map<uint64_t, int>::iterator iter = rUidMap.find(Uid);
	if (rUidMap.end() != iter)
	{
		return iter->second;
	}	

	return  0;
}

void PushRoleIo(uint64_t Uid,int SockIo)
{
	rUidMap[Uid] = SockIo;
	return;
}

void DeleteRoleIo(uint64_t Uid)
{
	rUidMap.erase(Uid);
}


