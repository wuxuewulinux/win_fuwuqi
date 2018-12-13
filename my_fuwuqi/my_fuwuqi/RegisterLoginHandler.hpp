


#ifndef _REGISTERLOGINHANDLER_HPP_
#define _REGISTERLOGINHANDLER_HPP_

#include "Handler.hpp"
#include "../proto/CSmsg.pb.h"
#include "../proto/SSmsg.pb.h"
#include "SocketDBClient.hpp"

class RegisterLoginHandler : public IHandler
{
public:
	RegisterLoginHandler();
	virtual ~RegisterLoginHandler();

	virtual int OnClientMsg(const CSMsg& rCSMsg, int iFd); //iFd为io套接字
	virtual int OnServerMsg(const SSMsg& rSSMsg);

private:
	//返回一个结构内存地址出来。eMsgId为第一层协议号，CmdType为第2层协议号，pMsgObj传入一个结构的空地址，函数会给该空地址附上值
	//该函数每个模块的handler都会有这个函数，每个模块有自己该函数的封装

	void* OnCSMsg(CSMsg& rMsg, uint64_t Uid, CSMsgID eMsgId, int CmdType);  //客户端向服务器发送

	void* OnSSMsg(SSMsg& rMsg, uint64_t Uid, SSMsgID eMsgId, int CmdType); //服务器向服务器发送

	//检查客户端的数据包是否存在，正确就返回0，不正确就返回负数
	int OnCheckCSMsg(const CSMsg& rMsg, CSMsgID eMsgId);

	//检查数据库服务器发过来的数据库包是否正确
	int OnCheckSSMsg(const SSMsg& rMsg, SSMsgID eMsgId);

	// 注: OnCSMsg 和 OnCheckCSMsg 和 OnClientMsg 这三个函数每个模块的Handler都有自个的封装。必须要有的

	
private:
	//下面是所有模块功能的入口

	int OnRegisterReq(const CSMsg& rCSMsg, int iFd);							//注册功能(处理客户端)
	int OnLoginReq(const CSMsg& rCSMsg, int iFd);								//登陆功能(处理客户端)
	int OnQuitReq(const CSMsg& rCSMsg, int iFd);								//游戏退出功能(处理客户端)

	int OnRegisterRsp(const SSMsg& rSSMsg);							//注册功能(返回客户端)
	int OnLoginRsp(const SSMsg& rSSMsg);								//登陆功能(返回客户端)
};


#endif // !_REGISTERLOGINHANDLER_HPP_
