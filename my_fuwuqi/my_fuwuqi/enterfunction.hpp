#ifndef _ENTERFUNCTION_HPP_
#define _ENTERFUNCTION_HPP_

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include "quanju.hpp"

void gateway();         //启动服务器。

bool LogInit(std::string path);       //启动所有日志文件

CRoleObj * GetRole(uint64_t Uid);

void PushRole(uint64_t Uid,CRoleObj* rRole);

void DeleteRole(uint64_t Uid);

//匹配界面的所有操作

int GetUserRoom(uint64_t Uid);

void PushUserRoom(uint64_t Uid,int RoomIndex);

void DeleteUserRoom(uint64_t Uid);

Room * GetRoomMap(int RoomIndex);

void PushRoomMap(int RoomIndex,Room* rRoom);

void DeleteRoomMap(int RoomIndex);

int AskRoomIndex();				//申请房间号

void DeleteRoomIndex(int RoomIndex);	//销毁一个房间相当于回收一个房间号


//英雄选择界面所有操作

int GetShowHeroRoom(uint64_t Uid);

void PushShowHeroRoom(uint64_t Uid,int RoomIndex);

void DeleteShowHeroRoom(uint64_t Uid);

ShowHeroRoom * GetShowHeroRoomMap(int RoomIndex);

void PushShowHeroRoomMap(int RoomIndex,ShowHeroRoom* rRoom);

void DeleteShowHeroRoomMap(int RoomIndex);

int AskShowHeroRoomIndex();						//申请房间号

void DeleteShowHeroRoomIndex(int RoomIndex);	//销毁一个房间相当于回收一个房间号


//调用定时器接口

int GetTimeIndex(int Room_id,void (*p)(void * canshu),void * data);

bool DeleteTimeIndex(int TimeIndex,int Room_id);

#endif // !_ENTERFUNCTION_HPP_
