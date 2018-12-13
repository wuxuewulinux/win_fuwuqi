
#include "BagWork.hpp"
#include "quanju.hpp"
#include "BagManager.hpp"

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
	{
		CSBagGridInfo * pCSGridInfo = rCSBagInfo.add_gridinfo();
		GenCSGrid( rTGridList.m_astGrids[i], *pCSGridInfo );
	} 

	return 0;
} 

