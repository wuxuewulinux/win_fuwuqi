#ifndef __BABMANAGER_HPP__
#define __BABMANAGER_HPP__

#include "DataStore.hpp"
#include "../proto/DBmsg.pb.h"

//ע: ������չʾ����Ϸ������Ʒ�Ĳ������ڸ����з�װ���ˣ���ʯ�ͻ���Ҳ������Ʒ

//ע��ֻ��ĳЩ��Ʒ������ʾ�ڱ����У�����ʯ�ͻ��־Ͳ�����ʾ�ڱ����У�����һ��Ҫ��һ�����ñ�����������

class CBagManager
{
public:
	CBagManager();
	~CBagManager();

public:
	int Init();

	int SetBagInfo( const DBBagInfo* pBagInfo );
	int GenDBBagInfo( DBBagInfo& rBagInfo );
	
	int GetGridNum();												//��ȡ���������ж�������Ʒ
	int SetGridNum( uint32_t uiNum );
	
	TDBBagGrid * GetGridByID( uint32_t uiItemId );					//����ƷID���ҳ�����Ʒ������û�оͷ���NULL
	TDBBagGrid * GetGridByIdx( uint32_t uiIdx );					//��������±�ȥ�ҳ�һ����Ʒ����
	TDBBagGridList & GetGridList( );								//��ȡ��Ʒ���顣
	
	TDBBagGrid * AddGrid( uint32_t uiItemId, uint32_t uiNum );		//������Ʒ����������,�൱����������ӵ�и���Ʒ
	int DelGrid( uint32_t uiItemId );								//�����������Ƴ���Ʒ������˵����û�и���Ʒ��

	//����Ʒ����������չ

	int AddItem(uint32_t uiItemId, uint32_t uiNum);					//������Ʒ���������ϣ������Ʒ	

	int DelItem(uint32_t uiItemId, uint32_t uiNum);					//����������ɾ����Ʒ������

	int GetItemNum(uint32_t uiItemId);									//��ȡ����Ʒ������


private:
	TDBBagInfo m_stBagInfo;
};

#endif // !__BABMANAGER_HPP__
