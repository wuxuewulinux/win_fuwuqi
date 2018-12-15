
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

	//TDBDecorateItem * GetDecorateGridByIdx( uint32_t uiIdx ,uint32_t uiType);				//��������±�ȥ�ҳ�һ��ĳ��װ�α����е�װ����Ʒ����

	TDBDecorateItem * GetDecorateGridByID( uint32_t uiId ,uint32_t uiType);					//��װ�α�����ƷID���ҳ�����Ʒ������û�оͷ���NULL
	TDBDecorateItemList * GetDecorateGridList(uint32_t uiType);								//��ȡĳ�������װ�α������顣
	TDBDecorateItem * AddDecorateGrid( uint32_t uiId, uint32_t uiType,long rEndTime );		//����װ�α�����Ʒ����Ӧ�ı���,�൱����������ӵ�и�װ����Ʒ
	int DelDecorateGrid( uint32_t uiItemId,uint32_t uiType );								//��ĳ��װ�α����Ƴ�װ����Ʒ������˵������װ�α���û�и���Ʒ��

	//����Ʒ����������չ

	int AddDecorateItem(uint32_t uiId, uint32_t uiType,long rEndTime);		//����װ����Ʒ���������ϣ���ø�װ����Ʒ	

	int CheckDecorateItemTime(uint32_t uiType);								//���ĳ������װ�α���������װ����Ʒʱ��(�鿴��Щװ����Ʒ�Ƿ��ʱ)

	uint64_t GetDecorateItemTime(uint32_t uiId, uint32_t uiType);			//��ȡ��װ����Ʒ�Ľ���ʱ��

private:

TDBDecorateBagModuleInfo rBagModuleInfo;

};





#endif