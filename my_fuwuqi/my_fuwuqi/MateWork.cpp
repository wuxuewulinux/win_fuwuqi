
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
	OpenMateSuo();		//������
	//�����ƥ�䷿�䣬�Ȳ鿴�Ƿ��п��еķ���
	bool Falge = false;
	if (RoomMap.size() > 0)
	{
		//�з��䣬�������з����ҳ����е�λ��
		for (std::map<int,Room>::iterator iter = RoomMap.begin(); iter != RoomMap.end();iter++)
		{
			if (iter->second.MaxCount > iter->second.UserCount)
			{
				//�÷�����λ�ã��Ѹ���Ҽ���÷�����
				++iter->second.UserCount;
				UidInfo rInfo;
				rInfo.Uid = rReq.uid();
				rInfo.Button = false;
				iter->second.UidList.push_back(rInfo);
				PushUserRoom(rReq.uid(),iter->first);
				Falge = true;
				if (iter->second.MaxCount == iter->second.UserCount)
				{
					//���������ˣ�������Ϸ��ʼ���͸��÷����������
					for (int i = 0; i < (int)iter->second.UidList.size();i++)
					{
						CRoleObj* pRoleObj = GetRole(iter->second.UidList[i].Uid);
						if (pRoleObj == NULL)
						{
							CloseMateSuo();
							return -3;
						}
						SendMateFetch(pRoleObj->GetFd(),iter->second.UidList[i].Uid);
						//ѹ�붨ʱ�� 10 �룬����ʮ��ʱ��ȥ���ͬ�⣬���ʱ�䵽������ʹ����¼�
						//�����¼�ִ�аѲ����ͬ�������Ƴ����䡣

						iter->second.TimeIndex = GetTimeIndex(iter->first,Time_MateFetch,&iter->second); //���붨ʱ��
						int TimeValue = LOGIC_CONFIG->GetTimeConfig().GetMateFetchTime();
						TimeSend(true,iter->second.TimeIndex,TimeValue,iter->first);		//��ʱ10��
					}
				}
				break;
			}
		}
		if (Falge == false)	//�п���ȫ�����䶼���ˡ�����Ҫ������������
		{
			int RoomIndex = AskRoomIndex();
			Room rRoom;
			rRoom.RoomIndex = RoomIndex;
			rRoom.UserCount = 0;
			rRoom.Agree = 0;
			rRoom.MaxCount = 5;					//����˲ſ�ʼ��Ϸ
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
		//û���κεȴ��ķ����Ҫ�����µķ���������
		int RoomIndex = AskRoomIndex();
		Room rRoom;
		rRoom.TimeIndex = 0;
		rRoom.UserCount = 0;
		rRoom.Agree = 0;
		rRoom.MaxCount = 5;					//����˲ſ�ʼ��Ϸ
		++rRoom.UserCount;
		UidInfo rInfo;
		rInfo.Uid = rReq.uid();
		rInfo.Button = false;
		rRoom.UidList.push_back(rInfo);
		PushRoomMap(RoomIndex,&rRoom);
		PushUserRoom(rReq.uid(),RoomIndex);
	}
	CloseMateSuo();			//�ͷ���
	return 0;
}



int MateWork::QuitMateFetch(const CSQuitMateFetchReq& rReq)
{
	//�ҵ�����ҵķ���ţ�Ȼ���Ƴ���ȥ
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
	//�жϸ��û��Ƴ�ʱ�����仹��û�ˣ�û���˾�Ҫ�������
	if (pRoom->UserCount == 0 && pRoom->UidList.size() == 0)
	{
		DeleteRoomMap(RoomIndex);
		DeleteRoomIndex(RoomIndex);
	}
	return 0;
}


