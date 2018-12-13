
#include "RoleObj.hpp"
#include "quanju.hpp"

int CRoleObj::Init()
{
	uUID = 0;											
	uiLevel = 0;										
	uiRank = 0;										
	uiRankExper = 0;									
	uiLevelExper = 0;	
	//ÿ��ģ�鶼���װ�Լ���Init()���� ֱ����ģ��Ķ������Init()�����Ϳ�����

	m_oBagMgr.Init();						//����ģ��

	return 0;
}

int CRoleObj::SetRoleInfo(const DBRoleInfo& rRoleInfo)
{
	if(rRoleInfo.has_uid())
	{
		SetUid(rRoleInfo.uid());
	}
	if (rRoleInfo.has_name())
	{
		SetName(rRoleInfo.name());
	}
	if (rRoleInfo.has_level())
	{
		SetLevel(rRoleInfo.level());
	}
	if (rRoleInfo.has_rank())
	{
		SetRand(rRoleInfo.rank());
	}
	if (rRoleInfo.has_levelexper())
	{
		SetLevelExper(rRoleInfo.levelexper());
	}
	if (rRoleInfo.has_rankexper())
	{
		SetRandExper(rRoleInfo.rankexper());
	}
	//���¾��ǵ��ø���ģ������ú���
	if ( rRoleInfo.has_baginfo() )								//����ģ��
	{
		m_oBagMgr.SetBagInfo( & rRoleInfo.baginfo() );
	}

	return 0;
}
int CRoleObj::GenDBRoleInfo(DBRoleInfo* pRoleInfo)
{
	pRoleInfo->set_uid(uUID);
	pRoleInfo->set_name(sName);
	pRoleInfo->set_level(uiLevel);
	pRoleInfo->set_rank(uiRank);
	pRoleInfo->set_levelexper(uiLevelExper);
	pRoleInfo->set_rankexper(uiRankExper);
	//���¾��ǵ��ø���ģ������ú���

	//����ģ��
	DBBagInfo * pDBBagInfo = pRoleInfo->mutable_baginfo();
	HANDCHECH_P( pDBBagInfo, -5);	
	m_oBagMgr.GenDBBagInfo( *pDBBagInfo );

	return 0;
}