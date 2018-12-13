#ifndef __BAGFRAMEWORK_HPP__
#define __BAGFRAMEWORK_HPP__

#include "RoleObj.hpp"
#include "../proto/CSmsg.pb.h"

class CBagFramework
{
public:
	static int GenCSGrid( TDBBagGrid & rTGrid, CSBagGridInfo & rCSGrid );
	static int GenCSBagInfo( CRoleObj * pRoleObj, CSBagBagInfo & rBagInfo );

};	

#endif // !__BAGFRAMEWORK_HPP__