int MateWork::NotButtonMateFetch(const CSNotButtonMateFetchReq& rReq)
{
	OpenMateSuo();		//������
	//�ҵ�����ҵķ���ţ�Ȼ���Ƴ���ȥ
	int RoomIndex = GetUserRoom(rReq.uid());
	if (RoomIndex == 0)
	{
		CloseMateSuo();		//�ͷ���
		return -1;
	}
	Room* pRoom = GetRoomMap(RoomIndex);
	if (pRoom == NULL)
	{
		CloseMateSuo();		//�ͷ���
		return -2;
	}
	pRoom->Agree = 0;
	//��ֹͬһ����������ͬʱ����ܾ���
	if (pRoom->UserCount != 5 || pRoom->UidList.size() != 5)
	{
		CloseMateSuo();		//�ͷ���
		return 0;
	}
	//���͹ص���ʱʱ��
	TimeSend(false,pRoom->TimeIndex,0,pRoom->RoomIndex);
	--pRoom->UserCount;
	std::vector<UidInfo>::iterator iter = pRoom->UidList.begin();
	while( iter != pRoom->UidList.end())
	{
		if (iter->Uid == rReq.uid())
		{
			//�����û�е��ͬ�⣬�����ӷ����Ƴ���ȥ
			DeleteUserRoom(iter->Uid);
			iter = pRoom->UidList.erase(iter);
			pRoom->UserCount = pRoom->UserCount - 1;
		}
		else
		{
			//�㲥Э��˵�����û�е��ͬ��
			CRoleObj* pRoleObj = GetRole(iter->Uid);
			if (pRoleObj == NULL)
			{
				CloseMateSuo();		//�ͷ���
				return -1;
			}
			MateWork::SendMateNotSuccess(pRoleObj->GetFd(),iter->Uid);
			iter->Button = false;
			iter++;
		}
	}
	//�жϸ��û��Ƴ�ʱ�����仹��û�ˣ�û���˾�Ҫ�������
	if (pRoom->UserCount == 0 && pRoom->UidList.size() == 0)
	{
		DeleteRoomMap(RoomIndex);
		DeleteRoomIndex(RoomIndex);
	}
	CloseMateSuo();		//�ͷ���
	return 0;
}



int MateWork::SuccessButtonMateFetch(const CSSuccessButtonMateFetchReq& rReq)
{
	//�ҵ�����ҵ���ң���������ͬ��
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
				//����÷���Ķ�ʱ������
				TimeSend(false,pRoom->TimeIndex,0,pRoom->RoomIndex);		//�������ʱ��ʱ��
				DeleteTimeIndex(pRoom->TimeIndex,pRoom->RoomIndex);
				//�÷��������˶�ͬ�⿪ʼ��Ϸ������ѡӢ�۽���
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
	//ȡ������ҵ�Vip��Ϣ
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
	//��ʼ������Ҹ�������Э�鸳ֵ
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
			UserBag.CheckDecorateItemTime(ZHADAN_TYPE);//ˢ��ʱ��
			TDBDecorateItemList * pItemList = UserBag.GetDecorateGridList(ZHADAN_TYPE);
			HANDCHECH_P(pItemList,-1);
			for (int j = 0; j < pItemList->m_nGridsRef;j++)
			{
				pZhaDanBagList->add_zhadan(pItemList->m_astDecorateGrids[j].m_uiID);
			}
		}
		else if (i == DIPAN_TYPE)
		{
			UserBag.CheckDecorateItemTime(DIPAN_TYPE);//ˢ��ʱ��
			TDBDecorateItemList * pItemList = UserBag.GetDecorateGridList(DIPAN_TYPE);
			HANDCHECH_P(pItemList,-1);
			for (int j = 0; j < pItemList->m_nGridsRef;j++)
			{
				pFeiJiDiPanBagList->add_feijidipan(pItemList->m_astDecorateGrids[j].m_uiID);
			}
		}
		else if (i == ZHANJI_TYPE)
		{
			UserBag.CheckDecorateItemTime(ZHANJI_TYPE);//ˢ��ʱ��
			TDBDecorateItemList * pItemList = UserBag.GetDecorateGridList(ZHANJI_TYPE);
			HANDCHECH_P(pItemList,-1);
			for (int j = 0; j < pItemList->m_nGridsRef;j++)
			{
				pFeiJiBagList->add_feiji(pItemList->m_astDecorateGrids[j].m_uiID);
			}
		}
		else if (i == SKILLONE_TYPE)
		{
			UserBag.CheckDecorateItemTime(SKILLONE_TYPE);//ˢ��ʱ��
			TDBDecorateItemList * pItemList = UserBag.GetDecorateGridList(SKILLONE_TYPE);
			HANDCHECH_P(pItemList,-1);
			for (int j = 0; j < pItemList->m_nGridsRef;j++)
			{
				pFeiJiSkillOneBagList->add_feijiskillone(pItemList->m_astDecorateGrids[j].m_uiID);
			}
		}
		else if (i == SKILLTWO_TYPE)
		{
			UserBag.CheckDecorateItemTime(SKILLTWO_TYPE);//ˢ��ʱ��
			TDBDecorateItemList * pItemList = UserBag.GetDecorateGridList(SKILLTWO_TYPE);
			HANDCHECH_P(pItemList,-1);
			for (int j = 0; j < pItemList->m_nGridsRef;j++)
			{
				pFeiJiSkillTwoBagList->add_feijiskilltwo(pItemList->m_astDecorateGrids[j].m_uiID);
			}
		}
	}
	//ȡ������ҵ��û���Ϣ���ݣ��������Լ�������
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
	//����һ����ϷѡӢ�۽��淿��
	int HeroRoomIndex = AskShowHeroRoomIndex();
	ShowHeroRoom rHeroRoom;
	rHeroRoom.RoomIndex = HeroRoomIndex;
	rHeroRoom.MateRoomIndex = iDeleteRoomIndex;
	PushShowHeroRoomMap(HeroRoomIndex,&rHeroRoom);
	ShowHeroRoom* pHeroRoom = GetShowHeroRoomMap(HeroRoomIndex);
	HANDCHECH_P(pHeroRoom,-1);
	for (std::vector<UidInfo>::iterator iter = PRoom->UidList.begin(); iter != PRoom->UidList.end(); iter++)
	{
		DeleteUserRoom(iter->Uid);					//��������Ҫ�Ƴ�ƥ��
		PushShowHeroRoom(iter->Uid,HeroRoomIndex);	
		CRoleObj* pRole = GetRole(iter->Uid);
		HANDCHECH_P(pRole,-1);
		//ȡ����һ�������ѹ�뵽�����У���Ϊ����ս��ʱ��Ҫ��ҵĻ�������
		HeroInfo rHeroInfo;
		memset(&rHeroInfo,0,sizeof(rHeroInfo));
		rHeroInfo.Name = pRole->GetName();
		rHeroInfo.Level = pRole->GetLevel();
		rHeroInfo.LevelExper = pRole->GetLevelExper();
		rHeroInfo.Rank = pRole->GetRand();
		rHeroInfo.RankExper = pRole->GetRandExper();
		rHeroInfo.Uid = iter->Uid;
		DecorateBagManager& rBag = pRole->GetDecorateBagMgr();
		rBag.CheckDecorateItemTime(1); //�ȱ������VIP���Ƿ��й�ʱ��״̬
		TDBDecorateItemList * pItemList = rBag.GetDecorateGridList(1); //��ȡVIP����
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
		//�Ѹ��û�ѹ����Ϸ������
		pHeroRoom->HeroList.push_back(rHeroInfo);
		SendHeroInfo(pRole,pRole->GetFd(),iter->Uid,PRoom);
	}
	//����ѡӢ�۽��棬��Ҫ�Ѹ÷������
	//������ϷѡӢ�۽��涨ʱ�����涨����ʱ�����ȥѡӢ��
	pHeroRoom->TimeIndex = GetTimeIndex(pHeroRoom->RoomIndex,Time_MateShowHeroFetch,pHeroRoom); //���붨ʱ��
	int TimeValue = LOGIC_CONFIG->GetTimeConfig().GetMateShowHeroFetchTime();					//��ȡ����ʱ��
	TimeSend(true,pHeroRoom->TimeIndex,TimeValue,pHeroRoom->RoomIndex);								
	return 0;
}



