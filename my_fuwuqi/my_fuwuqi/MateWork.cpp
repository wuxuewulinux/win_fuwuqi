
#include "MateWork.hpp"
#include "quanju.hpp"
#include "enterfunction.hpp"
#include "sendclient.hpp"
#include "MateHandler.hpp"
#include "TimeEvent.hpp"
#include "DecorateBagHandler.hpp"
#include "logicconfigmanager.hpp"
#include <algorithm>

MateWork::MateWork()
{

}

MateWork::~MateWork()
{

}



int MateWork::SendMateNotUser(int SockIo,uint64_t Uid)
{
	CSMsg oCSMsg;
	MateHandler::OnCSMsg(oCSMsg, Uid, CS_MSGID_Mate, CSMateCmd_MateNotUser);
	SendClient(SockIo,&oCSMsg);

	return 0;
}


int MateWork::SendMateNotSuccess(int SockIo,uint64_t Uid)
{
	CSMsg oCSMsg;
	MateHandler::OnCSMsg(oCSMsg, Uid, CS_MSGID_Mate, CSMateCmd_MateNotSuccess);
	SendClient(SockIo,&oCSMsg);

	return 0;
}


int MateWork::SendMateFetch(int SockIo,uint64_t Uid)
{
	CSMsg oCSMsg;
	MateHandler::OnCSMsg(oCSMsg, Uid, CS_MSGID_Mate, CSMateCmd_MateFetch);
	SendClient(SockIo,&oCSMsg);
	return 0;
}


int MateWork::MateFetch(const CSMateFetchReq& rReq)
{
	OpenMateSuo();		//申请锁
	//给玩家匹配房间，先查看是否有空闲的房间
	bool Falge = false;
	if (RoomMap.size() > 0)
	{
		//有房间，遍历所有房间找出空闲的位置
		for (std::map<int,Room>::iterator iter = RoomMap.begin(); iter != RoomMap.end();iter++)
		{
			if (iter->second.MaxCount > iter->second.UserCount)
			{
				//该房间有位置，把该玩家加入该房间中
				++iter->second.UserCount;
				UidInfo rInfo;
				rInfo.Uid = rReq.uid();
				rInfo.Button = false;
				iter->second.UidList.push_back(rInfo);
				PushUserRoom(rReq.uid(),iter->first);
				Falge = true;
				if (iter->second.MaxCount == iter->second.UserCount)
				{
					//房间满人了，触发游戏开始发送给该房间所有玩家
					for (int i = 0; i < (int)iter->second.UidList.size();i++)
					{
						CRoleObj* pRoleObj = GetRole(iter->second.UidList[i].Uid);
						if (pRoleObj == NULL)
						{
							CloseMateSuo();
							return -3;
						}
						SendMateFetch(pRoleObj->GetFd(),iter->second.UidList[i].Uid);
						//压入定时器 10 秒，给你十秒时间去点击同意，如果时间到不点击就触发事件
						//触发事件执行把不点击同意的玩家移除房间。

						iter->second.TimeIndex = GetTimeIndex(iter->first,Time_MateFetch,&iter->second); //申请定时器
						int TimeValue = LOGIC_CONFIG->GetTimeConfig().GetMateFetchTime();
						TimeSend(true,iter->second.TimeIndex,TimeValue,iter->first);		//定时10秒
					}
				}
				break;
			}
		}
		if (Falge == false)	//有可能全部房间都满人。所以要给他创建房间
		{
			int RoomIndex = AskRoomIndex();
			Room rRoom;
			rRoom.RoomIndex = RoomIndex;
			rRoom.UserCount = 0;
			rRoom.Agree = 0;
			rRoom.MaxCount = 5;					//五个人才开始游戏
			++rRoom.UserCount;
			UidInfo rInfo;
			rInfo.Uid = rReq.uid();
			rInfo.Button = false;
			rRoom.UidList.push_back(rInfo);
			PushRoomMap(RoomIndex,&rRoom);
			PushUserRoom(rReq.uid(),RoomIndex);
		}
	}
	else
	{
		//没有任何等待的房间就要创建新的房间给该玩家
		int RoomIndex = AskRoomIndex();
		Room rRoom;
		rRoom.TimeIndex = 0;
		rRoom.UserCount = 0;
		rRoom.Agree = 0;
		rRoom.MaxCount = 5;					//五个人才开始游戏
		++rRoom.UserCount;
		UidInfo rInfo;
		rInfo.Uid = rReq.uid();
		rInfo.Button = false;
		rRoom.UidList.push_back(rInfo);
		PushRoomMap(RoomIndex,&rRoom);
		PushUserRoom(rReq.uid(),RoomIndex);
	}
	CloseMateSuo();			//释放锁
	return 0;
}



