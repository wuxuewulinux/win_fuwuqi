
#include"lognamespace.hpp"
#include "enterfunction.hpp"



namespace logquanju
{
	//��Ӹ���ģ�����־

	txtlog majiangLog;           //��¼�齫ģ�����־��Ϣ

	//�Ժ��б��ģ����־���������������־��

}

	bool LogInit(std::string path)    //����������־�ļ�
	{
		//�����齫ģ�����־��¼
	  {
		if (!logquanju::majiangLog.Init(path + "log/majiangLog.txt"))
		{
			return false;
		}
	  }
	  //�Ժ��б��ģ����־�������������

	  return true;
	  
	}


