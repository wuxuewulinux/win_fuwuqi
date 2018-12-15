
#include "timeku.hpp"


bool paixu(const fuzhunode & a,const fuzhunode & b)   //计算排序，主要是排序出哪个时间触发比较短。
{
	return a.timeid<b.timeid;
}


uint64_t get_time()     //获取目前系统的时间，时间以毫秒为单位
{

	struct timeval tv;

	struct timezone tz;

	gettimeofday(&tv,&tz);

	return ((tv.tv_usec/1000)+(tv.tv_sec * 1000));
}


timeku::timeku()
{

	rongqi.resize(100);    //先固定好时间节点内存，先给100，如果不够还可以继续增加

	for(int i=0;i<100;i++)
	{
		index_stack.push(i);   //把所有空闲的内存下标压入栈中
	}

}

//轮到用户游戏时就会调用该函数 
bool timeku::pushtime(int index,int miao,int user_id)   //index为该用户时间节点所在容器的下标,user_id为用户唯一uid,miao为定时器多少秒才触发
{
	if(rongqi[index].uid != user_id)
		return false;                 
	rongqi[index].endtime=get_time() + miao*1000;           //重新刷新该用户的时间节点的最终触发时间。
	return true;  
}


bool timeku::poptime(int index,int user_id) //返回false时，时间不符合，发生不安全内存访问
{
	if(rongqi[index].uid != user_id)
		return false;
	rongqi[index].endtime = 0;
	return true;
}


int timeku::startgame(int user_id,void (*p)(void * canshu),void * data_t) //user_id为用户uid,p为该节点触发执行函数，data_t为执行函数传入的参数
{
	if(index_stack.empty())   //如果栈没有数值，证明容器内存不够，需要扩大内存
	{
		rongqi.resize(rongqi.size()+100);     //在继续增加100内存
		for(int i=rongqi.size()-100;i<(int)rongqi.size();i++)
			index_stack.push(i);    //把空闲的内存下标压入栈中
		int value=index_stack.top();     //取出一个空闲的内存下标
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


bool timeku::endgame(int index,int user_id)    //当返回false时，证明内存数据和用户数据发生不一致错误
{
	if(rongqi[index].uid != user_id)
		return false;                       
	rongqi[index].uid=0;
	rongqi[index].data=NULL;
	rongqi[index].endtime=0;
	index_stack.push(index);
	return true;
}


//bianli函数和chufatime函数用在定时器检测线程上


struct fuzhunode timeku::bianli()
{
	std::vector<struct fuzhunode> timerongqi;
	struct fuzhunode test;
	for(std::vector<timenode>::iterator i=rongqi.begin();i!=rongqi.end();i++)
	{
		if((*i).uid == 0 || (*i).endtime == 0)
			continue;

		long timedanwei=(*i).endtime - get_time();         //系统触发时间减去系统时间等于要触发的时间
		if(timedanwei < 0)                                //小于0证明该时间节点触发了
		{
			(*i).p((*i).data);                                  //执行该触发条件函数
		}
		else
		{
			test.timeid=timedanwei;                         //把剩余等待触发时间赋值给辅助结构体
			test.chufanode=(*i);                           //把时间节点赋值辅助结构体的时间节点
			timerongqi.push_back(test);                //把辅助结构体压入容器中，主要是为了取出最短的等待时间对应的辅助结构体节点
		}

	}
	if(timerongqi.size() == 0)  
	{
		test.timeid = 0;
		return test;                 //当没有时间节点时，就要返回初始值给上层线程，让线程等待阻塞 
	} 
	sort(timerongqi.begin(),timerongqi.end(),paixu);     //排序拿到最短的触发的哪个时间节点的辅助结构体
	return timerongqi[0];     //返回最短时间的哪个辅助结构体节点

}


struct timespec timeku::chufatime(long haomiao)
{

	struct timespec outtime;
	struct timeval now;
	gettimeofday(&now, NULL);
	long nsec = now.tv_usec * 1000 + (haomiao % 1000) * 1000000;    //先取纳秒，取模的作用是如果毫秒够秒将把他秒去掉
	outtime.tv_sec=now.tv_sec + nsec / 1000000000 + haomiao / 1000;  //先把毫秒转换成秒添加到系统时间中，剩余的时间将添加到纳秒哪里，把毫米得到的秒添加进来，
	// nsec / 1000000000够一秒就取出来。有可能纳秒+纳秒够一秒
	outtime.tv_nsec=nsec % 1000000000;         //把纳秒赋值给结果体的纳秒,使用%是为了有可能纳秒和纳秒相加够一秒，就要把秒去掉，留下剩余的纳秒
	return outtime;

}