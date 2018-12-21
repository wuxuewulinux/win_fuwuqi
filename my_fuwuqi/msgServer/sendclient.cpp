
#include"quanju.hpp"
#include<string.h>
#include "sendclient.hpp"
#include <arpa/inet.h>


void SendServer(int userio,SSMsg * test)
{
	//��ʼ���л�����
	struct sendshuji sendtest;
	unsigned short len = test->ByteSize();
	if (!test->SerializeToArray(sendtest.rMessageBody.buff,len)) 
	{ 
		std::cout << "���л�����ʧ��" << std::endl; 
		MYLOG.printflog("���л�����ʧ�ܣ�"); 
		return;
	}
	sendtest.rMessageBody.length = htons(len);
	sendtest.io=userio;
	sendtest.len = len + sizeof(unsigned short);
	//��ʼ�Ѹ��������������߳�ȥ������������
	if(sendduilie.empty() )                 //֤����һ����������Ҫ���ͳ�ȥ
	{
		//��ֹ��������
		pthread_mutex_lock(&writesuo);
		pthread_mutex_unlock(&writesuo);
		sendduilie.push(sendtest);        //��Ҫ���͵����ݰ�ѹ������У�
		pthread_cond_signal(&writecode);  //�����ź�
	}
	else
	{
		sendduilie.push(sendtest);
		pthread_cond_signal(&writecode);            
	}


}


//�ϲ��߼���ʱ���ӿں���


void timesend(int kaiguan,int index,int miao,int userid)
{

	struct timeduilie timetest;
	timetest.kaiguan=kaiguan;
	timetest.index=index;
	timetest.miao=miao;
	timetest.userid=userid;

	//�Ѹö�ʱʱ��������ʱ�߳�ȥ����

	 if(timequeue.empty() )                 //֤����һ����������Ҫ���ͳ�ȥ
  {
	//��ֹ��������
    pthread_mutex_lock(&timesuo);
    pthread_mutex_unlock(&timesuo);
    timequeue.push(timetest);        //��Ҫ���͵����ݰ�ѹ������У�
    pthread_cond_signal(&timecode);  //�����ź�
  }
  else
  { 
  timequeue.push(timetest);
  pthread_cond_signal(&timecode);            
   }


}


//���ʹ�������ʾ���ͻ��ˣ��磺��Ҳ�����ʾҲ���ڴ����룩

void errorsend(int userio,int error)
{
	//��ʼ���л�����
	
	SSMsg test;
	SSMsgHead * head = test.mutable_head();
    head->set_uid(3);                  //���ô���ģ��Ϊ 3 ��ʶ���������Ժ���ܻ��޸ģ�set_type��ʾ����ģ�������ֵ
	head->set_msgid(SS_MSGID_GameMysql);
	//test.set_error(error);                 //����ID��Ϊerror���ڿͻ�����ʾ error Ӧ����ʾ�Ǹ���Ϣ
	struct sendshuji sendtest;
	unsigned short len = test.ByteSize();
	if (!test.SerializeToArray(sendtest.rMessageBody.buff,len)) 
	{ 
		std::cout << "���л�����ʧ��" << std::endl; 
		MYLOG.printflog("���л�����ʧ�ܣ�"); 
		return;
	}
	sendtest.rMessageBody.length = htons(len);
	sendtest.io=userio;
	sendtest.len = len + sizeof(unsigned short);
	//��ʼ�Ѹ��������������߳�ȥ������������

   if(sendduilie.empty() )                 //֤����һ����������Ҫ���ͳ�ȥ
  {
	//��ֹ��������
    pthread_mutex_lock(&writesuo);
    pthread_mutex_unlock(&writesuo);
    sendduilie.push(sendtest);        //��Ҫ���͵����ݰ�ѹ������У�
    pthread_cond_signal(&writecode);  //�����ź�
  }
  else
  {
  sendduilie.push(sendtest);
  pthread_cond_signal(&writecode);            
   }

   }


/*
errorsend����ʹ�ã�  

quaju::errorsend(io,errornum::JINBI_BUZU); //errornum::JINBI_BUZUΪһ�������룬���ø�errorsend�����൱���Ѿ�����һ���������ݸ��ͻ�����


*/


/*
worksend���ϲ��߼���ʹ��˵���� ÿ���ϲ㹦��ģ����Ҫ��������ʱ�������װһ�����ͺ��������Ͷ��ٸ����ݾͻ��װ���ٸ��ϲ㷢�ͺ�����

���ͺ�������
{
1.�Ȱ�Ҫ���͵����ݱ��浽protobuff�ṹ�У�

2.Ȼ�����worksend�������ϲ�ʹ�÷��ͺ������ǰ��������·ȥ������

}

*/
