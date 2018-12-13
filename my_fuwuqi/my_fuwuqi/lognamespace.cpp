
#include"lognamespace.hpp"
#include "enterfunction.hpp"



namespace logquanju
{
	//添加各个模块的日志

	txtlog majiangLog;           //记录麻将模块的日志信息

	//以后还有别的模块日志可以在这里添加日志类

}

	bool LogInit(std::string path)    //启动所有日志文件
	{
		//启动麻将模块的日志记录
	  {
		if (!logquanju::majiangLog.Init(path + "log/majiangLog.txt"))
		{
			return false;
		}
	  }
	  //以后还有别的模块日志可以在这里添加

	  return true;
	  
	}


