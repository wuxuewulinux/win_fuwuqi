#ifndef __BABMANAGER_HPP__
#define __BABMANAGER_HPP__

#include "DataStore.hpp"
#include "../proto/DBmsg.pb.h"

//注: 背包的展示和游戏所有物品的操作都在该类中封装好了，钻石和积分也属于物品

//注：只有某些物品才能显示在背包中，像钻石和积分就不能显示在背包中，所有一定要有一张配置表来区别他们

class CBagManager
{
public:
	CBagManager();
	~CBagManager();

public:
	int Init();

	int SetBagInfo( const DBBagInfo* pBagInfo );
	int GenDBBagInfo( DBBagInfo& rBagInfo );
	
	int GetGridNum();												//获取人物身上有多少种物品
	int SetGridNum( uint32_t uiNum );
	
	TDBBagGrid * GetGridByID( uint32_t uiItemId );					//按物品ID查找出该物品出来，没有就返回NULL
	TDBBagGrid * GetGridByIdx( uint32_t uiIdx );					//按数组的下标去找出一个物品出来
	TDBBagGridList & GetGridList( );								//获取物品数组。
	
	TDBBagGrid * AddGrid( uint32_t uiItemId, uint32_t uiNum );		//增加物品到人物身上,相当于人物身上拥有该物品
	int DelGrid( uint32_t uiItemId );								//从人物身上移除物品。就是说身上没有该物品了

	//对物品操作进行括展

	int AddItem(uint32_t uiItemId, uint32_t uiNum);					//增加物品到人物身上，获得物品	

	int DelItem(uint32_t uiItemId, uint32_t uiNum);					//从人物身上删除物品的数量

	int GetItemNum(uint32_t uiItemId);									//获取该物品的数量


private:
	TDBBagInfo m_stBagInfo;
};

#endif // !__BABMANAGER_HPP__
