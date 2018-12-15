
#include "DecorateBagManager.hpp"
#include <stdlib.h>
#include <string.h>
#include "quanju.hpp"

DecorateBagManager::DecorateBagManager()
{


}


DecorateBagManager::~DecorateBagManager()
{



}


int DecorateBagManager::Init()
{
	memset(&rBagModuleInfo, 0, sizeof(rBagModuleInfo));
	return 0;
}

int DecorateBagManager::SetDecorateBagInfo( const DBDecorateBagModuleInfo* pDecorateBagInfo )
{
	if ( pDecorateBagInfo )
	{
		rBagModuleInfo.PB2ST(*pDecorateBagInfo);
	}

	return 0;
}
int DecorateBagManager::GenDBDecorateBagInfo( DBDecorateBagModuleInfo& rDecorateBagInfo )
{
	rBagModuleInfo.ST2PB(rDecorateBagInfo);	
	return 0;
}


TDBDecorateItem * DecorateBagManager::GetDecorateGridByID( uint32_t uiId ,uint32_t uiType)
{
	if (uiType > DB_MAX_DECORATE_BAG)
	{
		return NULL;
	}
	TDBDecorateItem stGrid;
	stGrid.m_uiID = uiId;
	TDBDecorateItemList & rGridList = rBagModuleInfo.m_stDecorateBagInfoList.m_astDecorateBagList[uiType -1].m_stBagGridList;		//���������ҵ���Ӧ��װ�α���
	TDBDecorateItem * pGrid = (TDBDecorateItem *)bsearch( &stGrid, rGridList.m_astDecorateGrids, rGridList.m_nGridsRef, sizeof( TDBDecorateItem), TDBDecorateItem::Compare );

	return pGrid; 
}

TDBDecorateItemList * DecorateBagManager::GetDecorateGridList(uint32_t uiType)
{
	if (uiType > DB_MAX_DECORATE_BAG)
	{
		return NULL;
	}
	return &rBagModuleInfo.m_stDecorateBagInfoList.m_astDecorateBagList[uiType -1].m_stBagGridList;
}



TDBDecorateItem * DecorateBagManager::AddDecorateGrid( uint32_t uiId, uint32_t uiType,long rEndTime )
{
	TDBDecorateItemList & rGridList = rBagModuleInfo.m_stDecorateBagInfoList.m_astDecorateBagList[uiType -1].m_stBagGridList;
	if ( rGridList.m_nGridsRef >= DB_MAX_DECORATE_BAG_NUM )	
	{
		return NULL ;		
	}
	TDBDecorateItem * pGrid = &rGridList.m_astDecorateGrids[ rGridList.m_nGridsRef++ ];
	pGrid->m_uiID = uiId;
	pGrid->m_uiType = uiType;
	pGrid->m_uiEndTime = rEndTime;
	qsort( rGridList.m_astDecorateGrids, rGridList.m_nGridsRef, sizeof(TDBDecorateItem), TDBDecorateItem::Compare );

	TDBDecorateItem stGrid;
	stGrid.m_uiID = uiId;
	pGrid = (TDBDecorateItem *) bsearch( &stGrid, rGridList.m_astDecorateGrids, rGridList.m_nGridsRef, sizeof( TDBDecorateItem), TDBDecorateItem::Compare );

	return pGrid;
}


int DecorateBagManager::DelDecorateGrid( uint32_t uiItemId,uint32_t uiType )
{
	TDBDecorateItem * pDelGrid = GetDecorateGridByID( uiItemId ,uiType);
	HANDCHECH_P( pDelGrid , -1 );	
	TDBDecorateItemList & rGridList = rBagModuleInfo.m_stDecorateBagInfoList.m_astDecorateBagList[uiType -1].m_stBagGridList;
	rGridList.m_nGridsRef--;
	* pDelGrid = rGridList.m_astDecorateGrids[ rGridList.m_nGridsRef ];	
	qsort( rGridList.m_astDecorateGrids, rGridList.m_nGridsRef, sizeof(TDBDecorateItem), TDBDecorateItem::Compare );

	return 0;
}



int DecorateBagManager::AddDecorateItem(uint32_t uiId, uint32_t uiType,long rEndTime)
{
	//�Ȳ������������Ƿ��и���Ʒ
	TDBDecorateItem * pBagItem =  GetDecorateGridByID(uiId,uiType);
	if (pBagItem)
	{
		//��Ϊ��֤�������Ѿ����ڸ���ƷID��,��ԭ�л������ж�ʱ���Ƿ��ʱ
		if (pBagItem->m_uiEndTime < TIMESTAMP)
		{
			//��֮ǰ�����Ѿ���ʱ��
			DelDecorateGrid(uiId,uiType);			//ɾ��֮ǰ�ĸ���
			AddDecorateGrid(uiId,uiType,rEndTime);	//����һ���µĸ���
		}
		else
		{
			//��֮ǰ��������ǻ�û�й�ʱ
			pBagItem->m_uiEndTime = (pBagItem->m_uiEndTime - TIMESTAMP) + rEndTime;			//��������ʱ��
		}
		
	}
	else	//����֮ǰû���ڸ�װ����Ʒ����Ҫ��������װ����ƷID����������
	{
		AddDecorateGrid(uiId,uiType,rEndTime);
	}
	return 0;
}



uint64_t DecorateBagManager::GetDecorateItemTime(uint32_t uiId, uint32_t uiType)
{
	TDBDecorateItem * pBagItem =  GetDecorateGridByID(uiId,uiType);
	HANDCHECH_P(pBagItem,-1);
	return pBagItem->m_uiEndTime;
}



int DecorateBagManager::CheckDecorateItemTime(uint32_t uiType)
{
	TDBDecorateItemList * pItemList = GetDecorateGridList(uiType);
	HANDCHECH_P(pItemList,-1);
	//��ʼ����������װ�α������Դ��ڵ�װ����Ʒʱ��
	for (int16_t i =0; i < pItemList->m_nGridsRef; i++)
	{
		if (pItemList->m_astDecorateGrids[i].m_uiEndTime < TIMESTAMP)		//���ʱ���ʱ�˾�Ҫ�������װ����Ʒ
		{
			DelDecorateGrid(pItemList->m_astDecorateGrids[i].m_uiID,pItemList->m_astDecorateGrids[i].m_uiType);
		}
	}
	return 0;
}