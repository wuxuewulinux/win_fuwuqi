
#ifndef _ROLEOBJ_HPP_
#define _ROLEOBJ_HPP_

#include "../proto/CSmsg.pb.h"
#include "../proto/DBmsg.pb.h"
#include <string>
#include "BagManager.hpp"
#include "DecorateBagManager.hpp"

class CRoleObj
{
public:
	CRoleObj(){}
	~CRoleObj(){}

	
	int Init();			//初始化所有模块数据
	int SetRoleInfo(const DBRoleInfo& rRoleInfo);			//从数据库拿到role结构然后给各个模块开始设值
	int GenDBRoleInfo(DBRoleInfo* pRoleInfo);				//把各个模块的数值设置到role结构中，发给数据库存储


public:

	void SetFd(int iFd) { m_iFd = iFd; }
	int GetFd() { return m_iFd; }

	void SetUid(uint64_t u_Uid){uUID = u_Uid;}
	uint64_t GetUid(){return uUID;}

	void SetName(const std::string& m_Name){sName = m_Name;}
	std::string& GetName(){return sName;}

	void SetLevel(uint32_t m_Level){uiLevel = m_Level;}
	uint32_t GetLevel(){return uiLevel;}

	void SetRand(uint32_t m_Rand){uiRank = m_Rand;}
	uint32_t GetRand(){return uiRank;}

	void SetRandExper(uint32_t m_uiRandExper){uiRankExper = m_uiRandExper;}
	uint32_t GetRandExper(){return uiRankExper;}

	void SetLevelExper(uint32_t m_uiLevelExper){uiLevelExper = m_uiLevelExper;}
	uint32_t GetLevelExper(){return uiLevelExper;}

public:

	CBagManager & GetBagMgr(){ return m_oBagMgr; }
	DecorateBagManager & GetDecorateBagMgr(){return m_oDecorateBagMgr;}

private:

	int m_iFd;												//套接字IO	

	uint64_t uUID;											//唯一uid

	std::string sName;										//游戏名称

	uint32_t uiLevel;										//游戏等级

	uint32_t uiRank;										//段位级别

	uint32_t uiRankExper;									//该段位经验

	uint32_t uiLevelExper;									//人物等级经验

	//添加各个模块的类对象在这里

	CBagManager	m_oBagMgr;									//背包模块
	DecorateBagManager m_oDecorateBagMgr;					//装饰背包模块
};

#endif