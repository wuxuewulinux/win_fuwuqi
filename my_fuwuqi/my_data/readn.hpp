
#ifndef _READN_HPP_
#define _READN_HPP_

struct message
{
 short len;

 char buff[256];

 char clearbuff[256];             //添加这个数组是为了辅助清除缓冲区的数据包数据，因为接送一个数据包走就要从缓冲区清除掉

};

int readn(int fd,void * buffer,int length);     //返回值：-1表示客户端关闭连接，大于0表示实际接收到的字节数

int readline(int fd,struct message * message_m,int count);      //返回值：-1表示客户端关闭连接，-2为数据包超标，1表示足够一个数据包

int my_write(int fd,void *buffer,int length); //发送函数，返回-1表示客户端已关闭，返回0表示写入完整的数据包


#endif

