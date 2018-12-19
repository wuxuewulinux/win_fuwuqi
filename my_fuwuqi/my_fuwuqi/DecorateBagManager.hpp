
#ifndef _DECORATEBAGMANAGER_HPP_
#define _DECORATEBAGMANAGER_HPP_

#include "DataStore.hpp"
#include "../proto/DBmsg.pb.h"


class DecorateBagManager
{
public:
	DecorateBagManager();
	~DecorateBagManager();

	int Init();

	int SetDecorateBagInfo( const DBDecorateBagModuleInfo* pDecorateBagInfo );
	int GenDBDecorateBagInfo( DBDecorateBagModuleInfo& rDecorateBagInfo );

	int SetDecorateVIPBagInfo( const DBDecorateBagVIPInfo* pDecorateVIPBagInfo );
	int GenDBDecorateVIPBagInfo( DBDecorateBagVIPInfo& rDecorateVIPBagInfo );

	//TDBDecorateItem * GetDecorateGridByIdx( uint32_t uiIdx ,uint32_t uiType);				//按数组的下标去找出一个某个装饰背包中的装饰物品出来

	TDBDecorateItem * GetDecorateGridByID( uint32_t uiId ,uint32_t uiType);					//按装饰背包物品ID查找出该物品出来，没有就返回NULL
	TDBDecorateItemList * GetDecorateGridList(uint32_t uiType);								//获取某个种类的装饰背包数组。
	TDBDecorateItem * AddDecorateGrid( uint32_t uiId, uint32_t uiType,long rEndTime );		//增加装饰背包物品到对应的背包,相当于人物身上拥有该装饰物品
	int DelDecorateGrid( uint32_t uiItemId,uint32_t uiType );								//从某个装饰背包移除装饰物品。就是说该种类装饰背包没有该物品了

	TDBDecorateBagVIPInfo * GetDecorateBagVIPInfo(){return &rBagVIPInfo;}

	//对物品操作进行括展

	int AddDecorateItem(uint32_t uiId, uint32_t uiType,long rEndTime);		//增加装饰物品到人物身上，获得该装饰物品

	//该函数返回的是装饰背包默认设置的装饰ID物品，只要用到装饰ID物品就会调用该函数
	uint32_t CheckDecorateItemTime(uint32_t uiType);								//检查某个种类装饰背包的所有装饰物品时间(查看哪些装饰物品是否过时)

	uint64_t GetDecorateItemTime(uint32_t uiId, uint32_t uiType);			//获取该装饰物品的结束时间

	int SetDecorateItemShowBag(uint32_t uiId, uint32_t uiType);				//设置装饰背包的显示默认值。

	int AllCheckDecorateItemTime();											//刷新所有装饰背包时间



private:

TDBDecorateBagModuleInfo rBagModuleInfo;

TDBDecorateBagVIPInfo rBagVIPInfo;									//保存VIP三种类型的等级和经验值

};





#endif