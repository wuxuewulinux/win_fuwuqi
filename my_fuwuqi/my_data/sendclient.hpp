
#ifndef _SENDCLIENT_HPP_
#define _SENDCLIENT_HPP_
#include "../proto/SSmsg.pb.h"

void SendServer(int userio,SSMsg * test);	//�ú����Ƿ�����һ��������������)

void timesend(int kaiguan,int index,int miao,int userid); //�ú������߼���ֻҪ�õ���ʱ���ͻ���ã����ϲ�һ����ʱ�ӿ�

void errorsend(int userio,int error);           //���ʹ�������ͻ��ˣ�ֻҪ���ִ�����ʾ���ͻ��˾͵��øú����ӿڡ�errorΪ�������ֵ

//worksend��timesend��errorsend�������ϲ��߼�������õĽӿ�
/*
void worksend(int userio,MY::msg * test);    //�ú������߼����ڷ������ݸ��ͻ���ʱ��������øýӿڲ��ܷ������ݳ�ȥ��

void timesend(int kaiguan,int index,int miao,int userid); //�ú������߼���ֻҪ�õ���ʱ���ͻ���ã����ϲ�һ����ʱ�ӿ�

void errorsend(int userio,int error);           //���ʹ�������ͻ��ˣ�ֻҪ���ִ�����ʾ���ͻ��˾͵��øú����ӿڡ�errorΪ�������ֵ
*/
#endif // !_SENDCLIENT_HPP_
