
#include "timeku.hpp"


bool paixu(const fuzhunode & a,const fuzhunode & b)   //����������Ҫ��������ĸ�ʱ�䴥���Ƚ϶̡�
{
	return a.timeid<b.timeid;
}


uint64_t get_time()     //��ȡĿǰϵͳ��ʱ�䣬ʱ���Ժ���Ϊ��λ
{

	struct timeval tv;

	struct timezone tz;

	gettimeofday(&tv,&tz);

	return ((tv.tv_usec/1000)+(tv.tv_sec * 1000));
}


timeku::timeku()
{

	rongqi.resize(100);    //�ȹ̶���ʱ��ڵ��ڴ棬�ȸ�100��������������Լ�������

	for(int i=0;i<100;i++)
	{
		index_stack.push(i);   //�����п��е��ڴ��±�ѹ��ջ��
	}

}

//�ֵ��û���Ϸʱ�ͻ���øú��� 
bool timeku::pushtime(int index,int miao,int user_id)   //indexΪ���û�ʱ��ڵ������������±�,user_idΪ�û�Ψһuid,miaoΪ��ʱ��������Ŵ���
{
	if(rongqi[index].uid != user_id)
		return false;                 
	rongqi[index].endtime=get_time() + miao*1000;           //����ˢ�¸��û���ʱ��ڵ�����մ���ʱ�䡣
	return true;  
}


bool timeku::poptime(int index,int user_id) //����falseʱ��ʱ�䲻���ϣ���������ȫ�ڴ����
{
	if(rongqi[index].uid != user_id)
		return false;
	rongqi[index].endtime = 0;
	return true;
}


int timeku::startgame(int user_id,void (*p)(void * canshu),void * data_t) //user_idΪ�û�uid,pΪ�ýڵ㴥��ִ�к�����data_tΪִ�к�������Ĳ���
{
	if(index_stack.empty())   //���ջû����ֵ��֤�������ڴ治������Ҫ�����ڴ�
	{
		rongqi.resize(rongqi.size()+100);     //�ڼ�������100�ڴ�
		for(int i=rongqi.size()-100;i<(int)rongqi.size();i++)
			index_stack.push(i);    //�ѿ��е��ڴ��±�ѹ��ջ��
		int value=index_stack.top();     //ȡ��һ�����е��ڴ��±�
		index_stack.pop();
		rongqi[value].uid=user_id;
		rongqi[value].p=p;
		rongqi[value].data=data_t;
		return value;
	}
	else     
	{
		int value=index_stack.top();
		index_stack.pop();
		rongqi[value].uid=user_id;
		rongqi[value].p=p;
		rongqi[value].data=data_t;
		return value; 
	}
	return 0;
}


bool timeku::endgame(int index,int user_id)    //������falseʱ��֤���ڴ����ݺ��û����ݷ�����һ�´���
{
	if(rongqi[index].uid != user_id)
		return false;                       
	rongqi[index].uid=0;
	rongqi[index].data=NULL;
	rongqi[index].endtime=0;
	index_stack.push(index);
	return true;
}


//bianli������chufatime�������ڶ�ʱ������߳���


struct fuzhunode timeku::bianli()
{
	std::vector<struct fuzhunode> timerongqi;
	struct fuzhunode test;
	for(std::vector<timenode>::iterator i=rongqi.begin();i!=rongqi.end();i++)
	{
		if((*i).uid == 0 || (*i).endtime == 0)
			continue;

		long timedanwei=(*i).endtime - get_time();         //ϵͳ����ʱ���ȥϵͳʱ�����Ҫ������ʱ��
		if(timedanwei < 0)                                //С��0֤����ʱ��ڵ㴥����
		{
			(*i).p((*i).data);                                  //ִ�иô�����������
		}
		else
		{
			test.timeid=timedanwei;                         //��ʣ��ȴ�����ʱ�丳ֵ�������ṹ��
			test.chufanode=(*i);                           //��ʱ��ڵ㸳ֵ�����ṹ���ʱ��ڵ�
			timerongqi.push_back(test);                //�Ѹ����ṹ��ѹ�������У���Ҫ��Ϊ��ȡ����̵ĵȴ�ʱ���Ӧ�ĸ����ṹ��ڵ�
		}

	}
	if(timerongqi.size() == 0)  
	{
		test.timeid = 0;
		return test;                 //��û��ʱ��ڵ�ʱ����Ҫ���س�ʼֵ���ϲ��̣߳����̵߳ȴ����� 
	} 
	sort(timerongqi.begin(),timerongqi.end(),paixu);     //�����õ���̵Ĵ������ĸ�ʱ��ڵ�ĸ����ṹ��
	return timerongqi[0];     //�������ʱ����ĸ������ṹ��ڵ�

}


struct timespec timeku::chufatime(long haomiao)
{

	struct timespec outtime;
	struct timeval now;
	gettimeofday(&now, NULL);
	long nsec = now.tv_usec * 1000 + (haomiao % 1000) * 1000000;    //��ȡ���룬ȡģ��������������빻�뽫������ȥ��
	outtime.tv_sec=now.tv_sec + nsec / 1000000000 + haomiao / 1000;  //�ȰѺ���ת��������ӵ�ϵͳʱ���У�ʣ���ʱ�佫��ӵ���������Ѻ��׵õ�������ӽ�����
	// nsec / 1000000000��һ���ȡ�������п�������+���빻һ��
	outtime.tv_nsec=nsec % 1000000000;         //�����븳ֵ������������,ʹ��%��Ϊ���п��������������ӹ�һ�룬��Ҫ����ȥ��������ʣ�������
	return outtime;

}