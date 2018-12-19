
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



int DecorateBagWork::GenCSBagInfo( CRoleObj * pRoleObj,const CSDecorateBagFetchReq& rFetchReq, CSDecorateBagInfo & rCSBagInfo,CSDecorateBagFetchRsp* Rsp)
{
	HANDCHECH_P( pRoleObj, -1 );
	DecorateBagManager & rBagMgr = pRoleObj->GetDecorateBagMgr();
	TDBDecorateItemList * rTGridList = rBagMgr.GetDecorateGridList(rFetchReq.type());	//��ȡ��Ӧ����װ�α���
	HANDCHECH_P( rTGridList, -2 );
	Rsp->set_id(rBagMgr.CheckDecorateItemTime(rFetchReq.type()));						//�ȶԸ����ͱ��������д��ڵ�װ����Ʒ��ʱ����м��
	for ( int i=0 ; i< rTGridList->m_nGridsRef ; ++i )
	{   
		CSDecorateBagGridInfo * pCSGridInfo = rCSBagInfo.add_decorategridinfo();
		GenCSGrid( rTGridList->m_astDecorateGrids[i], *pCSGridInfo );
	} 

	return 0;
}


int DecorateBagWork::OnBagShowSet(CRoleObj * pRoleObj,const CSDecorateBagShowSetReq & req)
{
	HANDCHECH_P( pRoleObj, -1 );
	pRoleObj->GetDecorateBagMgr().SetDecorateItemShowBag(req.id(),req.type());
	return 0;
}


int DecorateBagWork::GenCSVIPBagInfo(CRoleObj * pRoleObj,const CSDecorateBagVIPFetchReq& rReq, CSDecorateBagVIPFetchRsp & rRsp)
{
	if (rReq.type() != 1)
	{
		return -1;
	}
	HANDCHECH_P( pRoleObj, -1 );
	DecorateBagManager & rBagMgr = pRoleObj->GetDecorateBagMgr();
	TDBDecorateItemList * rTGridList = rBagMgr.GetDecorateGridList(rReq.type());	//��ȡ��Ӧ����װ�α���
	HANDCHECH_P( rTGridList, -2 );
	TDBDecorateBagVIPInfo * pVipInfo = rBagMgr.GetDecorateBagVIPInfo();
	HANDCHECH_P(pVipInfo,-4);
	rRsp.set_id(rBagMgr.CheckDecorateItemTime(rReq.type()));						//�ȶԸ����ͱ��������д��ڵ�װ����Ʒ��ʱ����м��
	CSDecorateBagInfo* pCSBagInfo = rRsp.mutable_decoratebaginfo();
	HANDCHECH_P( pCSBagInfo, -1 );
	for ( int i=0 ; i< rTGridList->m_nGridsRef ; ++i )
	{   
		CSDecorateBagGridInfo * pCSGridInfo = pCSBagInfo->add_decorategridinfo();
		GenCSGrid( rTGridList->m_astDecorateGrids[i], *pCSGridInfo );
	} 
	//������VIP�ĵȼ��;���ȫ�����ͻ���
	CSDecorateBagVIPList* pVipList = rRsp.mutable_viplist();
	HANDCHECH_P( pVipList, -1 );
	for ( int i=0 ; i< DB_MAX_VIP ; ++i )
	{   
		CSDecorateBagVIPItem * pVIPGridInfo = pVipList->add_vipitemlist();
		HANDCHECH_P( pVIPGridInfo, -1 );
		pVIPGridInfo->set_vipgrade(pVipInfo->m_stVipGridList.m_astVipGrids[i].m_uiGrade);
		pVIPGridInfo->set_vipexper(pVipInfo->m_stVipGridList.m_astVipGrids[i].m_uiExper);
	} 

	return 0;
}