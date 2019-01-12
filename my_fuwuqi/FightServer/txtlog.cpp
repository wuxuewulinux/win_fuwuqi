
#include"txtlog.hpp"

bool txtlog::Init(std::string filename)
{
     outf.open(filename.c_str(), std::ios::out|std::ios::app);  //以写入和在文件末尾添加的方式打开.txt文件，没有的话就创建该文件。
        if (!outf.is_open())
        {      std::cout<<"日志文件创建失败:"+ filename<<std::endl;
                return false;
        }

      return true;
}


void txtlog::printflog(std::string neirong)
{
//先知道该日志是哪个时间段产生的
time(&timep);
std::string timename(ctime(&timep));    //成功得到现在时间
timename[timename.size()-1]=' ';
//开始把时间和内容整合到一行写入到文件中
outf<<timename<<"         "<<neirong<<std::endl;
}


void txtlog::sprintf(char * buffer,const char * format,...)
{
	va_list arg;
	va_start(arg,format);
	vsprintf(buffer,format,arg);
	std::string name(buffer);
	printflog(name);             //把日志内容按格式输入到日志文件中
	memset(buffer,0,200);
	va_end(arg);
}















