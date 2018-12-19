
#ifndef _DECORATEBAGWORK_HPP_
#define _DECORATEBAGWORK_HPP_

#include "RoleObj.hpp"
#include "../proto/CSmsg.pb.h"

class DecorateBagWork
{
public:
	static int GenCSGrid( TDBDecorateItem & rTGrid, CSDecorateBagGridInfo & rCSGrid );
	static int GenCSBagInfo( CRoleObj * pRoleObj,const CSDecorateBagFetchReq& rFetchReq, CSDecorateBagInfo & rCSBagInfo,CSDecorateBagFetchRsp* Rsp);

	static int OnBagShowSet(CRoleObj * pRoleObj,const CSDecorateBagShowSetReq & req);

	static int GenCSVIPBagInfo(CRoleObj * pRoleObj,const CSDecorateBagVIPFetchReq& rReq, CSDecorateBagVIPFetchRsp & rRsp);

};	


#endif