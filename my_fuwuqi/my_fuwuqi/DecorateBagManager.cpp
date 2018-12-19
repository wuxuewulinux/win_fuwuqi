
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
	memset(&rBagVIPInfo,0,sizeof(rBagVIPInfo));
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


int DecorateBagManager::SetDecorateVIPBagInfo( const DBDecorateBagVIPInfo* pDecorateVIPBagInfo )
{
	if ( pDecorateVIPBagInfo )
	{
		rBagVIPInfo.PB2ST(*pDecorateVIPBagInfo);
	}
	return 0;
}


int DecorateBagManager::GenDBDecorateVIPBagInfo( DBDecorateBagVIPInfo& rDecorateVIPBagInfo )
{
	rBagVIPInfo.ST2PB(rDecorateVIPBagInfo);
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
	TDBDecorateItemList & rGridList = rBagModuleInfo.m_stDecorateBagInfoList.m_astDecorateBagList[uiType -1].m_stBagGridList;		//根据类型找到对应的装饰背包
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
	//先查找人物身上是否有该物品
	TDBDecorateItem * pBagItem =  GetDecorateGridByID(uiId,uiType);
	if (pBagItem)
	{
		//不为空证明人物已经存在该物品ID了,在原有基础上判断时间是否过时
		if (pBagItem->m_uiEndTime < TIMESTAMP)
		{
			//他之前购买已经过时了
			DelDecorateGrid(uiId,uiType);			//删除之前的格子
			AddDecorateGrid(uiId,uiType,rEndTime);	//增加一个新的格子
		}
		else
		{
			//他之前购买过但是还没有过时
			pBagItem->m_uiEndTime = (pBagItem->m_uiEndTime - TIMESTAMP) + rEndTime;			//给他增加时间
		}
		
	}
	else	//人物之前没存在该装饰物品，就要给他增加装饰物品ID到人物身上
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



uint32_t DecorateBagManager::CheckDecorateItemTime(uint32_t uiType)
{
	if (uiType >= DB_MAX_DECORATE_BAG)
	{
		return 0;
	}
	TDBDecorateItemList * pItemList = GetDecorateGridList(uiType);
	HANDCHECH_P(pItemList,-1);
	//每个背包装饰都会有0和1表示系统默认的垃圾物品显示，1的出现是因为他有漂亮的装饰物品他就选择系统默认设置的。
	if (rBagModuleInfo.m_astTypeList[uiType] == 1)			
	{
		return rBagModuleInfo.m_astTypeList[uiType];
	}
	//开始遍历该种类装饰背包所以存在的装饰物品时间
	for (int16_t i =0; i < pItemList->m_nGridsRef; i++)
	{
		if (pItemList->m_astDecorateGrids[i].m_uiEndTime < TIMESTAMP)		//如果时间过时了就要清除掉该装饰物品
		{
			DelDecorateGrid(pItemList->m_astDecorateGrids[i].m_uiID,pItemList->m_astDecorateGrids[i].m_uiType);
		}
	}
	//他之前有装饰物品有默认值，如果现在没有了就设置为0
	if (pItemList->m_nGridsRef == 0 && rBagModuleInfo.m_astTypeList[uiType] > 1)
	{
		rBagModuleInfo.m_astTypeList[uiType] = 0;				//每个装饰背包都会1的物品ID就是默认物品（1为垃圾物品)
	}
	else if (rBagModuleInfo.m_astTypeList[uiType] == 0 && pItemList->m_nGridsRef > 0)
	{
		//设置默认值ID
		rBagModuleInfo.m_astTypeList[uiType] = pItemList->m_astDecorateGrids[pItemList->m_nGridsRef - 1].m_uiID;
	}
	else if (rBagModuleInfo.m_astTypeList[uiType] > 1 && pItemList->m_nGridsRef > 0)
	{
		//开始遍历默认值是否存在。
		TDBDecorateItem * pItem = GetDecorateGridByID(rBagModuleInfo.m_astTypeList[uiType],uiType);
		if (!pItem)
		{
			//不满足就要移动位置设置默认值
			rBagModuleInfo.m_astTypeList[uiType] = pItemList->m_astDecorateGrids[pItemList->m_nGridsRef - 1].m_uiID;
		}
	}
	
	return rBagModuleInfo.m_astTypeList[uiType];
}



int DecorateBagManager::SetDecorateItemShowBag(uint32_t uiId, uint32_t uiType)
{
	if (uiType >= DB_MAX_DECORATE_BAG)
	{
		return 0;
	}
	if (uiId == 1)
	{
		rBagModuleInfo.m_astTypeList[uiType] = 1;
	}
	if (uiId > 1)		//他有好看的装饰物品，判断他的背包是否存在该好看的物品
	{
		CheckDecorateItemTime(uiType); //先刷新数据
		TDBDecorateItem * pItem = GetDecorateGridByID(uiId,uiType);
		if (!pItem)		//数据有BUG
		{
			return -1;
		}
		rBagModuleInfo.m_astTypeList[uiType] = uiId;	//成功设置成该默认值
	}

	return 0;
}



int DecorateBagManager::AllCheckDecorateItemTime()
{
	for(int i = 0; i < DB_MAX_DECORATE_BAG;i++)
	{
		CheckDecorateItemTime((uint32_t)i);
	}

	return 0;
}