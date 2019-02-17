
#include "BillMysql.hpp"
#include "quanju.hpp"

BillMysqlKu::BillMysqlKu()
{


}

BillMysqlKu *gs_BillMysqlKu = NULL;


BillMysqlKu * BillMysqlKu::Instance()
{
	if (NULL == gs_BillMysqlKu)
	{
		gs_BillMysqlKu = new BillMysqlKu();
	}

	return gs_BillMysqlKu;
}

int BillMysqlKu::InitBillMysql(string user,string mima,string databases,vector<string> & tables)
{
	//把从配置中读取到的表名称压入容器中
	for(int i = 0;i<(int)tables.size();i++)
	{
		table.push_back(tables[i]); 
	}
	if(table.size() == 0)
	{
		printf("Error BillInitMysql connecting to table:");
		MYLOG.printflog("BillInitMysql : Error connecting to table");
		return -1;
	}
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql,NULL,user.c_str(),mima.c_str(),databases.c_str(),0,NULL,0))
	{           
		printf("Error connecting to BillMysql database:%s\n",mysql_error(&mysql));
		return -1;
	}
	else
	{
		printf("connecting to BillMysql Success\n");
	}
	return 0;
}



void BillMysqlKu::InsertDiamondBill(uint64_t Uid,int Type,int BeginDiamond,int lastDiamond)
{
	int t;
	string str="call DiamondBillData("+to_string(Uid)+","+to_string(Type)+","
		+to_string(BeginDiamond)+","+to_string(lastDiamond)+");";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("InsertDiamondBill Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("InsertDiamondBill : Error1 making query");
	}
	res = mysql_use_result(&mysql);
	mysql_free_result(res);
	
	return;
}

void BillMysqlKu::InsertGoldBill(uint64_t Uid,int Type,int BeginGold,int lastGold)
{
	int t;
	string str="call GoldBill("+to_string(Uid)+","+to_string(Type)+","
		+to_string(BeginGold)+","+to_string(lastGold)+");";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("InsertGoldBill Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("InsertGoldBill : Error1 making query");
	}
	res = mysql_use_result(&mysql);
	mysql_free_result(res);

	return;

}
