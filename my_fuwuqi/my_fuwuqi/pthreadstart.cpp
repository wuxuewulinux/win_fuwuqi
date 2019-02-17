
#include "quanju.hpp"
#include "pthreadstart.hpp"
#include <errno.h>
#include "HandlerList.hpp"
#include "SocketDBClient.hpp"
#include "epoll_ku.hpp"

const int MAX_BUFF=255;                  //�������ݰ������Χ

//���̵߳�ִ�д���

void * readxiancheng(void *canshu)
{

	pthread_detach(pthread_self());   //����ʬ�߳�
	int sockio,ret;
	struct shuji agemess;           //����ṹ�����ݰ��ṹ�����к���Ҫ�ýṹ�������л��ɹ�
	while(1)
	{

		while(readduilie.empty())  //����Ϊ�ս��ȴ�
		{
			pthread_cond_wait(&readcode,&readsuo);
		}

		while(!readduilie.empty())   //������������ݽ�Ҫ����
		{
			sockio=readduilie.front();
			agemess.io=sockio;
			ret=readline(sockio,&agemess.mess,MAX_BUFF);     //��ʼ�Ը�IO�����ݽ��ж�ȡ
			if(ret == -2)
			{      
				std::cout<<"read error message!"<<std::endl;
				MYLOG.printflog("read error message!");
				//������ݷ������󣬾���Ҫ�Ը�IO���д���

			}
			if(ret == -1)
			{
				EPOLLKU->Epoll_ShanChu(sockio);
				std::cout<<"Client Quit !"<<std::endl;
				//����յ��ͻ����˳�������Ҫ�Ը�IO���д���
			}	
			if(ret > 0 && messageduilie.empty() )                 //֤����һ���������� 
			{
				//��ֹ��������
				pthread_mutex_lock(&chulisuo);
				pthread_mutex_unlock(&chulisuo);
				messageduilie.push(agemess);        //�ѽ��յ������ݰ�ѹ������У��͸����ģ��ȥ�����ʹ�������ݰ�
				pthread_cond_signal(&chulicode);  //�����ź�
			}
			else if(ret > 0)       //��һ�����ݰ������Ƕ��в�Ϊ��
			{
				messageduilie.push(agemess);
				pthread_cond_signal(&chulicode);            
			}
			readduilie.pop();                    //�������IO�����Ӷ���ɾ����
			memset(&agemess,0,sizeof(agemess));
		}

	}
	return NULL;
}



//�����̵߳�ִ�к���

void ChuLiAgemessClient(struct message * message,int * io)
{
	CSMsg mess_ti;

	//�����л����ݰ�

	if (!mess_ti.ParseFromArray(message->buff, message->len))
	{
		std::cout << " Deserialization ChuLiAgemessClient Fali ��" << std::endl;
		MYLOG.printflog("������ʧ�ܣ�");     
		return;
	}
	//��ȡģ�����
	if (CS_MSGID_MIN < mess_ti.head().msgid() && mess_ti.head().msgid() < CS_MSGID_MAX)
	{
		IHandler* handler = HANDLERLIST->GetHandler(mess_ti.head().msgid());
		//ִ��ģ�鹦��
		handler->OnClientMsg(mess_ti,*io);
	}
}



void ChuLiAgemessServer(struct message * message,int * io)
{
	SSMsg mess_ti;

	//�����л����ݰ�

	if (!mess_ti.ParseFromArray(message->buff, message->len))
	{
		std::cout << "Deserialization ChuLiAgemessServer Fali ����" << std::endl;
		MYLOG.printflog("������ʧ�ܣ�");     
		return;
	}
	if (SS_MSGID_MIN < mess_ti.head().msgid() && mess_ti.head().msgid() < SS_MSGID_MAX)
	{
		IHandler* handler = HANDLERLIST->GetHandler(mess_ti.head().msgid());
		//ִ��ģ�鹦��
		handler->OnServerMsg(mess_ti);		//��Ϸ���������������ת�����ݣ�Ȼ���ķ��������ؾ͵���OnServerMsg
	}
}


void * chulimessage(void *canshu)
{
	pthread_detach(pthread_self());   //����ʬ�߳�
	struct shuji test;
	while(1)
	{

		while(messageduilie.empty())  //����Ϊ�ս��ȴ�
		{
			pthread_cond_wait(&chulicode,&chulisuo);
		}

		while(!messageduilie.empty())   //������������ݽ�Ҫ����
		{
			test=messageduilie.front();
			if(DBCLIENT->GetSocketIo() == test.io)
				ChuLiAgemessServer(&test.mess,&test.io);
			else
			    ChuLiAgemessClient(&test.mess,&test.io);      //�ú�������������ݺ�ת������Ӧ�Ĺ���ģ����    

			messageduilie.pop();         //���������Ϣ��Ҫ�Ӷ���ȥ��          


		}	

	}

	return NULL;
}



