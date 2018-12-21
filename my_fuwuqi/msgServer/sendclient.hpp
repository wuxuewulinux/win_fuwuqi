
#ifndef _SENDCLIENT_HPP_
#define _SENDCLIENT_HPP_
#include "../proto/SSmsg.pb.h"

void SendServer(int userio,SSMsg * test);	//该函数是发给另一个服务器（进程)

void timesend(int kaiguan,int index,int miao,int userid); //该函数是逻辑层只要用到定时器就会调用，给上层一个定时接口

void errorsend(int userio,int error);           //发送错误码给客户端，只要出现错误提示给客户端就调用该函数接口。error为错误码的值

//worksend和timesend和errorsend函数是上层逻辑必须调用的接口
/*
void worksend(int userio,MY::msg * test);    //该函数是逻辑层在发送数据给客户端时，必须调用该接口才能发送数据出去。

void timesend(int kaiguan,int index,int miao,int userid); //该函数是逻辑层只要用到定时器就会调用，给上层一个定时接口

void errorsend(int userio,int error);           //发送错误码给客户端，只要出现错误提示给客户端就调用该函数接口。error为错误码的值
*/
#endif // !_SENDCLIENT_HPP_
