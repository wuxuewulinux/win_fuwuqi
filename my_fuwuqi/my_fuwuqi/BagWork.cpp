
#include "BagWork.hpp"
#include "quanju.hpp"
#include "BagManager.hpp"

const uint32_t ItemNotBag = 500;			//����ʾ�ڱ�����Ʒ��IDȡֵ��ΧΪ 1 - 499. ��ʾ�ڱ�������ƷID��500��ʼ����

int CBagFramework::GenCSGrid( TDBBagGrid & rTGrid, CSBagGridInfo & rCSGrid )
{
	rCSGrid.set_itemid( rTGrid.m_uiID );
	rCSGrid.set_num( rTGrid.m_uiNum );

	return 0;
}

int CBagFramework::GenCSBagInfo( CRoleObj * pRoleObj, CSBagBagInfo & rCSBagInfo )
{
	HANDCHECH_P( pRoleObj, -1 );
	CBagManager & rBagMgr = pRoleObj->GetBagMgr();
	TDBBagGridList & rTGridList = rBagMgr.GetGridList();
	
	for ( int i=0 ; i< rTGridList.m_nGridsRef ; ++i )
	{   //���ڵ���500����Ʒ������ʾ�ڱ�����
		if (rTGridList.m_astGrids[i].m_uiID >= ItemNotBag)
		{
			CSBagGridInfo * pCSGridInfo = rCSBagInfo.add_gridinfo();
			GenCSGrid( rTGridList.m_astGrids[i], *pCSGridInfo );
		}	
	} 

	return 0;
} 

