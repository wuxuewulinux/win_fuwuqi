
#ifndef _TIMEEVENT_HPP_
#define _TIMEEVENT_HPP_

//该文件是封装所有定时器触发事件时执行的函数

void Time_MateFetch(void * Data);			//匹配界面触发。当弹出匹配成功进入游戏，10秒内有玩家不点击同意就会触发事件

void Time_MateShowHeroFetch(void * Data);	//英雄选择界面，规定30秒时间，30秒内触发事件进入场景

int SendHeroEnterFight(CRoleObj* pRoleObj,int SockIo,uint64_t uUid,ShowHeroRoom* pRoom);

#endif