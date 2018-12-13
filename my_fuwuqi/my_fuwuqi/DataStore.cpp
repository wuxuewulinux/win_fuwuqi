
#include "DataStore.hpp"


//定义所有模块结构体的PB2ST 和 ST2PB函数
/*
int tagDBTreasureInfo::ST2PB(DBTreasureInfo& msg)
{
	DBTreasureList* treasurelist = msg.mutable_treasurelist();
	m_stTreasureList.ST2PB(*treasurelist);
	msg.set_lastfreetime(m_uiLastFreeTime);
	msg.set_lastastrologytime(m_uiLastAstrologyTime);
	msg.set_dailyastrologytimes(m_uiDailyAstrologyTimes);
	msg.set_totalastrologytimes(m_uiTotalAstrologyTimes);
	msg.set_haddrop200(m_ucHadDrop200);
	return 0;
}

int tagDBTreasureInfo::PB2ST(const DBTreasureInfo& msg)
{
	m_stTreasureList.PB2ST(msg.treasurelist());
	if (msg.has_lastfreetime())
	{
		m_uiLastFreeTime = msg.lastfreetime();
	}
	if (msg.has_lastastrologytime())
	{
		m_uiLastAstrologyTime = msg.lastastrologytime();
	}
	if (msg.has_dailyastrologytimes())
	{
		m_uiDailyAstrologyTimes = msg.dailyastrologytimes();
	}
	if (msg.has_totalastrologytimes())
	{
		m_uiTotalAstrologyTimes = msg.totalastrologytimes();
	}
	if (msg.has_haddrop200())
	{
		m_ucHadDrop200 = msg.haddrop200();
	}
	return 0;
}

*/

//背包模块保存的数据

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

//背包模块保存的数据