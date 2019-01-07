
#include"quanju.hpp"
#include<string.h>
#include "sendclient.hpp"
#include <arpa/inet.h>

void SendClient(int userio,CSMsg * test)
{
	//开始序列化数据
	struct sendshuji sendtest;
	unsigned short len = test->ByteSize();
	if (!test->SerializeToArray(sendtest.rMessageBody.buff,len)) 
	{ 
		std::cout << "序列化数据失败" << std::endl; 
	    MYLOG.printflog("序列化数据失败！"); 
		return;
	}
	sendtest.rMessageBody.length = htons(len);
	sendtest.io=userio;
	sendtest.len = len + sizeof(unsigned short);
	//开始把该数据送往发送线程去处理发送数据

   //sendtest.length=len;
   //memcpy(sendtest.buff,buff,len);
   if(sendduilie.empty() )                 //证明有一个完整包需要发送出去
  {
	//防止死锁出现
    pthread_mutex_lock(&writesuo);
    pthread_mutex_unlock(&writesuo);
    sendduilie.push(sendtest);        //把要发送的数据包压入队列中，
    pthread_cond_signal(&writecode);  //发送信号
  }
  else
  {
  sendduilie.push(sendtest);
  pthread_cond_signal(&writecode);            
   }

}



void SendServer(int userio,SSMsg * test)
{
	//开始序列化数据
	struct sendshuji sendtest;
	unsigned short len = test->ByteSize();
	if (!test->SerializeToArray(sendtest.rMessageBody.buff,len)) 
	{ 
		std::cout << "序列化数据失败" << std::endl; 
		MYLOG.printflog("序列化数据失败！"); 
		return;
	}
	sendtest.rMessageBody.length = htons(len);
	sendtest.io=userio;
	sendtest.len = len + sizeof(unsigned short);
	//开始把该数据送往发送线程去处理发送数据
	if(sendduilie.empty() )                 //证明有一个完整包需要发送出去
	{
		//防止死锁出现
		pthread_mutex_lock(&writesuo);
		pthread_mutex_unlock(&writesuo);
		sendduilie.push(sendtest);        //把要发送的数据包压入队列中，
		pthread_cond_signal(&writecode);  //发送信号
	}
	else
	{
		sendduilie.push(sendtest);
		pthread_cond_signal(&writecode);            
	}


}


//上层逻辑定时器接口函数


void TimeSend(bool kaiguan,int index,int miao,int userid)
{

	struct timeduilie timetest;
	timetest.kaiguan=kaiguan;
	timetest.index=index;
	timetest.miao=miao;
	timetest.userid=userid;

	//把该定时时间送往定时线程去处理

	 if(timequeue.empty() )                 //证明有一个完整包需要发送出去
  {
	//防止死锁出现
    pthread_mutex_lock(&timesuo);
    pthread_mutex_unlock(&timesuo);
    timequeue.push(timetest);        //把要发送的数据包压入队列中，
    pthread_cond_signal(&timecode);  //发送信号
  }
  else
  { 
  timequeue.push(timetest);
  pthread_cond_signal(&timecode);            
   }


}


//发送错误码提示给客户端（如：金币不足提示也属于错误码）

void errorsend(int userio,int error)
{
	//开始序列化数据
	
	CSMsg test;
	CSMsgHead * head = test.mutable_head();
    head->set_uid(3);                  //设置错误模块为 3 标识，该设置以后可能会修改，set_type表示访问模块的类型值
	head->set_msgid(CS_MSGID_RegisterLogin);
	//test.set_error(error);                 //错误ID号为error，在客户端提示 error 应该显示那个信息
	struct sendshuji sendtest;
	unsigned short len = test.ByteSize();
	if (!test.SerializeToArray(sendtest.rMessageBody.buff,len)) 
	{ 
		std::cout << "序列化数据失败" << std::endl; 
	    MYLOG.printflog("序列化数据失败！"); 
		return;
	}
	sendtest.rMessageBody.length = htons(len);
	sendtest.io=userio;
	sendtest.len = len + sizeof(unsigned short);
	//开始把该数据送往发送线程去处理发送数据

   if(sendduilie.empty() )                 //证明有一个完整包需要发送出去
  {
	//防止死锁出现
    pthread_mutex_lock(&writesuo);
    pthread_mutex_unlock(&writesuo);
    sendduilie.push(sendtest);        //把要发送的数据包压入队列中，
    pthread_cond_signal(&writecode);  //发送信号
  }
  else
  {
  sendduilie.push(sendtest);
  pthread_cond_signal(&writecode);            
   }

   }


/*
errorsend函数使用：  

quaju::errorsend(io,errornum::JINBI_BUZU); //errornum::JINBI_BUZU为一个错误码，调用该errorsend函数相当于已经发送一个错误数据给客户端了


*/


/*
worksend在上层逻辑中使用说明： 每个上层功能模块需要发送数据时，都会封装一个发送函数，发送多少个数据就会封装多少个上层发送函数，

发送函数（）
{
1.先把要发送的数据保存到protobuff结构中；

2.然后调用worksend函数，上层使用发送函数都是按照这个套路去制作的

}

*/

