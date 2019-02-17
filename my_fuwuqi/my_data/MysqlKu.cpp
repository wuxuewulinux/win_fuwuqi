
#include "MysqlKu.hpp"
#include "quanju.hpp"

MysqlKu::MysqlKu()
{


}

MysqlKu *gs_MysqlKu = NULL;


MysqlKu * MysqlKu::Instance()
{
	if (NULL == gs_MysqlKu)
	{
		gs_MysqlKu = new MysqlKu();
	}

	return gs_MysqlKu;
}

int MysqlKu::InitMysql(string user,string mima,string databases,vector<string> & tables)
{
	//把从配置中读取到的表名称压入容器中
	for(int i = 0;i<(int)tables.size();i++)
	{
		table.push_back(tables[i]); 
	}
	if(table.size() == 0)
	{
		printf("Error connecting to table:");
		MYLOG.printflog("InitMysql : Error connecting to table");
		return -1;
	}
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql,NULL,user.c_str(),mima.c_str(),databases.c_str(),0,NULL,0))
	{           
		printf("Error connecting to database:%s\n",mysql_error(&mysql));
		return -1;
	}
	else
	{
		printf("connecting to Mysql Success\n");
	}
	return 0;
}




int MysqlKu::Login(const string & zhanghu,const string & mima)
{
	int t;
	uint64_t uid;
	string write_mima;
	string str="select Uid,mima from "+table[0]+" where zhanghu='";
	str=str+zhanghu+"';";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error1 making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("Login : Error1 making query");
	}

	res = mysql_use_result(&mysql);
	if((row = mysql_fetch_row(res)))
	{
		uid=atoi(row[0]);
		write_mima=row[1];

		if(write_mima.compare(mima))
		{
			mysql_free_result(res);
			return 2;
		}

		if(FindUid(uid))
		{
			mysql_free_result(res);
			return 4;
		}
		InsertUid(uid);
		mysql_free_result(res);
		return 3;
	}
	else
	{
		mysql_free_result(res);
		return 1;
	}
	mysql_free_result(res);
	return 0;
}



string MysqlKu::GetName(uint64_t Uid)
{
	int t;
	string str="select name from "+table[0]+" where Uid='";
	str=str+std::to_string(Uid)+"';";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("GetName : Error1 making query");
	}

	res = mysql_use_result(&mysql);
	while((row = mysql_fetch_row(res)))
	{
		for(t=0;t < (int)mysql_num_fields(res);t++)
		{
			str=row[t];
			mysql_free_result(res);
			return str;
		}
		printf("\n");

	}
	mysql_free_result(res);
	return "";
}



string MysqlKu::GetDBRole(uint64_t Uid)
{
	int t;
	string str="select role from "+table[0]+" where Uid=";
	str=str+to_string(Uid)+";";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("GetDBRole : Error1 making query");
	}
	res = mysql_use_result(&mysql);
	while((row = mysql_fetch_row(res)))
	{
		for(t=0;t < (int)mysql_num_fields(res);t++)
		{
			str=row[t];
			mysql_free_result(res);
			return str;
		}
		printf("\n");

	}
	mysql_free_result(res);
	return "";
}



void MysqlKu::ChangeDBRole(string & rDBRole,uint64_t Uid)
{
	int t;
	string str="update "+table[0]+" set role='"+rDBRole;
	str=str+"' where Uid="+to_string(Uid)+";";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("ChangeDBRole : Error1 making query");
	}
	res = mysql_use_result(&mysql);
	mysql_free_result(res);

	return;
}



void MysqlKu::ChangeDiamond(int Diamond ,uint64_t Uid)
{
	int t;
	string str="update "+table[0]+" set Diamond="+to_string(Diamond);
	str=str+" where Uid="+to_string(Uid)+";";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("ChangeDBRole : Error1 making query");
	}
	res = mysql_use_result(&mysql);
	mysql_free_result(res);

	return;
}


void MysqlKu::ChangeGole(int Gold ,uint64_t Uid)
{
	int t;
	string str="update "+table[0]+" set Gold="+to_string(Gold);
	str=str+" where Uid="+to_string(Uid)+";";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("ChangeDBRole : Error1 making query");
	}
	res = mysql_use_result(&mysql);
	mysql_free_result(res);

	return;
}


void MysqlKu::ChangeLoginTime(uint64_t Uid,int Type)
{
	int t;
	string str="call LoginChangeTime("+to_string(Uid)+","+to_string(Type)+");";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("ChangeDBRole : Error1 making query");
	}
	res = mysql_use_result(&mysql);
	mysql_free_result(res);
	return;
}

int MysqlKu::Register(const string & zhanghu,const string & mingzi)
{

	int t;
	string str="select * from "+table[0]+" where zhanghu='";
	str=str+zhanghu+"';";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("Register : Error1 making query");
	}

	res = mysql_use_result(&mysql);
	if((row = mysql_fetch_row(res)))
	{
		mysql_free_result(res);
		return 1;
	}
	mysql_free_result(res);
	str="select * from "+table[0]+" where name='";
	str=str+mingzi+"';";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("Register : Error1 making query");
	}

	res = mysql_use_result(&mysql);
	if((row = mysql_fetch_row(res)))
	{
		mysql_free_result(res);
		return 2;
	}
	mysql_free_result(res);
	return 3;
}


void MysqlKu::InsertAccount(const string & zhanghu,const string & mingzi,const string & mima,const string & DBRole)
{
	int t;
	string str="insert into "+table[0]+" (zhanghu,mima,name,role) values ('";
	str=str+zhanghu+"','"+mingzi+"','"+mima+"','"+DBRole+"');";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("InsertAccount : Error1 making query");
	}
	res = mysql_use_result(&mysql);
	mysql_free_result(res);
	return;
}


bool MysqlKu::FindUid(uint64_t userid)
{
	set<int>::iterator itera=uid_rongqi.find(userid);
	if(itera != uid_rongqi.end())
		return true;

	return false;
}



void MysqlKu::PopUid(uint64_t userid)
{
	//用户登录成功时需要从容器查找是否有该uid,如果有证明重复登录
	set<int>::iterator itera=uid_rongqi.find(userid);    
	if(itera != uid_rongqi.end())
		uid_rongqi.erase(itera);

}




uint64_t MysqlKu::GetUid(const string & zhanghu)
{
	uint64_t t;
	string str="select Uid from "+table[0]+" where zhanghu='";
	str=str+zhanghu+"';";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("GetUid : Error1 making query");
	}

	res = mysql_use_result(&mysql);
	if(res)
	{
		while((row = mysql_fetch_row(res)))
		{
			for(t=0;t < (uint64_t)mysql_num_fields(res);t++)
			{
				t=(uint64_t)atoi(row[t]);
				mysql_free_result(res);
				return t;
			}
			printf("\n");
		}
	}
	mysql_free_result(res);
	return 0;
}


