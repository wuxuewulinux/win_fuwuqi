
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

	static int SendMateFetch(int SockIo,uint64_t Uid);						//�㲥�����ɹ�ƥ�����

	static int SendMateNotSuccess(int SockIo,uint64_t Uid);					//�㲥�����ɹ�ƥ����浫�����˲����ͬ�⵼�²��ܿ�ʼ��Ϸ

	static int SendMateNotUser(int SockIo,uint64_t Uid);					//���͸�Э�����Щû�е��ͬ����ʱ�䳬ʱ����ң������ǵĿͻ��˽���رյ�ƥ��״̬.

	static int QuitMateFetch(const CSQuitMateFetchReq& rReq);				//�����ƥ���У�Ȼ����ͻȻ����˳�

	static int NotButtonMateFetch(const CSNotButtonMateFetchReq& rReq);		//�ɹ�ƥ�乻��������ͬ��;ܾ���ť���������ѡ��ܾ���ť

	static int SuccessButtonMateFetch(const CSSuccessButtonMateFetchReq& rReq);	//�ɹ�ƥ�乻��������ͬ��;ܾ���ť���������ѡ��ͬ�ⰴť

	static int EnterHeroShow(Room* PRoom,int iDeleteRoomIndex);						//ִ�н���ѡӢ�۽���Ĳ���

	static int SendHeroInfo(CRoleObj* pRoleObj,int SockIo,uint64_t uUid,Room* pRoom);				//��������Ӣ�۽����������ݸ��ͻ���

	static int EnterHeroShowBag(const CSEnterHeroShowBagReq& rReq);					//����װ�α���ѡ��װ�α�����ID��Ʒ��Ϊ��ʾ����ը����������

	static int ShowZhaDanBag(const CSShowZhaDanBagReq& rReq,CSShowZhaDanBagRsp* pRsp);						//���ѡ��ը��

	static int SendUserShowZhaDan(CRoleObj* pRoleObj,uint64_t UserUid,uint64_t Uid,uint32_t ZhaDanId);		//�㲥��������Ҹ����ѡ���ը����

private:

};


#endif