
#include "TimeEvent.hpp"
#include "quanju.hpp"
#include "enterfunction.hpp"
#include "MateWork.hpp"

void Time_MateFetch(void * Data)
{
	//有人不点击同意进入游戏，所以导致条件被触发
	Room* rUserRoom = (Room*)Data;
	if (rUserRoom == NULL)
	{
		return;
	}
	rUserRoom->Agree = 0;			//重新刷新为0，因为这局匹配没有意义了
	std::vector<UidInfo>::iterator iter = rUserRoom->UidList.begin();
	while( iter != rUserRoom->UidList.end())
	{
		if (iter->Button == false)
		{
			//该玩家没有点击同意，把他从房间移除出去
			DeleteUserRoom(iter->Uid);
			CRoleObj* pRoleObj = GetRole(iter->Uid);
			if (pRoleObj == NULL)
			{
				return;
			}
			MateWork::SendMateNotUser(pRoleObj->GetFd(),iter->Uid);
			iter = rUserRoom->UidList.erase(iter);
			rUserRoom->UserCount = rUserRoom->UserCount - 1;
		}
		else
		{
			//广播协议说有玩家没有点击同意
			CRoleObj* pRoleObj = GetRole(iter->Uid);
			if (pRoleObj == NULL)
			{
				return;
			}
			MateWork::SendMateNotSuccess(pRoleObj->GetFd(),iter->Uid);
			iter->Button = false;
			iter++;
		}
	}
	//判断该用户移除时，房间还有没人，没有人就要清除房间
	if (rUserRoom->UserCount == 0 && rUserRoom->UidList.size() == 0)
	{
		DeleteRoomMap(rUserRoom->RoomIndex);
		DeleteRoomIndex(rUserRoom->RoomIndex);
	}
	return;
}


void Time_MateShowHeroFetch(void * Data)
{
	ShowHeroRoom* rUserRoom = (ShowHeroRoom*)Data;

	return;
}