int MateWork::QuitMateFetch(const CSQuitMateFetchReq& rReq)
{
	//找到该玩家的房间号，然后移除出去
	int RoomIndex = GetUserRoom(rReq.uid());
	if (RoomIndex == 0)
	{
		return -1;
	}
	Room* pRoom = GetRoomMap(RoomIndex);
	if (pRoom->UserCount == 5 && pRoom->UidList.size() == 5)
	{
		return -5;
	}
	DeleteUserRoom(rReq.uid());
	HANDCHECH_P(pRoom,-2);
	--pRoom->UserCount;
	for (std::vector<UidInfo>::iterator iter = pRoom->UidList.begin(); iter != pRoom->UidList.end(); iter++)
	{
		if (iter->Uid == rReq.uid())
		{
			pRoom->UidList.erase(iter);
			break;
		}
	}
	//判断该用户移除时，房间还有没人，没有人就要清除房间
	if (pRoom->UserCount == 0 && pRoom->UidList.size() == 0)
	{
		DeleteRoomMap(RoomIndex);
		DeleteRoomIndex(RoomIndex);
	}
	return 0;
}


int MateWork::NotButtonMateFetch(const CSNotButtonMateFetchReq& rReq)
{
	OpenMateSuo();		//申请锁
	//找到该玩家的房间号，然后移除出去
	int RoomIndex = GetUserRoom(rReq.uid());
	if (RoomIndex == 0)
	{
		CloseMateSuo();		//释放锁
		return -1;
	}
	Room* pRoom = GetRoomMap(RoomIndex);
	if (pRoom == NULL)
	{
		CloseMateSuo();		//释放锁
		return -2;
	}
	pRoom->Agree = 0;
	//防止同一个房间多个人同时点击拒绝。
	if (pRoom->UserCount != 5 || pRoom->UidList.size() != 5)
	{
		CloseMateSuo();		//释放锁
		return 0;
	}
	//发送关掉定时时间
	TimeSend(false,pRoom->TimeIndex,0,pRoom->RoomIndex);
	--pRoom->UserCount;
	std::vector<UidInfo>::iterator iter = pRoom->UidList.begin();
	while( iter != pRoom->UidList.end())
	{
		if (iter->Uid == rReq.uid())
		{
			//该玩家没有点击同意，把他从房间移除出去
			DeleteUserRoom(iter->Uid);
			iter = pRoom->UidList.erase(iter);
			pRoom->UserCount = pRoom->UserCount - 1;
		}
		else
		{
			//广播协议说有玩家没有点击同意
			CRoleObj* pRoleObj = GetRole(iter->Uid);
			if (pRoleObj == NULL)
			{
				CloseMateSuo();		//释放锁
				return -1;
			}
			MateWork::SendMateNotSuccess(pRoleObj->GetFd(),iter->Uid);
			iter->Button = false;
			iter++;
		}
	}
	//判断该用户移除时，房间还有没人，没有人就要清除房间
	if (pRoom->UserCount == 0 && pRoom->UidList.size() == 0)
	{
		DeleteRoomMap(RoomIndex);
		DeleteRoomIndex(RoomIndex);
	}
	CloseMateSuo();		//释放锁
	return 0;
}



