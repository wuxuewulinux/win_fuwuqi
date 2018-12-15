
#ifndef _DECORATEBAGWORK_HPP_
#define _DECORATEBAGWORK_HPP_

#include "RoleObj.hpp"
#include "../proto/CSmsg.pb.h"

class DecorateBagWork
{
public:
	static int GenCSGrid( TDBDecorateItem & rTGrid, CSDecorateBagGridInfo & rCSGrid );
	static int GenCSBagInfo( CRoleObj * pRoleObj,const CSDecorateBagFetchReq& rFetchReq, CSDecorateBagInfo & rCSBagInfo);

};	


#endif