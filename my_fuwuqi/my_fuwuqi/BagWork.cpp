
#include "BagWork.hpp"
#include "quanju.hpp"
#include "BagManager.hpp"

const uint32_t ItemNotBag = 500;			//不显示在背包物品的ID取值范围为 1 - 499. 显示在背包的物品ID从500开始叠加

int CBagFramework::GenCSGrid( TDBBagGrid & rTGrid, CSBagGridInfo & rCSGrid )
{
	rCSGrid.set_itemid( rTGrid.m_uiID );
	rCSGrid.set_num( rTGrid.m_uiNum );

	return 0;
}

int CBagFramework::GenCSBagInfo( CRoleObj * pRoleObj, CSBagBagInfo & rCSBagInfo )
{
	HANDCHECH_P( pRoleObj, -1 );
	CBagManager & rBagMgr = pRoleObj->GetBagMgr();
	TDBBagGridList & rTGridList = rBagMgr.GetGridList();
	
	for ( int i=0 ; i< rTGridList.m_nGridsRef ; ++i )
	{   //大于等于500的物品才能显示在背包中
		if (rTGridList.m_astGrids[i].m_uiID >= ItemNotBag)
		{
			CSBagGridInfo * pCSGridInfo = rCSBagInfo.add_gridinfo();
			GenCSGrid( rTGridList.m_astGrids[i], *pCSGridInfo );
		}	
	} 

	return 0;
} 