int MateWork::SuccessButtonMateFetch(const CSSuccessButtonMateFetchReq& rReq)
{
	//找到该玩家的玩家，标记他点击同意
	int RoomIndex = GetUserRoom(rReq.uid());
	if (RoomIndex == 0)
	{
		return -1;
	}
	Room* pRoom = GetRoomMap(RoomIndex);
	HANDCHECH_P(pRoom,-2);
	for (std::vector<UidInfo>::iterator iter = pRoom->UidList.begin(); iter != pRoom->UidList.end(); iter++)
	{
		if (iter->Uid == rReq.uid())
		{
			iter->Button = true;
			++pRoom->Agree;
			if (pRoom->Agree == pRoom->MaxCount && pRoom->UserCount == pRoom->MaxCount)
			{
				//清除该房间的定时器触发
				TimeSend(false,pRoom->TimeIndex,0,pRoom->RoomIndex);		//先清除定时器时间
				DeleteTimeIndex(pRoom->TimeIndex,pRoom->RoomIndex);
				//该房间所有人都同意开始游戏，进入选英雄界面
				EnterHeroShow(pRoom,pRoom->RoomIndex);
			}
		}
	}
	return 0;
}



int MateWork::SendHeroInfo(CRoleObj* pRoleObj,int SockIo,uint64_t uUid,Room* pRoom)
{
	HANDCHECH_P(pRoleObj,-1);
	CSMsg oCSMsg;
	CSEnterHeroShowRsp* Rsp = static_cast<CSEnterHeroShowRsp*>(MateHandler::OnCSMsg(oCSMsg, uUid, CS_MSGID_Mate, CSMateCmd_EnterHeroShow));
	HANDCHECH_P(Rsp, -1);
	CSShowHeroInfoList * pInfoList = Rsp->mutable_heroshow();
	HANDCHECH_P(pInfoList, -2);
	//取出该玩家的Vip信息
	CSShowHeroVipInfo* pUserVip = Rsp->mutable_showherovipinfo();
	HANDCHECH_P(pUserVip,-1);
	DecorateBagManager& UserBag =  pRoleObj->GetDecorateBagMgr();
	uint32_t UserVIPid = UserBag.CheckDecorateItemTime(1);
	pUserVip->set_vipid(UserVIPid);
	if (UserVIPid > 1)
	{
		TDBDecorateBagVIPItem* pUserBagVIPItem = UserBag.GetVIPTypeInfo(UserVIPid);
		HANDCHECH_P(pUserBagVIPItem,-5);
		pUserVip->set_vipgrade(pUserBagVIPItem->m_uiGrade);
	}
	//开始给该玩家各个背包协议赋值
	CSFeiJiBagList * pFeiJiBagList = Rsp->mutable_feijibaglist();
	HANDCHECH_P(pFeiJiBagList,-1);
	CSZhaDanBagList * pZhaDanBagList = Rsp->mutable_zhadanbaglist();
	HANDCHECH_P(pZhaDanBagList,-1);
	CSFeiJiDiPanBagList * pFeiJiDiPanBagList = Rsp->mutable_feijidipanbaglist();
	HANDCHECH_P(pFeiJiDiPanBagList,-1);
	CSFeiJiSkillOneBagList * pFeiJiSkillOneBagList = Rsp->mutable_feijiskillonebaglist();
	HANDCHECH_P(pFeiJiSkillOneBagList,-1);
	CSFeiJiSkillTwoBagList * pFeiJiSkillTwoBagList = Rsp->mutable_feijiskilltwobaglist();
	HANDCHECH_P(pFeiJiSkillTwoBagList,-1);
	for (int i = ZHADAN_TYPE; i < MAX_TYPE; i++)
	{
		if (i == ZHADAN_TYPE)
		{
			UserBag.CheckDecorateItemTime(ZHADAN_TYPE);//刷新时间
			TDBDecorateItemList * pItemList = UserBag.GetDecorateGridList(ZHADAN_TYPE);
			HANDCHECH_P(pItemList,-1);
			for (int j = 0; j < pItemList->m_nGridsRef;j++)
			{
				pZhaDanBagList->add_zhadan(pItemList->m_astDecorateGrids[j].m_uiID);
			}
		}
		else if (i == DIPAN_TYPE)
		{
			UserBag.CheckDecorateItemTime(DIPAN_TYPE);//刷新时间
			TDBDecorateItemList * pItemList = UserBag.GetDecorateGridList(DIPAN_TYPE);
			HANDCHECH_P(pItemList,-1);
			for (int j = 0; j < pItemList->m_nGridsRef;j++)
			{
				pFeiJiDiPanBagList->add_feijidipan(pItemList->m_astDecorateGrids[j].m_uiID);
			}
		}
		else if (i == ZHANJI_TYPE)
		{
			UserBag.CheckDecorateItemTime(ZHANJI_TYPE);//刷新时间
			TDBDecorateItemList * pItemList = UserBag.GetDecorateGridList(ZHANJI_TYPE);
			HANDCHECH_P(pItemList,-1);
			for (int j = 0; j < pItemList->m_nGridsRef;j++)
			{
				pFeiJiBagList->add_feiji(pItemList->m_astDecorateGrids[j].m_uiID);
			}
		}
		else if (i == SKILLONE_TYPE)
		{
			UserBag.CheckDecorateItemTime(SKILLONE_TYPE);//刷新时间
			TDBDecorateItemList * pItemList = UserBag.GetDecorateGridList(SKILLONE_TYPE);
			HANDCHECH_P(pItemList,-1);
			for (int j = 0; j < pItemList->m_nGridsRef;j++)
			{
				pFeiJiSkillOneBagList->add_feijiskillone(pItemList->m_astDecorateGrids[j].m_uiID);
			}
		}
		else if (i == SKILLTWO_TYPE)
		{
			UserBag.CheckDecorateItemTime(SKILLTWO_TYPE);//刷新时间
			TDBDecorateItemList * pItemList = UserBag.GetDecorateGridList(SKILLTWO_TYPE);
			HANDCHECH_P(pItemList,-1);
			for (int j = 0; j < pItemList->m_nGridsRef;j++)
			{
				pFeiJiSkillTwoBagList->add_feijiskilltwo(pItemList->m_astDecorateGrids[j].m_uiID);
			}
		}
	}
	//取所有玩家的用户信息数据，不包含自己在里面
	for (std::vector<UidInfo>::iterator iter = pRoom->UidList.begin(); iter != pRoom->UidList.end(); iter++)
	{
		if (iter->Uid != uUid)
		{
			CSShowHeroInfo * pHeroInfo = pInfoList->add_heroinfolist();
			HANDCHECH_P(pHeroInfo,-3);
			CRoleObj* pRole = GetRole(iter->Uid);
			HANDCHECH_P(pRole,-1);
			pHeroInfo->set_uid(iter->Uid);
			DecorateBagManager& rBag =  pRole->GetDecorateBagMgr();
			uint32_t VIPid = rBag.CheckDecorateItemTime(1);
			pHeroInfo->set_vipid(VIPid);
			pHeroInfo->set_name(pRole->GetName());
			if (VIPid > 1)
			{
				TDBDecorateBagVIPItem* pBagVIPItem = rBag.GetVIPTypeInfo(VIPid);
				HANDCHECH_P(pBagVIPItem,-5);
				pHeroInfo->set_vipgrade(pBagVIPItem->m_uiGrade);
			}
			pHeroInfo->set_rolegrade(pRole->GetLevel());
			pHeroInfo->set_rolerank(pRole->GetRand());
			uint32_t HeadId = rBag.CheckDecorateItemTime(2);
			pHeroInfo->set_roleheadtype(HeadId);
		}
	}
	SendClient(SockIo,&oCSMsg);
	return 0;
}


