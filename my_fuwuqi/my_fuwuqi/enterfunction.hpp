#ifndef _ENTERFUNCTION_HPP_
#define _ENTERFUNCTION_HPP_

#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "quanju.hpp"


void gateway();         //������������

int ConditionInit();	//��������������ʼ��.

bool LogInit(std::string path);       //����������־�ļ�

CRoleObj * GetRole(uint64_t Uid);

void PushRole(uint64_t Uid,CRoleObj* rRole);

void DeleteRole(uint64_t Uid);

//ƥ���������в���

int GetUserRoom(uint64_t Uid);

void PushUserRoom(uint64_t Uid,int RoomIndex);

void DeleteUserRoom(uint64_t Uid);

Room * GetRoomMap(int RoomIndex);

void PushRoomMap(int RoomIndex,Room* rRoom);

void DeleteRoomMap(int RoomIndex);

int AskRoomIndex();				//���뷿���

void DeleteRoomIndex(int RoomIndex);	//����һ�������൱�ڻ���һ�������


//Ӣ��ѡ��������в���

int GetShowHeroRoom(uint64_t Uid);

void PushShowHeroRoom(uint64_t Uid,int RoomIndex);

void DeleteShowHeroRoom(uint64_t Uid);

ShowHeroRoom * GetShowHeroRoomMap(int RoomIndex);

void PushShowHeroRoomMap(int RoomIndex,ShowHeroRoom* rRoom);

void DeleteShowHeroRoomMap(int RoomIndex);

int AskShowHeroRoomIndex();						//���뷿���

void DeleteShowHeroRoomIndex(int RoomIndex);	//����һ�������൱�ڻ���һ�������


//���ö�ʱ���ӿ�

int GetTimeIndex(int Room_id,void (*p)(void * canshu),void * data);			//��ȡ��ʱ���±��ڴ��ַ

bool DeleteTimeIndex(int TimeIndex,int Room_id);							//�ͷŶ�ʱ���±��ڴ��ַ

//����ƥ����

void OpenMateSuo();								//��ȡ��

void CloseMateSuo();							//�ͷ���

//��ȡһ����Χ���������������������ֵҲ������

int Rand(int Min,int Max);

//��ȡһ��ս����ͼ

uint32_t GetMapId();

#endif // !_ENTERFUNCTION_HPP_
