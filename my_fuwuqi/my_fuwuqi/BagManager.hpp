#ifndef __BABMANAGER_HPP__
#define __BABMANAGER_HPP__

#include "DataStore.hpp"
#include "../proto/DBmsg.pb.h"

class CBagManager
{
public:
	CBagManager();
	~CBagManager();

public:
	int Init();

	int SetBagInfo( const DBBagInfo* pBagInfo );
	int GenDBBagInfo( DBBagInfo& rBagInfo );
	
	int GetGridNum();
	int SetGridNum( uint32_t uiNum );
	
	TDBBagGrid * GetGridByID( uint32_t uiItemId );
	TDBBagGrid * GetGridByIdx( uint32_t uiIdx );
	TDBBagGridList & GetGridList( );
	
	TDBBagGrid * AddGrid( uint32_t uiItemId, uint32_t uiNum );
	int DelGrid( uint32_t uiItemId );
private:
	TDBBagInfo m_stBagInfo;
};

#endif // !__BABMANAGER_HPP__
