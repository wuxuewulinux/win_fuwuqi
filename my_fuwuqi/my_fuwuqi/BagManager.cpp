#include "BagManager.hpp"
#include <string.h>
#include <stdlib.h>
#include "quanju.hpp"

CBagManager::CBagManager()
{

}

CBagManager::~CBagManager()
{

}

int CBagManager::Init()
{
	memset(&m_stBagInfo, 0, sizeof(m_stBagInfo));

	return 0;
}

int CBagManager::SetBagInfo(const DBBagInfo* pBagInfo)
{
	if ( pBagInfo )
	{
		m_stBagInfo.PB2ST(*pBagInfo);
	}
	
	TDBBagGridList & rGridList = m_stBagInfo.m_stGridList;
	qsort( rGridList.m_astGrids, rGridList.m_nGridsRef, sizeof(TDBBagGrid), TDBBagGrid::Compare );
	
	return 0;
}

int CBagManager::GenDBBagInfo(DBBagInfo& rBagInfo)
{
	m_stBagInfo.ST2PB(rBagInfo);	
	return 0;
}


int CBagManager::GetGridNum()
{
	return m_stBagInfo.m_stGridList.m_nGridsRef ;
}

int CBagManager::SetGridNum( uint32_t uiNum )
{
	if ( uiNum > DB_MAX_BAG_GRID_NUM )
	{
		return -1;
	}
	m_stBagInfo.m_stGridList.m_nGridsRef = uiNum;	
	return 0;
}

TDBBagGrid * CBagManager::GetGridByID( uint32_t uiItemId )
{
	TDBBagGrid stGrid;
	stGrid.m_uiID = uiItemId;
	TDBBagGridList & rGridList = m_stBagInfo.m_stGridList;
	TDBBagGrid * pGrid = (TDBBagGrid *) bsearch( &stGrid, rGridList.m_astGrids, rGridList.m_nGridsRef, sizeof( TDBBagGrid), TDBBagGrid::Compare );
	
	return pGrid;
}

TDBBagGrid * CBagManager::GetGridByIdx( uint32_t uiIdx )
{
	if ( (int)uiIdx >= m_stBagInfo.m_stGridList.m_nGridsRef )
	{
		return NULL;
	}
	return & m_stBagInfo.m_stGridList.m_astGrids[uiIdx];
}

TDBBagGrid * CBagManager::AddGrid( uint32_t uiItemId, uint32_t uiNum )
{
	TDBBagGridList & rGridList = m_stBagInfo.m_stGridList;
	
	if ( m_stBagInfo.m_stGridList.m_nGridsRef >= DB_MAX_BAG_GRID_NUM )	
	{
		return NULL ;		
	}

	TDBBagGrid * pGrid = &rGridList.m_astGrids[ rGridList.m_nGridsRef++ ];
	
	pGrid->m_uiID = uiItemId;
	pGrid->m_uiNum = uiNum ;

	qsort( rGridList.m_astGrids, rGridList.m_nGridsRef, sizeof(TDBBagGrid), TDBBagGrid::Compare );
	
	TDBBagGrid stGrid;
	stGrid.m_uiID = uiItemId;
	pGrid = (TDBBagGrid *) bsearch( &stGrid, rGridList.m_astGrids, rGridList.m_nGridsRef, sizeof( TDBBagGrid), TDBBagGrid::Compare );
	
	return pGrid;

}

TDBBagGridList & CBagManager::GetGridList( )
{
	return m_stBagInfo.m_stGridList ;
}

int CBagManager::DelGrid( uint32_t uiItemId )
{
	TDBBagGrid * pDelGrid = GetGridByID( uiItemId );
	HANDCHECH_P( pDelGrid , -1 );	
	TDBBagGridList & rGridList = m_stBagInfo.m_stGridList;
	rGridList.m_nGridsRef--;
	* pDelGrid = rGridList.m_astGrids[ rGridList.m_nGridsRef ];	
	qsort( rGridList.m_astGrids, rGridList.m_nGridsRef, sizeof(TDBBagGrid), TDBBagGrid::Compare );

	return 0;

}