int MateWork::EnterHeroShowBag(const CSEnterHeroShowBagReq& rReq)
{
	//�ҵ���������ڵķ���
	int HeroRoomIndex = GetShowHeroRoom(rReq.uid());
	ShowHeroRoom* pShowHeroRoom = GetShowHeroRoomMap(HeroRoomIndex);
	HANDCHECH_P(pShowHeroRoom,-1);
	for (int i = 0;i < (int)pShowHeroRoom->HeroList.size();i++)
	{
		if (rReq.uid() == pShowHeroRoom->HeroList[i].Uid && rReq.decoratebagtype() == DIPAN_TYPE)
		{
			if (rReq.id() == 1)			//���кÿ�����Ч��������ѡ������������Ч������ֵ�϶��� 1
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
	//���ҵ�����
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
	//֪ͨ������ҷ��͸�����Ѿ�ѡ���ը����
	for (int i = 0;i < (int)pShowHeroRoom->HeroList.size();i++)
	{
		if (pShowHeroRoom->HeroList[i].Uid == rReq.uid())
		{
			pShowHeroRoom->HeroList[i].ZhaDanId = rReq.zhadanid();
		}
		else
		{
			//�㲥�����
			CRoleObj* pRole = GetRole(pShowHeroRoom->HeroList[i].Uid);
			HANDCHECH_P(pRole,-1);
			SendUserShowZhaDan(pRole,rReq.uid(),pShowHeroRoom->HeroList[i].Uid,rReq.zhadanid());
		}
	}
	return 0;
}