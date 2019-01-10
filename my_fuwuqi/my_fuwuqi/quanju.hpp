#ifndef _QUANJU_HPP_
#define _QUANJU_HPP_

//该头文件定义存储日志线程所需要的准备资源
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

//判断指针的宏函数
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



#define TIMESTAMP  get_time()/1000			//获取当前时间，返回的是秒为单位

struct shuji
{

	int io;

	struct message mess;

};


struct MessageBody
{
	unsigned short length;                         //该数据的长度

	char buff[1024];
};

struct sendshuji     //序列化数据然后保存到线程队列中在发送给客户端
{

	int io;
	int len;
	struct MessageBody rMessageBody;

};

struct timeduilie       //定时器需要保存的结构体
{
	bool kaiguan;    //如果true为增加时间（push操作），如果false为去掉时间（pop操作）
	int index;
	int miao;
	int userid;
};

struct UidInfo
{
	uint64_t Uid;
	bool Button;				//false为没有点击同意，true为点击同意。初始化为false
};

struct Room
{
	int MaxCount;					//最大人数，就是够该人数就开始游戏
	int UserCount;					//在该房间人数
	int RoomIndex;					//房间号
	int Agree;						//点击同意开始游戏。Agree与MaxCount相当就可以开始游戏
	int TimeIndex;					//时间内存下标。每个房间都会申请一个时间内存。
	std::vector<UidInfo> UidList;	//正在该房间等待开始游戏的成员
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
	uint32_t PlaneId;		//飞机ID
	uint32_t ZhaDanId;		//炸弹ID
	uint32_t DiPanId;		//飞机底盘ID
	uint32_t SkillOneID;	//被动技能1特效ID
	uint32_t SkillTwoID;	//被动技能2特效ID
	//战斗结束就需要计算
	HeroVipInfo VipList[3];			//VipID(没有或者红，黄，紫色)最大拥有三种VIP
	uint32_t Level;					//角色等级
	uint32_t LevelExper;			//角色等级经验
	uint32_t Rank;					//角色段位
	uint32_t RankExper;				//段位经验
	std::string Name;				//角色名称
};

struct ShowHeroRoom					//保存所有进入选英雄界面的玩家数据
{
	int MateRoomIndex;				//匹配成功的房间号，需要删除的匹配房间
	int RoomIndex;					//英雄界面房间号
	int TimeIndex;					//时间内存下标。每个房间都会申请一个时间内存。
	int DiTuID;						//地图ID，因为会有很多种不同地图，会随机匹配一张战斗地图给玩家，读配置表
	std::vector<uint32_t> ZhaDanList;  //选择成功的炸弹类型压入到容器中，防止重复选择
	std::vector<HeroInfo> HeroList;	//正在该英雄选择界面房间等待开始游戏的成员
};

	extern char BUFF[200];

	extern pthread_cond_t readcode;       //读数据条件变量的变量

	extern pthread_mutex_t readsuo;      //读数据的锁

	extern pthread_t readpthread;      //读数据线程变量

	extern pthread_cond_t writecode;       //写数据条件变量的变量

	extern pthread_mutex_t writesuo;      //写数据的锁

	extern pthread_t writepthread;      //写数据线程变量

	extern pthread_cond_t chulicode;       //解析处理数据包需要的条件变量的变量

	extern pthread_mutex_t chulisuo;      //解析处理数据包需要的锁

	extern pthread_t chulipthread;      //解析处理数据包需要的线程变量

	extern pthread_cond_t timecode;       //处理定时器需要的条件变量的变量

	extern pthread_mutex_t timesuo;      //处理定时器需要的锁

	extern pthread_t timepthread;      //处理定时器需要的线程变量

	extern std::queue<int> readduilie;             //使用一个队列进行对数据读的操作，队列的数据就是 IO。队列有IO就证明该IO来数据了，要进行处理

	extern std::queue<shuji> messageduilie;       //把读到客户端发过来的数据包字节存到队列中，让另外一个线程去解析该数据包

	extern std::queue<sendshuji> sendduilie;      //把要发送给客户端的数据保存到队列中

	extern std::queue<struct timeduilie> timequeue;    //处理定时时间的结构保存到一个定时队列中，让定时器线程去处理所有定时时间

	extern std::map<int,CRoleObj> mRoleList;		//保存全部在线的用户role

	extern txtlog MYLOG;                    //声明一个记录错误日志的全局类。

	extern timeku timelei;                //声明一个时间库类，专门管理所有的定时时间变化

	//匹配

	extern std::map<uint64_t,int> UserRoom;		//每个进入匹配的玩家都会有一个房间号，进入匹配的uid对应一个房间号

	extern std::map<int,Room> RoomMap;			//等待开始游戏的房间。

	extern std::stack<int> DeleteStack;			//保存所有被销毁的房间，相当于回收房间号

	extern int iRoomIndex;						//申请房间号，用来记录房间号和申请房间号会使用到

	//进入英雄选择界面

	extern std::map<uint64_t,int> UserHeroRoom;		//每个进入英雄选择界面的玩家都会有一个房间号，进入匹配的uid对应一个房间号

	extern std::map<int,ShowHeroRoom> HeroRoomMap;			//等待英雄选择界面开始游戏的房间。

	extern std::stack<int> DeleteHeroRoomStack;			//保存所有被销毁的房间，相当于回收英雄选择界面房间号

	extern int iHeroRoomIndex;							//申请英雄选择界面房间号，用来记录房间号和申请房间号会使用到

	//匹配锁

	extern pthread_mutex_t MateSuo;						//创建一个匹配锁，用来控制全局数据的访问

	//控制随机数的全局变量

	extern int g_is_first;

#endif