int MateWork::EnterHeroShow(Room* PRoom,int iDeleteRoomIndex)
{
	HANDCHECH_P(PRoom,-1);
	//创建一个游戏选英雄界面房间
	int HeroRoomIndex = AskShowHeroRoomIndex();
	ShowHeroRoom rHeroRoom;
	rHeroRoom.RoomIndex = HeroRoomIndex;
	rHeroRoom.MateRoomIndex = iDeleteRoomIndex;
	PushShowHeroRoomMap(HeroRoomIndex,&rHeroRoom);
	ShowHeroRoom* pHeroRoom = GetShowHeroRoomMap(HeroRoomIndex);
	HANDCHECH_P(pHeroRoom,-1);
	for (std::vector<UidInfo>::iterator iter = PRoom->UidList.begin(); iter != PRoom->UidList.end(); iter++)
	{
		DeleteUserRoom(iter->Uid);					//进入界面就要移除匹配
		PushShowHeroRoom(iter->Uid,HeroRoomIndex);	
		CRoleObj* pRole = GetRole(iter->Uid);
		HANDCHECH_P(pRole,-1);
		//取出玩家基本数据压入到房间中，因为进入战斗时需要玩家的基本数据
		HeroInfo rHeroInfo;
		memset(&rHeroInfo,0,sizeof(rHeroInfo));
		rHeroInfo.Name = pRole->GetName();
		rHeroInfo.Level = pRole->GetLevel();
		rHeroInfo.LevelExper = pRole->GetLevelExper();
		rHeroInfo.Rank = pRole->GetRand();
		rHeroInfo.RankExper = pRole->GetRandExper();
		rHeroInfo.Uid = iter->Uid;
		DecorateBagManager& rBag = pRole->GetDecorateBagMgr();
		rBag.CheckDecorateItemTime(1); //先遍历检查VIP，是否有过时的状态
		TDBDecorateItemList * pItemList = rBag.GetDecorateGridList(1); //获取VIP背包
		HANDCHECH_P(pItemList,-1);
		for (int i = 0;i < pItemList->m_nGridsRef;i++)
		{
			if (i == 3)
			{
				return -1;
			}
			TDBDecorateBagVIPItem* pBagVIPItem = rBag.GetVIPTypeInfo(pItemList->m_astDecorateGrids[i].m_uiID);
			HANDCHECH_P(pBagVIPItem,-5);
			rHeroInfo.VipList[i].VipId = pItemList->m_astDecorateGrids[i].m_uiID;
			rHeroInfo.VipList[i].VipGreade = pBagVIPItem->m_uiGrade;
			rHeroInfo.VipList[i].VipExper = pBagVIPItem->m_uiExper;
		}
		//把该用户压入游戏房间中
		pHeroRoom->HeroList.push_back(rHeroInfo);
		SendHeroInfo(pRole,pRole->GetFd(),iter->Uid,PRoom);
	}
	//进入选英雄界面，就要把该房间清除
	//发送游戏选英雄界面定时器，规定多少时间给你去选英雄
	pHeroRoom->TimeIndex = GetTimeIndex(pHeroRoom->RoomIndex,Time_MateShowHeroFetch,pHeroRoom); //申请定时器
	int TimeValue = LOGIC_CONFIG->GetTimeConfig().GetMateShowHeroFetchTime();					//读取配置时间
	TimeSend(true,pHeroRoom->TimeIndex,TimeValue,pHeroRoom->RoomIndex);								
	return 0;
}



