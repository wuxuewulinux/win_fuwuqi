
#ifndef _DATASTORE_HPP_
#define _DATASTORE_HPP_

#include "../proto/DBmsg.pb.h"

//role��ģ�鲻��Ҫ�����ṹ��

//��������ģ������ݽṹ����Ҫ�����������һ���ṹ��������.ÿ���ṹ�嶼��Ӧһ��DBmsg�ļ��Ľṹ

//DBmsg�ļ��Ľṹ�������ýṹ��ST2PB��PB2ST�����ĺ���������ÿ�������Ľṹ������Ҫ��ST2PB������PB2ST����

//ST2PB��ʾ�����ݴ浽���ݿ�Ĳ�����PB2ST��ʾ�����ݿ�ȡ���ݳ���

//////////////////////////////////////////////////////////////����ģ�鱣�������////////////////////////////////////////////////////

const int DB_MAX_BAG_GRID_NUM = 100;			//���ֻ��100����Ʒid

const int DB_MAX_DECORATE_BAG = 30;				//����װ�α�������ֻ��30��

const int DB_MAX_DECORATE_BAG_NUM = 100;		//ÿ�������װ�α�����ID����� 100��װ��id

const int DB_MAX_VIP = 3;						//���ֻ����������VIP���죬�ƣ���ɫ��


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


//////////////////////////////////////////////////////////////////////����ģ�鱣�������/////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////װ�α���ģ�鱣�������/////////////////////////////////////////////////

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
	int16_t m_nGridsRef;											// �������ڵ�ID������
	TDBDecorateItem m_astDecorateGrids[DB_MAX_DECORATE_BAG_NUM];	// ����ŵĸ������Ϊ100��װ��ID

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
	int16_t m_nGridsRef;											// �������ڵ�װ�α������͵�����
	TDBDecorateBagInfo m_astDecorateBagList[DB_MAX_DECORATE_BAG];	// ���װ�α������͸������Ϊ30��װ�β�ͬ����ı���

	int ST2PB(DBDecorateBagInfoList& msg);
	int PB2ST(const DBDecorateBagInfoList& msg);
} TDBDecorateBagInfoList;


typedef struct tagDBDecorateBagModuleInfo
{
	TDBDecorateBagInfoList m_stDecorateBagInfoList; // CridList
	uint32_t m_astTypeList[DB_MAX_DECORATE_BAG];	//ÿ�����ͱ���������һ������Ĭ��ֵ

	int ST2PB(DBDecorateBagModuleInfo& msg);
	int PB2ST(const DBDecorateBagModuleInfo& msg);
} TDBDecorateBagModuleInfo;

typedef struct tagDBDecorateBagVIPItem
{
	uint32_t m_uiGrade;			// VIP�ȼ�
	uint32_t m_uiExper;			// VIP����ֵ

	int ST2PB(DBDecorateBagVIPItem& msg);
	int PB2ST(const DBDecorateBagVIPItem& msg);
} TDBDecorateBagVIPItem;

typedef struct tagDBDecorateBagVIPList
{
	TDBDecorateBagVIPItem m_astVipGrids[DB_MAX_VIP]; // VIP ����

	int ST2PB(DBDecorateBagVIPList& msg);
	int PB2ST(const DBDecorateBagVIPList& msg);
} TDBDecorateBagVIPList;

typedef struct tagDBDecorateBagVIPInfo
{
	TDBDecorateBagVIPList m_stVipGridList; // CridList

	int ST2PB(DBDecorateBagVIPInfo& msg);
	int PB2ST(const DBDecorateBagVIPInfo& msg);
} TDBDecorateBagVIPInfo;



//////////////////////////////////////////////////////////////////////װ�α���ģ�鱣�������/////////////////////////////////////////////////


#endif // !_DATASTORE_HPP_
