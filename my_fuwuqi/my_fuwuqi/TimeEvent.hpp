
#ifndef _TIMEEVENT_HPP_
#define _TIMEEVENT_HPP_

//���ļ��Ƿ�װ���ж�ʱ�������¼�ʱִ�еĺ���

void Time_MateFetch(void * Data);			//ƥ����津����������ƥ��ɹ�������Ϸ��10��������Ҳ����ͬ��ͻᴥ���¼�

void Time_MateShowHeroFetch(void * Data);	//Ӣ��ѡ����棬�涨30��ʱ�䣬30���ڴ����¼����볡��

int SendHeroEnterFight(CRoleObj* pRoleObj,int SockIo,uint64_t uUid,ShowHeroRoom* pRoom);

#endif