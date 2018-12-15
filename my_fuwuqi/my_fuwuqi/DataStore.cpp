
#include "DataStore.hpp"


//定义所有模块结构体的PB2ST 和 ST2PB函数


/////////////////////////////////背包模块保存的数据///////////////////////////////////////////////////////

int tagDBBagGrid::ST2PB(DBBagGrid& msg)
{
	msg.set_id(m_uiID);
	msg.set_num(m_uiNum);
	return 0;
}

int tagDBBagGrid::PB2ST(const DBBagGrid& msg)
{
	if (msg.has_id())
	{
		m_uiID = msg.id();
	}
	if (msg.has_num())
	{
		m_uiNum = msg.num();
	}
	return 0;
}

int tagDBBagGrid::Compare(const void* p1, const void* p2)
{
	return ((const tagDBBagGrid*)p1)->m_uiID - ((const tagDBBagGrid*)p2)->m_uiID;
}

int tagDBBagGridList::ST2PB(DBBagGridList& msg)
{
	for (int i = 0; i < m_nGridsRef && i < DB_MAX_BAG_GRID_NUM; ++i)
	{
		DBBagGrid* sub = msg.add_grids();
		m_astGrids[i].ST2PB(*sub);
	}

	return 0;
}

int tagDBBagGridList::PB2ST(const DBBagGridList& msg)
{
	m_nGridsRef = msg.grids_size();
	for (int i = 0; i < m_nGridsRef && i < DB_MAX_BAG_GRID_NUM ; ++i)
	{
		m_astGrids[i].PB2ST(msg.grids(i));
	}
	m_nGridsRef = m_nGridsRef <=  DB_MAX_BAG_GRID_NUM ? m_nGridsRef : DB_MAX_BAG_GRID_NUM;
	return 0;
}

int tagDBBagInfo::ST2PB(DBBagInfo& msg)
{
	DBBagGridList* gridlist = msg.mutable_gridlist();
	m_stGridList.ST2PB(*gridlist);
	return 0;
}

int tagDBBagInfo::PB2ST(const DBBagInfo& msg)
{
	m_stGridList.PB2ST(msg.gridlist());
	return 0;
}

/////////////////////////////////背包模块保存的数据/////////////////////////////////////////////////////



////////////////////////////////装饰背包模块保存的数据/////////////////////////////////////////////////

int tagDBDecorateItem::Compare(const void* p1, const void* p2)
{
	return ((const tagDBDecorateItem*)p1)->m_uiID - ((const tagDBDecorateItem*)p2)->m_uiID;
}

int tagDBDecorateItem::ST2PB(DBDecorateItem& msg)
{
	msg.set_id(m_uiID);
	msg.set_type(m_uiType);
	msg.set_endtime(m_uiEndTime);

	return 0;
}


int tagDBDecorateItem::PB2ST(const DBDecorateItem& msg)
{
	if (msg.has_id())
	{
		m_uiID = msg.id();
	}
	if (msg.has_type())
	{
		m_uiType = msg.type();
	}
	if (msg.has_endtime())
	{
		m_uiEndTime = msg.endtime();
	}

	return 0;
}


int tagDBDecorateItemList::ST2PB(DBDecorateItemList& msg)
{
	for (int i = 0; i < m_nGridsRef && i < DB_MAX_DECORATE_BAG_NUM; ++i)
	{
		DBDecorateItem* sub = msg.add_decorategrids();
		m_astDecorateGrids[i].ST2PB(*sub);
	}

	return 0;
}
int tagDBDecorateItemList::PB2ST(const DBDecorateItemList& msg)
{
	m_nGridsRef = msg.decorategrids_size();
	for (int i = 0; i < m_nGridsRef && i < DB_MAX_DECORATE_BAG_NUM ; ++i)
	{
		m_astDecorateGrids[i].PB2ST(msg.decorategrids(i));
	}
	m_nGridsRef = m_nGridsRef <=  DB_MAX_DECORATE_BAG_NUM ? m_nGridsRef : DB_MAX_DECORATE_BAG_NUM;

	return 0;
}


int tagDBDecorateBagInfo::ST2PB(DBDecorateBagInfo& msg)
{
	DBDecorateItemList* baggridlist = msg.mutable_baggridlist();
	m_stBagGridList.ST2PB(*baggridlist);

	return 0;
}
int tagDBDecorateBagInfo::PB2ST(const DBDecorateBagInfo& msg)
{
	m_stBagGridList.PB2ST(msg.baggridlist());
	return 0;
}

int tagDBDecorateBagInfoList::ST2PB(DBDecorateBagInfoList& msg)
{
	for (int i = 0; i < DB_MAX_DECORATE_BAG; ++i)
	{
		DBDecorateBagInfo* sub = msg.add_decoratebaglist();
		m_astDecorateBagList[i].ST2PB(*sub);
	}
	return 0;
}

int tagDBDecorateBagInfoList::PB2ST(const DBDecorateBagInfoList& msg)
{
	m_nGridsRef = msg.decoratebaglist_size();
	for (int i = 0;i < m_nGridsRef && i < DB_MAX_DECORATE_BAG ; ++i)
	{
		m_astDecorateBagList[i].PB2ST(msg.decoratebaglist(i));
	}
	m_nGridsRef = m_nGridsRef <=  DB_MAX_DECORATE_BAG ? m_nGridsRef : DB_MAX_DECORATE_BAG;

	return 0;	
}


int tagDBDecorateBagModuleInfo::ST2PB(DBDecorateBagModuleInfo& msg)
{
	DBDecorateBagInfoList* DecorateBagInfoList = msg.mutable_decoratebaginfolist();
	m_stDecorateBagInfoList.ST2PB(*DecorateBagInfoList);
	return 0;
}
int tagDBDecorateBagModuleInfo::PB2ST(const DBDecorateBagModuleInfo& msg)
{
	m_stDecorateBagInfoList.PB2ST(msg.decoratebaginfolist());
	return 0;
}


////////////////////////////////装饰背包模块保存的数据/////////////////////////////////////////////////
