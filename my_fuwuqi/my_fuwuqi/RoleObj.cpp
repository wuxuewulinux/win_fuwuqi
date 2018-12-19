
#include "RoleObj.hpp"
#include "quanju.hpp"

int CRoleObj::Init()
{
	uUID = 0;											
	uiLevel = 0;										
	uiRank = 0;										
	uiRankExper = 0;									
	uiLevelExper = 0;	
	//每个模块都会封装自己的Init()函数 直接用模块的对象调用Init()函数就可以了

	m_oBagMgr.Init();						//背包模块
	m_oDecorateBagMgr.Init();				//装饰背包模块

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
	//以下就是调用各个模块的设置函数

	if ( rRoleInfo.has_baginfo() )								//背包模块
	{
		m_oBagMgr.SetBagInfo( & rRoleInfo.baginfo() );
	}

	if (rRoleInfo.has_decoratebagmoduleinfo())
	{
		m_oDecorateBagMgr.SetDecorateBagInfo(&rRoleInfo.decoratebagmoduleinfo());	//装饰背包模块
	}
	if (rRoleInfo.has_vipinfo())
	{
		m_oDecorateBagMgr.SetDecorateVIPBagInfo(&rRoleInfo.vipinfo());				//装饰背包VIP记录
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
	//以下就是调用各个模块的设置函数

	//背包模块
	DBBagInfo * pDBBagInfo = pRoleInfo->mutable_baginfo();
	HANDCHECH_P( pDBBagInfo, -5);	
	m_oBagMgr.GenDBBagInfo( *pDBBagInfo );

	//装饰背包模块
	DBDecorateBagModuleInfo * pDBDecorateBagInfo = pRoleInfo->mutable_decoratebagmoduleinfo();
	HANDCHECH_P( pDBDecorateBagInfo, -6);	
	m_oDecorateBagMgr.GenDBDecorateBagInfo(*pDBDecorateBagInfo);

	//装饰背包VIP记录保存
	DBDecorateBagVIPInfo * pDBDecorateVIPBagInfo = pRoleInfo->mutable_vipinfo();
	HANDCHECH_P( pDBDecorateVIPBagInfo, -6);	
	m_oDecorateBagMgr.GenDBDecorateVIPBagInfo(*pDBDecorateVIPBagInfo);

	return 0;
}