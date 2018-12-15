
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

	
	int Init();			//��ʼ������ģ������
	int SetRoleInfo(const DBRoleInfo& rRoleInfo);			//�����ݿ��õ�role�ṹȻ�������ģ�鿪ʼ��ֵ
	int GenDBRoleInfo(DBRoleInfo* pRoleInfo);				//�Ѹ���ģ�����ֵ���õ�role�ṹ�У��������ݿ�洢


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

	int m_iFd;												//�׽���IO	

	uint64_t uUID;											//Ψһuid

	std::string sName;										//��Ϸ����

	uint32_t uiLevel;										//��Ϸ�ȼ�

	uint32_t uiRank;										//��λ����

	uint32_t uiRankExper;									//�ö�λ����

	uint32_t uiLevelExper;									//����ȼ�����

	//��Ӹ���ģ��������������

	CBagManager	m_oBagMgr;									//����ģ��
	DecorateBagManager m_oDecorateBagMgr;					//װ�α���ģ��
};

#endif