
#include "TimeEvent.hpp"
#include "quanju.hpp"
#include "enterfunction.hpp"
#include "MateWork.hpp"
#include "DecorateBagManager.hpp"
#include "DecorateBagHandler.hpp"
#include <algorithm>
#include "logicconfigmanager.hpp"
#include "MateHandler.hpp"
#include "sendclient.hpp"
#include "UpdateData.hpp"

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
	//获取战斗服务器ID
	rUserRoom->FightServerID = LOGIC_CONFIG->GetFightServerConfig().GetEmptyFightServerID();
	//增加该战斗服务器人数到该战斗服务器中
	LOGIC_CONFIG->GetFightServerConfig().AddFightServerUserCount(rUserRoom->FightServerID,5);
	//遍历装饰背包取出默认设置的ID
	for (int i = 0; i < (int)rUserRoom->HeroList.size(); i++)
	{
		CRoleObj* pRoleObj = GetRole(rUserRoom->HeroList[i].Uid);
		if (pRoleObj == NULL)
		{
			return;
		}
		if (rUserRoom->HeroList[i].ZhaDanId == 0)
		{
			//玩家没有选择炸弹，就要随机给他一个炸弹ID
			for (uint32_t j = 1; j < 6; j++)
			{
				std::vector<uint32_t>::iterator iter = find(rUserRoom->ZhaDanList.begin(),rUserRoom->ZhaDanList.end(),j);
				if (iter == rUserRoom->ZhaDanList.end())
				{
					rUserRoom->HeroList[i].ZhaDanId = j;
				}
			}
		}
		if (rUserRoom->HeroList[i].PlaneId == 0)
		{
			//玩家没有选择飞机，就要去装饰背包取出默认ID
			DecorateBagManager& rBag = pRoleObj->GetDecorateBagMgr();
			rUserRoom->HeroList[i].PlaneId = rBag.CheckDecorateItemTime(ZHANJI_TYPE);
		}
		if (rUserRoom->HeroList[i].DiPanId == 0)
		{
			DecorateBagManager& rBag = pRoleObj->GetDecorateBagMgr();
			rUserRoom->HeroList[i].DiPanId = rBag.CheckDecorateItemTime(DIPAN_TYPE);
		}
		if (rUserRoom->HeroList[i].SkillOneID == 0)
		{
			DecorateBagManager& rBag = pRoleObj->GetDecorateBagMgr();
			rUserRoom->HeroList[i].SkillOneID = rBag.CheckDecorateItemTime(SKILLONE_TYPE);
		}
		if (rUserRoom->HeroList[i].SkillTwoID == 0)
		{
			DecorateBagManager& rBag = pRoleObj->GetDecorateBagMgr();
			rUserRoom->HeroList[i].SkillTwoID = rBag.CheckDecorateItemTime(SKILLTWO_TYPE);
		}

	}
	//处理发送数据给客户端，然后客户端拿到数据进入战斗场景
	for (int i = 0; i < (int)rUserRoom->HeroList.size();i++)
	{
		//移除容器中的选择英雄界面用户Io
		DeleteShowHeroRoom(rUserRoom->HeroList[i].Uid);
		CRoleObj* pRoleObj = GetRole(rUserRoom->HeroList[i].Uid);
		if (pRoleObj == NULL)
		{
			return;
		}
		//向聊天服务器广播玩家好友正在战斗状态
		UpdateData::UpMsgStatus(rUserRoom->HeroList[i].Uid,2,STATUS);
		//拉数据进入战斗场景
		SendHeroEnterFight(pRoleObj,pRoleObj->GetFd(),pRoleObj->GetUid(),rUserRoom);
	}
	
	//删除英雄选择界面定时器
	TimeSend(false,rUserRoom->TimeIndex,0,rUserRoom->RoomIndex);		//先清除定时器时间
	DeleteTimeIndex(rUserRoom->TimeIndex,rUserRoom->RoomIndex);
	//删除一个选择英雄界面房间,不能在这里删除，必须等待该房间战斗结束，战斗服务器返回才能删除房间，不然有可能两个战场的房间号一样
	//DeleteShowHeroRoomMap(rUserRoom->RoomIndex);
	return;
}



int SendHeroEnterFight(CRoleObj* pRoleObj,int SockIo,uint64_t uUid,ShowHeroRoom* pRoom)
{
	HANDCHECH_P(pRoleObj,-1);
	CSMsg oCSMsg;
	CSEnterFightMapRsp* Rsp = static_cast<CSEnterFightMapRsp*>(MateHandler::OnCSMsg(oCSMsg, uUid, CS_MSGID_Mate, CSMateCmd_EnterFightMap));
	HANDCHECH_P(Rsp, -1);

	Rsp->set_mapid(pRoom->DiTuID);
	Rsp->set_fightserverid(pRoom->FightServerID);
	Rsp->set_roomindex(pRoom->RoomIndex);
	CSFightHeroInfoListRsp* pFightHeroInfoListRsp = Rsp->mutable_heroinfolistrsp();
	HANDCHECH_P(pFightHeroInfoListRsp,-2);
	for (int i = 0;i < (int)pRoom->HeroList.size();i++)
	{
		if (uUid != pRoom->HeroList[i].Uid)
		{
			CSFightHeroInfo* pFightHeroInfo = pFightHeroInfoListRsp->add_heroinfolist();
			HANDCHECH_P(pFightHeroInfo,-3);
			pFightHeroInfo->set_uid(pRoom->HeroList[i].Uid);
			pFightHeroInfo->set_name(pRoom->HeroList[i].Name);
			pFightHeroInfo->set_planeid(pRoom->HeroList[i].PlaneId);
			pFightHeroInfo->set_zhadanid(pRoom->HeroList[i].ZhaDanId);
			pFightHeroInfo->set_dipanid(pRoom->HeroList[i].DiPanId);
			pFightHeroInfo->set_skilloneid(pRoom->HeroList[i].SkillOneID);
			pFightHeroInfo->set_skilltwoid(pRoom->HeroList[i].SkillTwoID);
		}
	}
	SendClient(SockIo,&oCSMsg);
	return 0;
}