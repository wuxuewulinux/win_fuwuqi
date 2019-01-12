
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
	OpenMateSuo();		//������
	//���˲����ͬ�������Ϸ�����Ե�������������
	Room* rUserRoom = (Room*)Data;
	if (rUserRoom == NULL)
	{
		CloseMateSuo();		//�ͷ���
		return;
	}
	if (rUserRoom->UserCount != 5 || rUserRoom->UidList.size() != 5)
	{
		CloseMateSuo();		//�ͷ���
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
				CloseMateSuo();		//�ͷ���
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
				CloseMateSuo();		//�ͷ���
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
	CloseMateSuo();		//�ͷ���
	return;
}


void Time_MateShowHeroFetch(void * Data)
{
	ShowHeroRoom* rUserRoom = (ShowHeroRoom*)Data;
	if (rUserRoom == NULL)
	{
		return;
	}
	//���֮ǰƥ��ɹ��ķ��䡣
	DeleteRoomMap(rUserRoom->MateRoomIndex);
	DeleteRoomIndex(rUserRoom->MateRoomIndex);
	//��ȡһ��ս����ͼ
	rUserRoom->DiTuID = GetMapId();
	//��ȡս��������ID
	rUserRoom->FightServerID = LOGIC_CONFIG->GetFightServerConfig().GetEmptyFightServerID();
	//���Ӹ�ս����������������ս����������
	LOGIC_CONFIG->GetFightServerConfig().AddFightServerUserCount(rUserRoom->FightServerID,5);
	//����װ�α���ȡ��Ĭ�����õ�ID
	for (int i = 0; i < (int)rUserRoom->HeroList.size(); i++)
	{
		CRoleObj* pRoleObj = GetRole(rUserRoom->HeroList[i].Uid);
		if (pRoleObj == NULL)
		{
			return;
		}
		if (rUserRoom->HeroList[i].ZhaDanId == 0)
		{
			//���û��ѡ��ը������Ҫ�������һ��ը��ID
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
			//���û��ѡ��ɻ�����Ҫȥװ�α���ȡ��Ĭ��ID
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
	//���������ݸ��ͻ��ˣ�Ȼ��ͻ����õ����ݽ���ս������
	for (int i = 0; i < (int)rUserRoom->HeroList.size();i++)
	{
		//�Ƴ������е�ѡ��Ӣ�۽����û�Io
		DeleteShowHeroRoom(rUserRoom->HeroList[i].Uid);
		CRoleObj* pRoleObj = GetRole(rUserRoom->HeroList[i].Uid);
		if (pRoleObj == NULL)
		{
			return;
		}
		//������������㲥��Һ�������ս��״̬
		UpdateData::UpMsgStatus(rUserRoom->HeroList[i].Uid,2,STATUS);
		//�����ݽ���ս������
		SendHeroEnterFight(pRoleObj,pRoleObj->GetFd(),pRoleObj->GetUid(),rUserRoom);
	}
	
	//ɾ��Ӣ��ѡ����涨ʱ��
	TimeSend(false,rUserRoom->TimeIndex,0,rUserRoom->RoomIndex);		//�������ʱ��ʱ��
	DeleteTimeIndex(rUserRoom->TimeIndex,rUserRoom->RoomIndex);
	//ɾ��һ��ѡ��Ӣ�۽��淿��,����������ɾ��������ȴ��÷���ս��������ս�����������ز���ɾ�����䣬��Ȼ�п�������ս���ķ����һ��
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