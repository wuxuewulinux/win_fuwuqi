

#include"readn.hpp"
#include <unistd.h>
#include<iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>

int readn(int fd,void * buffer,int length)  
{

int bytes_left;
int bytes_read;
char *ptr;
  
bytes_left=length;
while(bytes_left>0)
{
    bytes_read=read(fd,ptr,bytes_read);
    if(bytes_read<0)
    {
      if(errno==EINTR)
         bytes_read=0;
      else
         return(-1);
    }
    else if(bytes_read==0)
        break;
     bytes_left-=bytes_read;
     ptr+=bytes_read;
}
return(length-bytes_left);

}


int readline(int fd,struct message * message_m,int count)                 //count参数为最大范围的字节数，如果超过这个数值将证明数据包超标了
{

int zongret=recv(fd,message_m->clearbuff,count,MSG_PEEK);        //先把目前系统缓冲区中的字节存到缓冲数组中

    if(zongret == 0)
   {
	   return -1;
   }
  while( zongret < 0 )     //小于0有可能客户端关闭连接，也有可能是信号中断
{
    if(errno == EINTR)   //重新接收
	{
		int ret=(int)recv(fd,&message_m->len,sizeof(unsigned short),MSG_PEEK);
		 if( ret == 0 )
	        return -1;    
	}
	else
		return -3;
}

int ret=recv(fd,&message_m->len,sizeof(unsigned short),MSG_PEEK);                   //先读头部字节，使用recv函数是为了不从缓冲区去除数据，等他足够一个数据包在用read函数读。 

   if(ret==0)
   {
	   return -1;
   }
  while(ret < 0 )     //小于0有可能客户端没有数据可读，也有可能是信号中断
{
    if(errno == EINTR)   //重新接收
	{
		ret=recv(fd,&message_m->len,sizeof(unsigned short),MSG_PEEK);
		 if(ret==0)
	        return -1;    
	}
	else 
		return -3;
}
  if(ret < (int)sizeof(unsigned short))
	  return -3;
  if(ret == (int)sizeof(unsigned short))                    //头部足够字节将进行解析头部的数值拿到包体长度，然后在读包体的字节
  {
   message_m->len=ntohs(message_m->len);
   if(message_m->len > count)                //发送了超标的数据客户端将对他进行处理。
   {
      return -2;
   }
   
   if(zongret < (int)((int)sizeof(unsigned short)+message_m->len))               //如果缓冲下来的数据如果小于一个数据包的字节，证明缓冲区目前还不够一个数据包
	   return -3;

   memcpy(message_m->buff,message_m->clearbuff+sizeof(unsigned short),message_m->len);      //把一个数据包的包体字节拷贝出去
   ret=readn(fd,message_m->clearbuff,message_m->len+sizeof(unsigned short));                      //从缓冲区清除一个数据包的字节       
   if(ret == (message_m->len + (int)sizeof(unsigned short)))
	   return 1;                                   //够一个数据包
   else
	   return -1;             
  }
  
  return 0;

 } 





int my_write(int fd,const void *buffer,int length)
{
int bytes_left;
int written_bytes;
char *ptr;

ptr=(char *)buffer;
bytes_left=length;
while(bytes_left>0)
{
        
         written_bytes=write(fd,ptr,bytes_left);
         if(written_bytes<=0)
         {       
                 if(errno==EINTR)
                         written_bytes=0;
                 else             
                         return(-1);
         }
         bytes_left-=written_bytes;
         ptr+=written_bytes;     
}
return(0);
}

