
#ifndef _DATASTORE_HPP_
#define _DATASTORE_HPP_

#include "../proto/DBmsg.pb.h"

//role类模块不需要声明结构体

//声明所有模块的数据结构，需要保存的数据用一个结构体来声明.每个结构体都对应一个DBmsg文件的结构

//DBmsg文件的结构都是做该结构体ST2PB和PB2ST函数的函数参数，每个声明的结构都必须要有ST2PB函数和PB2ST函数

//ST2PB表示把数据存到数据库的操作，PB2ST表示从数据库取数据出来

//背包模块保存的数据

const int DB_MAX_BAG_GRID_NUM = 100;			//最多只有100种物品

typedef struct tagDBBagGrid
{
	uint32_t m_uiID; // Item ID
	uint32_t m_uiNum; // Item Num

	int ST2PB(DBBagGrid& msg);
	int PB2ST(const DBBagGrid& msg);
	static int Compare(const void* p1, const void* p2);
} TDBBagGrid;

typedef struct tagDBBagGridList
{
	int16_t m_nGridsRef; // Grids's ref
	TDBBagGrid m_astGrids[DB_MAX_BAG_GRID_NUM]; // Grids

	int ST2PB(DBBagGridList& msg);
	int PB2ST(const DBBagGridList& msg);
} TDBBagGridList;

typedef struct tagDBBagInfo
{
	TDBBagGridList m_stGridList; // CridList

	int ST2PB(DBBagInfo& msg);
	int PB2ST(const DBBagInfo& msg);
} TDBBagInfo;


//背包模块保存的数据


#endif // !_DATASTORE_HPP_
