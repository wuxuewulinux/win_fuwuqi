
#ifndef _DATASTORE_HPP_
#define _DATASTORE_HPP_

#include "../proto/DBmsg.pb.h"

//role��ģ�鲻��Ҫ�����ṹ��

//��������ģ������ݽṹ����Ҫ�����������һ���ṹ��������.ÿ���ṹ�嶼��Ӧһ��DBmsg�ļ��Ľṹ

//DBmsg�ļ��Ľṹ�������ýṹ��ST2PB��PB2ST�����ĺ���������ÿ�������Ľṹ������Ҫ��ST2PB������PB2ST����

//ST2PB��ʾ�����ݴ浽���ݿ�Ĳ�����PB2ST��ʾ�����ݿ�ȡ���ݳ���

//����ģ�鱣�������

const int DB_MAX_BAG_GRID_NUM = 100;			//���ֻ��100����Ʒ

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


//����ģ�鱣�������


#endif // !_DATASTORE_HPP_
