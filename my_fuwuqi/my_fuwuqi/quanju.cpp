#include "quanju.hpp"

char BUFF[200];

pthread_cond_t readcode;       //���������������ı���

pthread_mutex_t readsuo;      //�����ݵ���

pthread_t readpthread;      //�������̱߳���

pthread_cond_t writecode;       //д�������������ı���

pthread_mutex_t writesuo;      //д���ݵ���

pthread_t writepthread;      //д�����̱߳���

pthread_cond_t chulicode;       //�����������ݰ���Ҫ�����������ı���

pthread_mutex_t chulisuo;      //�����������ݰ���Ҫ����

pthread_t chulipthread;      //�����������ݰ���Ҫ���̱߳���

pthread_cond_t timecode;       //����ʱ����Ҫ�����������ı���

pthread_mutex_t timesuo;      //����ʱ����Ҫ����

pthread_t timepthread;      //����ʱ����Ҫ���̱߳���

std::queue<int> readduilie;             //ʹ��һ�����н��ж����ݶ��Ĳ��������е����ݾ��� IO��������IO��֤����IO�������ˣ�Ҫ���д���

std::queue<shuji> messageduilie;       //�Ѷ����ͻ��˷����������ݰ��ֽڴ浽�����У�������һ���߳�ȥ���������ݰ�

std::queue<sendshuji> sendduilie;      //��Ҫ���͸��ͻ��˵����ݱ��浽������

std::queue<struct timeduilie> timequeue;    //����ʱʱ��Ľṹ���浽һ����ʱ�����У��ö�ʱ���߳�ȥ�������ж�ʱʱ��

std::map<int,CRoleObj> mRoleList;

//Epoll_Ku epoll_lei;              //����һ��epollȫ����

txtlog MYLOG;                    //����һ����¼������־��ȫ���ࡣ

timeku timelei;                //����һ��ʱ����࣬ר�Ź������еĶ�ʱʱ��仯

std::map<uint64_t,int> UserRoom;		//ÿ������ƥ�����Ҷ�����һ������ţ�����ƥ���uid��Ӧһ�������

std::map<int,Room> RoomMap;				//�ȴ���ʼ��Ϸ�ķ��䡣

std::stack<int> DeleteStack;			//�������б����ٵķ��䣬�൱�ڻ��շ����

int iRoomIndex = 0;


std::map<uint64_t,int> UserHeroRoom;		//ÿ������Ӣ��ѡ��������Ҷ�����һ������ţ�����ƥ���uid��Ӧһ�������

std::map<int,ShowHeroRoom> HeroRoomMap;			//�ȴ�Ӣ��ѡ����濪ʼ��Ϸ�ķ��䡣

std::stack<int> DeleteHeroRoomStack;			//�������б����ٵķ��䣬�൱�ڻ���Ӣ��ѡ����淿���

int iHeroRoomIndex = 0;