int MateWork::EnterHeroShowBag(const CSEnterHeroShowBagReq& rReq)
{
	//找到该玩家所在的房间
	int HeroRoomIndex = GetShowHeroRoom(rReq.uid());
	ShowHeroRoom* pShowHeroRoom = GetShowHeroRoomMap(HeroRoomIndex);
	HANDCHECH_P(pShowHeroRoom,-1);
	for (int i = 0;i < (int)pShowHeroRoom->HeroList.size();i++)
	{
		if (rReq.uid() == pShowHeroRoom->HeroList[i].Uid && rReq.decoratebagtype() == DIPAN_TYPE)
		{
			if (rReq.id() == 1)			//他有好看的特效，但是他选择了垃圾的特效，带的值肯定是 1
			{
				pShowHeroRoom->HeroList[i].PlaneId = rReq.id();
				break;
			}
			CRoleObj* pRole = GetRole(rReq.uid());
			HANDCHECH_P(pRole,-1);
			DecorateBagManager& rBag = pRole->GetDecorateBagMgr();
			TDBDecorateItem * pItem = rBag.GetDecorateGridByID(rReq.id(),rReq.decoratebagtype());
			HANDCHECH_P(pItem,-1);
			pShowHeroRoom->HeroList[i].DiPanId = rReq.id();
			break;
		}
		else if (rReq.uid() == pShowHeroRoom->HeroList[i].Uid && rReq.decoratebagtype() == ZHANJI_TYPE)
		{
			if (rReq.id() == 1)
			{
				pShowHeroRoom->HeroList[i].PlaneId = rReq.id();
				break;
			}
			CRoleObj* pRole = GetRole(rReq.uid());
			HANDCHECH_P(pRole,-1);
			DecorateBagManager& rBag = pRole->GetDecorateBagMgr();
			TDBDecorateItem * pItem = rBag.GetDecorateGridByID(rReq.id(),rReq.decoratebagtype());
			HANDCHECH_P(pItem,-1);
			pShowHeroRoom->HeroList[i].PlaneId = rReq.id();
			break;
		}
		else if (rReq.uid() == pShowHeroRoom->HeroList[i].Uid && rReq.decoratebagtype() == SKILLONE_TYPE)
		{
			if (rReq.id() == 1)
			{
				pShowHeroRoom->HeroList[i].PlaneId = rReq.id();
				break;
			}
			CRoleObj* pRole = GetRole(rReq.uid());
			HANDCHECH_P(pRole,-1);
			DecorateBagManager& rBag = pRole->GetDecorateBagMgr();
			TDBDecorateItem * pItem = rBag.GetDecorateGridByID(rReq.id(),rReq.decoratebagtype());
			HANDCHECH_P(pItem,-1);
			pShowHeroRoom->HeroList[i].SkillOneID = rReq.id();
			break;
		}
		else if (rReq.uid() == pShowHeroRoom->HeroList[i].Uid && rReq.decoratebagtype() == SKILLTWO_TYPE)
		{
			if (rReq.id() == 1)
			{
				pShowHeroRoom->HeroList[i].PlaneId = rReq.id();
				break;
			}
			CRoleObj* pRole = GetRole(rReq.uid());
			HANDCHECH_P(pRole,-1);
			DecorateBagManager& rBag = pRole->GetDecorateBagMgr();
			TDBDecorateItem * pItem = rBag.GetDecorateGridByID(rReq.id(),rReq.decoratebagtype());
			HANDCHECH_P(pItem,-1);
			pShowHeroRoom->HeroList[i].SkillTwoID = rReq.id();
			break;
		}
	}

	return 0;
}


