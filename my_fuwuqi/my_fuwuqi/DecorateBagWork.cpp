
#include "DecorateBagWork.hpp"
#include "quanju.hpp"
#include "DecorateBagManager.hpp"


int DecorateBagWork::GenCSGrid( TDBDecorateItem & rTGrid, CSDecorateBagGridInfo & rCSGrid )
{
	rCSGrid.set_itemid(rTGrid.m_uiID);
	rCSGrid.set_type(rTGrid.m_uiType);
	rCSGrid.set_endtime(rTGrid.m_uiEndTime);
	return 0;
}



int DecorateBagWork::GenCSBagInfo( CRoleObj * pRoleObj,const CSDecorateBagFetchReq& rFetchReq, CSDecorateBagInfo & rCSBagInfo)
{
	HANDCHECH_P( pRoleObj, -1 );
	DecorateBagManager & rBagMgr = pRoleObj->GetDecorateBagMgr();
	TDBDecorateItemList * rTGridList = rBagMgr.GetDecorateGridList(rFetchReq.type());	//��ȡ��Ӧ����װ�α���
	HANDCHECH_P( rTGridList, -2 );
	rBagMgr.CheckDecorateItemTime(rFetchReq.type());									//�ȶԸ����ͱ��������д��ڵ�װ����Ʒ��ʱ����м��
	for ( int i=0 ; i< rTGridList->m_nGridsRef ; ++i )
	{   
		CSDecorateBagGridInfo * pCSGridInfo = rCSBagInfo.add_decorategridinfo();
		GenCSGrid( rTGridList->m_astDecorateGrids[i], *pCSGridInfo );
	} 

	return 0;
}
