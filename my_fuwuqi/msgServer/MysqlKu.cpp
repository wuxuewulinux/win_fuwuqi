
#include "MysqlKu.hpp"

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
	int uid;
	string write_mima;
	string str="select Uid,mima from "+table[0]+" where zhanghu='";
	str=str+zhanghu+"';";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error1 making query:%s\n",mysql_error(&mysql));
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

	return 0;
}



string MysqlKu::GetName(int Uid)
{
	int t;
	string str="select name from "+table[0]+" where Uid='";
	str=str+std::to_string(Uid)+"';";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
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
	return "";
}



string MysqlKu::GetDBRole(int Uid)
{
	int t;
	string str="select role from "+table[0]+" where Uid=";
	str=str+to_string(Uid)+";";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
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
	return "";
}



void MysqlKu::ChangeDBRole(string & rDBRole,int Uid)
{
	int t;
	string str="update "+table[0]+" set role='"+rDBRole;
	str=str+"' where Uid="+to_string(Uid)+";";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
	}
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
	}

	res = mysql_use_result(&mysql);
	if((row = mysql_fetch_row(res)))
	{
		mysql_free_result(res);
		return 1;
	}
	mysql_free_result(res);
	str="select * from "+table[0]+" where mima='";
	str=str+mingzi+"';";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
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

/*
int MysqlKu::GetJiFen(const string & zhanghu)
{
	int t;
	string str="select jifen from "+table[0]+" where zhanghu='";
	str=str+zhanghu+"';";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
	}

	res = mysql_use_result(&mysql);
	if(res)
	{
		while((row = mysql_fetch_row(res)))
		{
			for(t=0;t<mysql_num_fields(res);t++)
			{
				t=atoi(row[t]);
				mysql_free_result(res);
				return t;
			}
			printf("\n");
		}
	}
	return 0;
}
*/


void MysqlKu::InsertAccount(const string & zhanghu,const string & mingzi,const string & mima,const string & DBRole)
{
	int t;
	string str="insert into "+table[0]+" (zhanghu,mima,name,role) values ('";
	str=str+zhanghu+"','"+mingzi+"','"+mima+"','"+DBRole+"');";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
	}
	mysql_free_result(res);
}


/*
void MysqlKu::ChangeJiFen(int jifen,const string & zhanghu)
{
	int t;
	char s[10];
	sprintf(s,"%d",jifen);
	string str="update "+table[0]+" set jifen="+s;
	str=str+" where zhanghu='"+zhanghu+"';";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
	}
	mysql_free_result(res);
}
*/

/*
先屏蔽掉，元宝或金币日志。
void MysqlKu::InsertGoldLog(const struct goldlog & gold_log)
{
	timename[timename.size()-1]=' ';
	string begingold=to_string(gold_log.begingold);
	string lastgold=to_string(gold_log.lastgold);
	string str="insert into "+table+" (datetime,zhanghao,homename,begingold,lastgold) values ('";
	str=str+timename+"','"+gold_log.zhanghao+"','"+gold_log.homename+"',"+begingold+","+lastgold+");";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
	}
	mysql_free_result(res);

}
*/

bool MysqlKu::FindUid(int userid)
{
	set<int>::iterator itera=uid_rongqi.find(userid);
	if(itera != uid_rongqi.end())
		return true;

	return false;
}



void MysqlKu::PopUid(int userid)
{

	set<int>::iterator itera=uid_rongqi.find(userid);    //用户登录成功时需要从容器查找是否有该uid,如果有证明重复登录
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
	return 0;
}


/*
void  MysqlKu::cheshi()
{
	int t;
	string str="select * from "+table[0];
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error1 making query:%s\n",mysql_error(&mysql));
	}

	res = mysql_use_result(&mysql);
	//获取表中的列数
	int rows;
	rows=mysql_num_fields(res);
	MYSQL_FIELD * my;                     //声明一个接收字段属性的类型
	my=mysql_fetch_fields(res);          //返回该表的所有字段属性
	std::cout<<"row="<<rows<<std::endl;
	for(int i=0;i<rows;i++)
	{
		int j=my[i].type;
		std::cout<<j<<std::endl;
		std::cout<<my[i].type<<std::endl;
	}
}


void  MysqlKu::cheshitable()
{
	for(int i=0;i<table.size();i++)
		cout<<table[i]<<endl;

}
*/