int MateWork::SendUserShowZhaDan(CRoleObj* pRoleObj,uint64_t UserUid,uint64_t Uid,uint32_t ZhaDanId)
{
	CSMsg oCSMsg;
	CSSendUserHeroShowRsp* Rsp = static_cast<CSSendUserHeroShowRsp*>(MateHandler::OnCSMsg(oCSMsg, Uid, CS_MSGID_Mate, CSMateCmd_SendUserHeroShow));
	HANDCHECH_P(Rsp, -1);
	Rsp->set_uid(UserUid);
	Rsp->set_zhadanid(ZhaDanId);
	SendClient(pRoleObj->GetFd(),&oCSMsg);
	return 0;
}


int MateWork::ShowZhaDanBag(const CSShowZhaDanBagReq& rReq,CSShowZhaDanBagRsp* pRsp)
{
	//先找到房间
	int HeroRoomIndex = GetShowHeroRoom(rReq.uid());
	ShowHeroRoom* pShowHeroRoom = GetShowHeroRoomMap(HeroRoomIndex);
	HANDCHECH_P(pShowHeroRoom,-1);
	std::vector<uint32_t>::iterator iter = find(pShowHeroRoom->ZhaDanList.begin(),pShowHeroRoom->ZhaDanList.end(),rReq.zhadanid());
	if (iter != pShowHeroRoom->ZhaDanList.end())
	{
		return -1;
	}
	pShowHeroRoom->ZhaDanList.push_back(rReq.zhadanid());
	pRsp->set_zhadanid(rReq.zhadanid());
	//通知所有玩家发送该玩家已经选择该炸弹了
	for (int i = 0;i < (int)pShowHeroRoom->HeroList.size();i++)
	{
		if (pShowHeroRoom->HeroList[i].Uid == rReq.uid())
		{
			pShowHeroRoom->HeroList[i].ZhaDanId = rReq.zhadanid();
		}
		else
		{
			//广播给玩家
			CRoleObj* pRole = GetRole(pShowHeroRoom->HeroList[i].Uid);
			HANDCHECH_P(pRole,-1);
			SendUserShowZhaDan(pRole,rReq.uid(),pShowHeroRoom->HeroList[i].Uid,rReq.zhadanid());
		}
	}
	return 0;
}