
#include "TimeEvent.hpp"
#include "quanju.hpp"
#include "enterfunction.hpp"
#include "MateWork.hpp"

void Time_MateFetch(void * Data)
{
	//���˲����ͬ�������Ϸ�����Ե�������������
	Room* rUserRoom = (Room*)Data;
	if (rUserRoom == NULL)
	{
		return;
	}
	rUserRoom->Agree = 0;			//����ˢ��Ϊ0����Ϊ���ƥ��û��������
	std::vector<UidInfo>::iterator iter = rUserRoom->UidList.begin();
	while( iter != rUserRoom->UidList.end())
	{
		if (iter->Button == false)
		{
			//�����û�е��ͬ�⣬�����ӷ����Ƴ���ȥ
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
			//�㲥Э��˵�����û�е��ͬ��
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
	//�жϸ��û��Ƴ�ʱ�����仹��û�ˣ�û���˾�Ҫ�������
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