//д�̵߳�ִ�к���

void * writexiancheng(void *canshu)
{
	pthread_detach(pthread_self());   //����ʬ�߳�
	struct sendshuji send;  
	while(1)
	{
		while(sendduilie.empty())  //����Ϊ�ս��ȴ�
		{
			pthread_cond_wait(&writecode,&writesuo);
		}
		while(!sendduilie.empty())   //������������ݾ�Ҫ����Ѹ����ݷ��ͳ�ȥ
		{
			send=sendduilie.front();
			int value=my_write(send.io,(const char *)&send.rMessageBody,send.len);    //��ͻ��˷���һ�����ݰ�
			if(value<0)
			{
				//�ͻ��˹ر����ӣ�����ر����ӵĲ���
			}
			sendduilie.pop();                    //��������ݣ�������Ӷ���ɾ��

		}	

	}
	return NULL;
}




//��ʱ���߳�ִ�к���

void * timexiancheng(void * canshu) //��ʱ��ִ���߳�
{
	int a;
	struct timeduilie test;
	struct fuzhunode node;
	node.timeid=0;
	while(1)
	{
		while(!timequeue.empty())  //������в�Ϊ�վ�һֱ����ʱ��
		{
			test=timequeue.front();
			if(test.kaiguan == true)  //ѹ��ʱ�����
			{
				timelei.pushtime(test.index,test.miao,test.userid);   //ѹ��һ��ʱ��ڵ㵽ʱ���������                        
				timequeue.pop();
				if(timequeue.empty())
					node = timelei.bianli();   //����ʱ��������鿴��Щʱ������̴���
			}
			else     //ɾ��ʱ�����
			{
				timelei.poptime(test.index,test.userid);                    
				timequeue.pop();
				if(timequeue.empty())
					node = timelei.bianli();
			}
		}
		while(timequeue.empty())  //�������Ϊ�վ�һ�α���ʱ�䣬ȡ����ʱʱ��
		{
			while(node.timeid > 0)    //ʱ�����0֤����ʱ��ڵ�
			{
				struct timespec outtime;
				outtime = timelei.chufatime(node.timeid);
				a = pthread_cond_timedwait(&timecode,&timesuo,&outtime);    //��ʱ��ڵ㣬�����ȴ���ʱ�¼�
				if(a == ETIMEDOUT)   //������������֤����ʱ��ڵ㳬ʱ��
				{
					node.chufanode.p(node.chufanode.data);
					break;               //ִ�иýڵ㴥������
				}
				else
					break;
			}
			while(node.timeid == 0)  //ʱ�����0֤������û��ʱ��ڵ㣬�����ȴ���
			{
				pthread_cond_wait(&timecode,&timesuo);
			}
			if(timequeue.empty())
				node = timelei.bianli();
		}
	}
	return NULL;
} 



//�����̵߳�����

int pthreadstart()
{
//�������ݰ��߳�
pthread_mutex_init(&readsuo,NULL);   //����һ����
pthread_cond_init(&readcode,NULL);    //����һ����������
int ret=pthread_create(&readpthread,NULL,readxiancheng,NULL);   //����һ���߳�
if(ret != 0)
{
std::cout<<"create read pthread fail!"<<std::endl;
MYLOG.printflog("create read pthread fail!");
return -1;
}

//���������ݰ��߳�
pthread_mutex_init(&chulisuo,NULL);   //����һ����
pthread_cond_init(&chulicode,NULL);    //����һ����������
ret=pthread_create(&chulipthread,NULL,chulimessage,NULL);   //����һ���߳�
if(ret != 0)
{
std::cout<<"create work  pthread fail!"<<std::endl;
MYLOG.printflog("create work  pthread fail!");
return -1;
}

//��д�����߳�
pthread_mutex_init(&writesuo,NULL);   //����һ����
pthread_cond_init(&writecode,NULL);    //����һ����������
ret=pthread_create(&writepthread,NULL,writexiancheng,NULL);   //����һ���߳�
if(ret != 0)
{
std::cout<<"create write  pthread fail!"<<std::endl;
MYLOG.printflog("create write  pthread fail!");
return -1;
}


//����ʱ������߳�
pthread_mutex_init(&timesuo,NULL);   //����һ����
pthread_cond_init(&timecode,NULL);    //����һ����������
ret=pthread_create(&timepthread,NULL,timexiancheng,NULL);   //����һ���߳�
if(ret != 0)
{
std::cout<<"create time  pthread fail!"<<std::endl;
MYLOG.printflog("create time  pthread fail!");
return -1;
}

return 0;

  }


