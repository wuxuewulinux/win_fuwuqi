
#ifndef _MATEWORK_HPP_
#define _MATEWORK_HPP_

#include "../proto/CSmsg.pb.h"
#include "quanju.hpp"

class MateWork
{
public:
	MateWork();
	~MateWork();

	static int MateFetch(const CSMateFetchReq& rReq);

	static int SendMateFetch(int SockIo,uint64_t Uid);						//广播弹出成功匹配界面

	static int SendMateNotSuccess(int SockIo,uint64_t Uid);					//广播弹出成功匹配界面但是有人不点击同意导致不能开始游戏

	static int SendMateNotUser(int SockIo,uint64_t Uid);					//发送该协议给哪些没有点击同意让时间超时的玩家，让他们的客户端界面关闭掉匹配状态.

	static int QuitMateFetch(const CSQuitMateFetchReq& rReq);				//玩家在匹配中，然后他突然点击退出

	static int NotButtonMateFetch(const CSNotButtonMateFetchReq& rReq);		//成功匹配够人数弹出同意和拒绝按钮，但是玩家选择拒绝按钮

	static int SuccessButtonMateFetch(const CSSuccessButtonMateFetchReq& rReq);	//成功匹配够人数弹出同意和拒绝按钮，但是玩家选择同意按钮

	static int EnterHeroShow(Room* PRoom,int iDeleteRoomIndex);						//执行进入选英雄界面的操作

	static int SendHeroInfo(CRoleObj* pRoleObj,int SockIo,uint64_t uUid,Room* pRoom);				//发送所在英雄界面的玩家数据给客户端

	static int EnterHeroShowBag(const CSEnterHeroShowBagReq& rReq);					//根据装饰背包选择装饰背包的ID物品作为显示，除炸弹背包以外

	static int ShowZhaDanBag(const CSShowZhaDanBagReq& rReq,CSShowZhaDanBagRsp* pRsp);						//玩家选择炸弹

	static int SendUserShowZhaDan(CRoleObj* pRoleObj,uint64_t UserUid,uint64_t Uid,uint32_t ZhaDanId);		//广播给所有玩家该玩家选择该炸弹了

private:

};


#endif