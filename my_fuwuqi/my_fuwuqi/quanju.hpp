#ifndef _QUANJU_HPP_
#define _QUANJU_HPP_

//��ͷ�ļ�����洢��־�߳�����Ҫ��׼����Դ
//
#include <pthread.h>
#include <queue>
#include <unistd.h>
#include <iostream>
#include <map>
#include "readn.hpp"
#include "txtlog.hpp"
#include "timeku.hpp"
#include "../proto/CSmsg.pb.h"
#include "RoleObj.hpp"
#include <vector>
#include <stack>

//�ж�ָ��ĺ꺯��
#define HANDCHECH_P(a,b) \
if (a == NULL)\
{\
	return b;\
}\

#define HANDCHECH_I(a,b) \
	if (a < 0 )\
{\
	return b;\
}\



#define TIMESTAMP  get_time()/1000			//��ȡ��ǰʱ�䣬���ص�����Ϊ��λ

struct shuji
{

	int io;

	struct message mess;

};


struct MessageBody
{
	unsigned short length;                         //�����ݵĳ���

	char buff[1024];
};

struct sendshuji     //���л�����Ȼ�󱣴浽�̶߳������ڷ��͸��ͻ���
{

	int io;
	int len;
	struct MessageBody rMessageBody;

};

struct timeduilie       //��ʱ����Ҫ����Ľṹ��
{
	bool kaiguan;    //���trueΪ����ʱ�䣨push�����������falseΪȥ��ʱ�䣨pop������
	int index;
	int miao;
	int userid;
};

struct UidInfo
{
	uint64_t Uid;
	bool Button;				//falseΪû�е��ͬ�⣬trueΪ���ͬ�⡣��ʼ��Ϊfalse
};

struct Room
{
	int MaxCount;					//������������ǹ��������Ϳ�ʼ��Ϸ
	int UserCount;					//�ڸ÷�������
	int RoomIndex;					//�����
	int Agree;						//���ͬ�⿪ʼ��Ϸ��Agree��MaxCount�൱�Ϳ��Կ�ʼ��Ϸ
	int TimeIndex;					//ʱ���ڴ��±ꡣÿ�����䶼������һ��ʱ���ڴ档
	std::vector<UidInfo> UidList;	//���ڸ÷���ȴ���ʼ��Ϸ�ĳ�Ա
};

struct HeroVipInfo
{
	uint32_t VipId;
	uint32_t VipGreade;
	uint32_t VipExper;
};

struct HeroInfo
{
	uint64_t Uid;
	uint32_t PlaneId;		//�ɻ�ID
	uint32_t ZhaDanId;		//ը��ID
	uint32_t DiPanId;		//�ɻ�����ID
	uint32_t SkillOneID;	//��������1��ЧID
	uint32_t SkillTwoID;	//��������2��ЧID
	//ս����������Ҫ����
	HeroVipInfo VipList[3];			//VipID(û�л��ߺ죬�ƣ���ɫ)���ӵ������VIP
	uint32_t Level;					//��ɫ�ȼ�
	uint32_t LevelExper;			//��ɫ�ȼ�����
	uint32_t Rank;					//��ɫ��λ
	uint32_t RankExper;				//��λ����
	std::string Name;				//��ɫ����
};

struct ShowHeroRoom					//�������н���ѡӢ�۽�����������
{
	int MateRoomIndex;				//ƥ��ɹ��ķ���ţ���Ҫɾ����ƥ�䷿��
	int RoomIndex;					//Ӣ�۽��淿���
	int TimeIndex;					//ʱ���ڴ��±ꡣÿ�����䶼������һ��ʱ���ڴ档
	int DiTuID;						//��ͼID����Ϊ���кܶ��ֲ�ͬ��ͼ�������ƥ��һ��ս����ͼ����ң������ñ�
	std::vector<uint32_t> ZhaDanList;  //ѡ��ɹ���ը������ѹ�뵽�����У���ֹ�ظ�ѡ��
	std::vector<HeroInfo> HeroList;	//���ڸ�Ӣ��ѡ����淿��ȴ���ʼ��Ϸ�ĳ�Ա
};

	extern char BUFF[200];

	extern pthread_cond_t readcode;       //���������������ı���

	extern pthread_mutex_t readsuo;      //�����ݵ���

	extern pthread_t readpthread;      //�������̱߳���

	extern pthread_cond_t writecode;       //д�������������ı���

	extern pthread_mutex_t writesuo;      //д���ݵ���

	extern pthread_t writepthread;      //д�����̱߳���

	extern pthread_cond_t chulicode;       //�����������ݰ���Ҫ�����������ı���

	extern pthread_mutex_t chulisuo;      //�����������ݰ���Ҫ����

	extern pthread_t chulipthread;      //�����������ݰ���Ҫ���̱߳���

	extern pthread_cond_t timecode;       //����ʱ����Ҫ�����������ı���

	extern pthread_mutex_t timesuo;      //����ʱ����Ҫ����

	extern pthread_t timepthread;      //����ʱ����Ҫ���̱߳���

	extern std::queue<int> readduilie;             //ʹ��һ�����н��ж����ݶ��Ĳ��������е����ݾ��� IO��������IO��֤����IO�������ˣ�Ҫ���д���

	extern std::queue<shuji> messageduilie;       //�Ѷ����ͻ��˷����������ݰ��ֽڴ浽�����У�������һ���߳�ȥ���������ݰ�

	extern std::queue<sendshuji> sendduilie;      //��Ҫ���͸��ͻ��˵����ݱ��浽������

	extern std::queue<struct timeduilie> timequeue;    //����ʱʱ��Ľṹ���浽һ����ʱ�����У��ö�ʱ���߳�ȥ�������ж�ʱʱ��

	extern std::map<int,CRoleObj> mRoleList;		//����ȫ�����ߵ��û�role

	extern txtlog MYLOG;                    //����һ����¼������־��ȫ���ࡣ

	extern timeku timelei;                //����һ��ʱ����࣬ר�Ź������еĶ�ʱʱ��仯

	//ƥ��

	extern std::map<uint64_t,int> UserRoom;		//ÿ������ƥ�����Ҷ�����һ������ţ�����ƥ���uid��Ӧһ�������

	extern std::map<int,Room> RoomMap;			//�ȴ���ʼ��Ϸ�ķ��䡣

	extern std::stack<int> DeleteStack;			//�������б����ٵķ��䣬�൱�ڻ��շ����

	extern int iRoomIndex;						//���뷿��ţ�������¼����ź����뷿��Ż�ʹ�õ�

	//����Ӣ��ѡ�����

	extern std::map<uint64_t,int> UserHeroRoom;		//ÿ������Ӣ��ѡ��������Ҷ�����һ������ţ�����ƥ���uid��Ӧһ�������

	extern std::map<int,ShowHeroRoom> HeroRoomMap;			//�ȴ�Ӣ��ѡ����濪ʼ��Ϸ�ķ��䡣

	extern std::stack<int> DeleteHeroRoomStack;			//�������б����ٵķ��䣬�൱�ڻ���Ӣ��ѡ����淿���

	extern int iHeroRoomIndex;							//����Ӣ��ѡ����淿��ţ�������¼����ź����뷿��Ż�ʹ�õ�

	//ƥ����

	extern pthread_mutex_t MateSuo;						//����һ��ƥ��������������ȫ�����ݵķ���

	//�����������ȫ�ֱ���

	extern int g_is_first;

#endif