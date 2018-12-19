
#ifndef _DATASTORE_HPP_
#define _DATASTORE_HPP_

#include "../proto/DBmsg.pb.h"

//role类模块不需要声明结构体

//声明所有模块的数据结构，需要保存的数据用一个结构体来声明.每个结构体都对应一个DBmsg文件的结构

//DBmsg文件的结构都是做该结构体ST2PB和PB2ST函数的函数参数，每个声明的结构都必须要有ST2PB函数和PB2ST函数

//ST2PB表示把数据存到数据库的操作，PB2ST表示从数据库取数据出来

//////////////////////////////////////////////////////////////背包模块保存的数据////////////////////////////////////////////////////

const int DB_MAX_BAG_GRID_NUM = 100;			//最多只有100种物品id

const int DB_MAX_DECORATE_BAG = 30;				//最多的装饰背包种类只有30种

const int DB_MAX_DECORATE_BAG_NUM = 100;		//每个种类的装饰背包的ID最多有 100种装饰id

const int DB_MAX_VIP = 3;						//最多只有三种类型VIP（红，黄，紫色）


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


//////////////////////////////////////////////////////////////////////背包模块保存的数据/////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////装饰背包模块保存的数据/////////////////////////////////////////////////

typedef struct tagDBDecorateItem
{
	uint32_t m_uiID;		
	uint32_t m_uiType; 
	uint64_t m_uiEndTime;

	int ST2PB(DBDecorateItem& msg);
	int PB2ST(const DBDecorateItem& msg);
	static int Compare(const void* p1, const void* p2);
} TDBDecorateItem;

typedef struct tagDBDecorateItemList
{
	int16_t m_nGridsRef;											// 真正存在的ID的总数
	TDBDecorateItem m_astDecorateGrids[DB_MAX_DECORATE_BAG_NUM];	// 最大存放的个数最大为100个装饰ID

	int ST2PB(DBDecorateItemList& msg);
	int PB2ST(const DBDecorateItemList& msg);
} TDBDecorateItemList;

typedef struct tagDBDecorateBagInfo
{
	TDBDecorateItemList m_stBagGridList; // CridList

	int ST2PB(DBDecorateBagInfo& msg);
	int PB2ST(const DBDecorateBagInfo& msg);
} TDBDecorateBagInfo;

typedef struct tagDBDecorateBagInfoList
{
	int16_t m_nGridsRef;											// 真正存在的装饰背包类型的总数
	TDBDecorateBagInfo m_astDecorateBagList[DB_MAX_DECORATE_BAG];	// 最大装饰背包类型个数最大为30个装饰不同种类的背包

	int ST2PB(DBDecorateBagInfoList& msg);
	int PB2ST(const DBDecorateBagInfoList& msg);
} TDBDecorateBagInfoList;


typedef struct tagDBDecorateBagModuleInfo
{
	TDBDecorateBagInfoList m_stDecorateBagInfoList; // CridList
	uint32_t m_astTypeList[DB_MAX_DECORATE_BAG];	//每个类型背包都会有一个设置默认值

	int ST2PB(DBDecorateBagModuleInfo& msg);
	int PB2ST(const DBDecorateBagModuleInfo& msg);
} TDBDecorateBagModuleInfo;

typedef struct tagDBDecorateBagVIPItem
{
	uint32_t m_uiGrade;			// VIP等级
	uint32_t m_uiExper;			// VIP经验值

	int ST2PB(DBDecorateBagVIPItem& msg);
	int PB2ST(const DBDecorateBagVIPItem& msg);
} TDBDecorateBagVIPItem;

typedef struct tagDBDecorateBagVIPList
{
	TDBDecorateBagVIPItem m_astVipGrids[DB_MAX_VIP]; // VIP 数组

	int ST2PB(DBDecorateBagVIPList& msg);
	int PB2ST(const DBDecorateBagVIPList& msg);
} TDBDecorateBagVIPList;

typedef struct tagDBDecorateBagVIPInfo
{
	TDBDecorateBagVIPList m_stVipGridList; // CridList

	int ST2PB(DBDecorateBagVIPInfo& msg);
	int PB2ST(const DBDecorateBagVIPInfo& msg);
} TDBDecorateBagVIPInfo;



//////////////////////////////////////////////////////////////////////装饰背包模块保存的数据/////////////////////////////////////////////////


#endif // !_DATASTORE_HPP_
