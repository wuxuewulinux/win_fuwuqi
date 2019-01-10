
#include "TimeEvent.hpp"
#include "quanju.hpp"
#include "enterfunction.hpp"
#include "MateWork.hpp"

void Time_MateFetch(void * Data)
{
	OpenMateSuo();		//申请锁
	//有人不点击同意进入游戏，所以导致条件被触发
	Room* rUserRoom = (Room*)Data;
	if (rUserRoom == NULL)
	{
		CloseMateSuo();		//释放锁
		return;
	}
	if (rUserRoom->UserCount != 5 || rUserRoom->UidList.size() != 5)
	{
		CloseMateSuo();		//释放锁
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
				CloseMateSuo();		//释放锁
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
				CloseMateSuo();		//释放锁
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
	CloseMateSuo();		//释放锁
	return;
}


void Time_MateShowHeroFetch(void * Data)
{
	ShowHeroRoom* rUserRoom = (ShowHeroRoom*)Data;
	if (rUserRoom == NULL)
	{
		return;
	}
	//清除之前匹配成功的房间。
	DeleteRoomMap(rUserRoom->MateRoomIndex);
	DeleteRoomIndex(rUserRoom->MateRoomIndex);
	//获取一张战斗地图
	rUserRoom->DiTuID = GetMapId();
	//遍历装饰背包取出默认设置的ID

	return;
}