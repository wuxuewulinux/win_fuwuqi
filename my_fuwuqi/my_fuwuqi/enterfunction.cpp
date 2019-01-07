
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


int GetUserRoom(uint64_t Uid)
{
	std::map<uint64_t,int>::iterator iter = UserRoom.find(Uid);
	if (UserRoom.end() != iter)
	{
		return iter->second;
	}	

	return 0;
}

void PushUserRoom(uint64_t Uid,int RoomIndex)
{
	
	UserRoom[Uid] = RoomIndex;
	
	return;
}

void DeleteUserRoom(uint64_t Uid)
{

	UserRoom.erase(Uid);
}


Room * GetRoomMap(int RoomIndex)
{
	std::map<int, Room>::iterator iter = RoomMap.find(RoomIndex);
	if (RoomMap.end() != iter)
	{
		return &iter->second;
	}	

	return NULL;
}

void PushRoomMap(int RoomIndex,Room* rRoom)
{
	if (rRoom)
	{
		RoomMap[RoomIndex] = *rRoom;
	}

	return;
}

void DeleteRoomMap(int RoomIndex)
{
	RoomMap.erase(RoomIndex);
}


int AskRoomIndex()
{
	//先查看回收栈是否有房间号,有就返回。
	if (DeleteStack.size() > 0)
	{
		int Value = DeleteStack.top();
		DeleteStack.pop();
		return Value;
	}
	iRoomIndex = iRoomIndex + 1;
	return iRoomIndex;
}


void DeleteRoomIndex(int RoomIndex)
{
	DeleteStack.push(RoomIndex);

	return;
}



int GetShowHeroRoom(uint64_t Uid)
{
	std::map<uint64_t,int>::iterator iter = UserHeroRoom.find(Uid);
	if (UserHeroRoom.end() != iter)
	{
		return iter->second;
	}	

	return 0;
}

void PushShowHeroRoom(uint64_t Uid,int RoomIndex)
{
	UserHeroRoom[Uid] = RoomIndex;
	return;
}

void DeleteShowHeroRoom(uint64_t Uid)
{
	UserHeroRoom.erase(Uid);
}

ShowHeroRoom * GetShowHeroRoomMap(int RoomIndex)
{
	std::map<int, ShowHeroRoom>::iterator iter = HeroRoomMap.find(RoomIndex);
	if (HeroRoomMap.end() != iter)
	{
		return &iter->second;
	}	

	return NULL;
}

void PushShowHeroRoomMap(int RoomIndex,ShowHeroRoom* rRoom)
{
	if (rRoom)
	{
		HeroRoomMap[RoomIndex] = *rRoom;
	}

	return;
}

void DeleteShowHeroRoomMap(int RoomIndex)
{
	HeroRoomMap.erase(RoomIndex);
}

int AskShowHeroRoomIndex()
{
	//先查看回收栈是否有房间号,有就返回。
	if (DeleteHeroRoomStack.size() > 0)
	{
		int Value = DeleteHeroRoomStack.top();
		DeleteHeroRoomStack.pop();
		return Value;
	}
	iHeroRoomIndex = iHeroRoomIndex + 1;
	return iHeroRoomIndex;
}

void DeleteShowHeroRoomIndex(int RoomIndex)
{
	DeleteHeroRoomStack.push(RoomIndex);
	return;
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


int GetTimeIndex(int Room_id,void (*p)(void * canshu),void * data)
{
	return timelei.startgame(Room_id,p,data);
}


bool DeleteTimeIndex(int TimeIndex,int Room_id)
{
	return timelei.endgame(TimeIndex,Room_id);
}