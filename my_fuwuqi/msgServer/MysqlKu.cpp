
#include "MysqlKu.hpp"
#include "quanju.hpp"
#include "GameMysqlHandler.hpp"

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
		MYLOG.printflog("Error connecting to table:");
		return -1;
	}
	mysql_init(&mysql);
	if(!mysql_real_connect(&mysql,NULL,user.c_str(),mima.c_str(),databases.c_str(),0,NULL,0))
	{           
		printf("Error connecting to database:%s\n",mysql_error(&mysql));
		MYLOG.printflog("InitMysql : Error connecting to database");
		return -1;
	}
	else
	{
		printf("connecting to Mysql Success\n");
	}
	return 0;
}



int MysqlKu::SendRoleUpLine(struct RoleInfo* pRole,FriendList* pList,CSMsgLoginSuccessRsp * rRsp)
{
	int t;
	uint64_t uUid;
	HANDCHECH_P(pRole,-1);
	CSMsgRoleInfoList * pRoleInfo = rRsp->mutable_friendlist();
	HANDCHECH_P(pRoleInfo,-1);
	string str;
	str = "select * from msgServer where Uid in(";
	str = str + to_string(pList->frienduid(0));
	for (int j = 1; j < pList->frienduid_size();j++)
	{
		str = str + ",";
		str = str + to_string(pList->frienduid(j));
	}
	str = str + ");";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("SendRoleUpLine : Error making query");
	}
	res = mysql_use_result(&mysql);
	if(res)
	{
		while((row = mysql_fetch_row(res)))
		{
			CSMsgRoleInfo * pBaseInfo= pRoleInfo->add_roleinfo();
			HANDCHECH_P(pBaseInfo,-2);
			for(int i=0;i < (int)mysql_num_fields(res);i++)
			{
				if (i == UID)
				{
					pBaseInfo->set_uid((uint64_t)atoi(row[i]));
					uUid = (uint64_t)atoi(row[i]);
				}
				else if (i == NAME)
				{
					pBaseInfo->set_name(row[i]);
				}
				else if (i == GRADE)
				{
					pBaseInfo->set_grade((uint32_t)atoi(row[i]));
				}
				else if (i == RANK)
				{
					pBaseInfo->set_rank((uint32_t)atoi(row[i]));
				}
				else if (i == STATUS)
				{
					pBaseInfo->set_status((uint32_t)atoi(row[i]));
					if ((int)atoi(row[i]) == 1 || (int)atoi(row[i]) == 2)				//1表示在线，2表示战斗状态也属于在线
					{
						//把该用户的RoleInfo向在线的好友发送在线通知
						GameMysqlHandler::OnSendRoleUpLine(pRole,uUid);
					}
				}
				else if (i == VIP)
				{
					pBaseInfo->set_vip((uint32_t)atoi(row[i]));
				}
				else if (i == HEAD)
				{
					pBaseInfo->set_head((uint32_t)atoi(row[i]));
				}
				else if (i == CHATFRAME)
				{
					pBaseInfo->set_chatframe((uint32_t)atoi(row[i]));
				}
			}

		}
	}

	return 0;
}



void MysqlKu::MsgLoginSuccess(uint64_t Uid,int Status,CSMsgLoginSuccessRsp * rRsp)
{
	struct RoleInfo rRole;			//保存用户的基本信息
	FriendList FriendUidList;		//protobuf结构体，该结构是用来反序列化拿到所有好友UID
	string str="call MsgLoginSuccess("+to_string(Uid)+","+to_string(Status)+");";
	int t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("MsgLoginSuccess : Error making query");
	}
	res = mysql_use_result(&mysql);
	if(res)
	{
		//拿到该用户的所有字段的数据
		if((row = mysql_fetch_row(res)))
		{
			for(int i=0;i < (int)mysql_num_fields(res);i++)
			{
				if (i == UID)
				{
					rRole.Uid = (uint64_t)atoi(row[i]);
				}
				else if (i == NAME)
				{
					rRole.Name = row[i];
				}
				else if (i == GRADE)
				{
					rRole.Grade = (uint32_t)atoi(row[i]);
				}
				else if (i == RANK)
				{
					rRole.Rank = (uint32_t)atoi(row[i]);
				}
				else if (i == STATUS)
				{
					rRole.Status = (uint32_t)atoi(row[i]);
				}
				else if (i == VIP)
				{
					rRole.Vip = (uint32_t)atoi(row[i]);
				}
				else if (i == HEAD)
				{
					rRole.Head = (uint32_t)atoi(row[i]);
				}
				else if (i == CHATFRAME)
				{
					rRole.ChatFrame = (uint32_t)atoi(row[i]);
				}
				else if (i == FRIENDLIST)
				{
					//把所有好友列表UID字段取出来，然后反序列化成protobuf数据
					if (row[i] != NULL)
					{
						if (!FriendUidList.ParseFromArray(row[i],strlen(row[i])))
						{
							std::cout << "Msg Friend ParseFromArray Fail！" << std::endl;
							MYLOG.printflog("Msg Friend ParseFromArray Fail！");
							mysql_free_result(res);
							return;
						}
						//拿该用户上线的好友的基本信息
						if (FriendUidList.frienduid_size() > 0)
						{
							//向好友广播上线通知
							mysql_free_result(res);
							SendRoleUpLine(&rRole,&FriendUidList,rRsp);
							break;
						}
						
					}
					
				}
			}
			printf("\n");
		}
	}
	mysql_free_result(res);

	return;
}




int MysqlKu::SendRoleQuitLine(struct RoleInfo* pRole,FriendList* pList)
{
	int t;
	HANDCHECH_P(pRole,-1);
	string str;
	str = "select Uid from msgServer where Uid in(";
	str = str + to_string(pList->frienduid(0));
	for (int j = 1; j < pList->frienduid_size();j++)
	{
		str = str + ",";
		str = str + to_string(pList->frienduid(j));
	}
	str = str + ") and Status in(1,2);";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("SendRoleQuitLine : Error making query");
	}
	res = mysql_use_result(&mysql);
	if(res)
	{
		while((row = mysql_fetch_row(res)))
		{
		//把该用户的RoleInfo向在线的好友发送离线通知
		GameMysqlHandler::OnSendRoleQuitLine(pRole,(uint64_t)atoi(row[0]));
						
		}

	  
	}

	return 0;
}



void MysqlKu::MsgQuitLine(uint64_t Uid,int Status)
{
	struct RoleInfo rRole;			//保存用户的基本信息
	FriendList FriendUidList;		//protobuf结构体，该结构是用来反序列化拿到所有好友UID
	string str="call MsgLoginSuccess("+to_string(Uid)+","+to_string(Status)+");";
	int t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("MsgQuitLine : Error making query");
	}
	res = mysql_use_result(&mysql);
	if(res)
	{
		//拿到该用户的所有字段的数据
		if((row = mysql_fetch_row(res)))
		{
			for(int i=0;i < (int)mysql_num_fields(res);i++)
			{
				if (i == UID)
				{
					rRole.Uid = (uint64_t)atoi(row[i]);
				}
				else if (i == NAME)
				{
					rRole.Name = row[i];
				}
				else if (i == GRADE)
				{
					rRole.Grade = (uint32_t)atoi(row[i]);
				}
				else if (i == RANK)
				{
					rRole.Rank = (uint32_t)atoi(row[i]);
				}
				else if (i == STATUS)
				{
					rRole.Status = (uint32_t)atoi(row[i]);
				}
				else if (i == VIP)
				{
					rRole.Vip = (uint32_t)atoi(row[i]);
				}
				else if (i == HEAD)
				{
					rRole.Head = (uint32_t)atoi(row[i]);
				}
				else if (i == CHATFRAME)
				{
					rRole.ChatFrame = (uint32_t)atoi(row[i]);
				}
				else if (i == FRIENDLIST)
				{
					//把所有好友列表UID字段取出来，然后反序列化成protobuf数据
					if (row[i] != NULL)
					{
						if (!FriendUidList.ParseFromArray(row[i],strlen(row[i])))
						{
							std::cout << "Msg Friend ParseFromArray Fail！" << std::endl;
							MYLOG.printflog("Msg Friend ParseFromArray Fail！");
							mysql_free_result(res);
							return;
						}
						//拿该用户上线的好友的基本信息
						if (FriendUidList.frienduid_size() > 0)
						{
							//向好友广播离线通知
							mysql_free_result(res);
							SendRoleQuitLine(&rRole,&FriendUidList);
							break;
						}

					}

				}
			}
			printf("\n");
		}
	}
	mysql_free_result(res);

	return;
}



int MysqlKu::UpFriendList(uint64_t Uid,FriendList* pList)
{
	//序列化好友数组存入数据库中
	int t;
	int len = pList->ByteSize();
	char buff[len+1];
	if (!pList->SerializeToArray(buff,len)) 
	{ 
		std::cout << "UpFriendList serialize Fail!" << std::endl; 
		MYLOG.printflog("UpFriendList serialize Fail！"); 
		return -1;
	}
	string sFriendArray(buff);
	string str="update "+table[0]+" set Friend='"+sFriendArray;
	str=str+"' where Uid="+to_string(Uid)+";";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("UpFriendList : Error making query");
	}
	res = mysql_use_result(&mysql);
	return 0;
}


int MysqlKu::MsgSuccessAddFriend(uint64_t Uid,uint64_t Uid1,CSMsgSuccessAddFriendRsp * rRsp)
{
	CSMsgRoleInfo * pRoleInfo = rRsp->mutable_friendroleinfo();
	HANDCHECH_P(pRoleInfo,-1);
	FriendList FriendUidList;		//protobuf结构体，该结构是用来反序列化拿到所有好友UID
	string str="select * from "+table[0]+" where Uid="+to_string(Uid)+";";
	int t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("MsgSuccessAddFriend : Error making query");
	}
	res = mysql_use_result(&mysql);
	if(res)
	{
		//拿到该用户的所有字段的数据
		if((row = mysql_fetch_row(res)))
		{
			for(int i=0;i < (int)mysql_num_fields(res);i++)
			{
				if (i == UID)
				{
					pRoleInfo->set_uid((uint64_t)atoi(row[i]));
				}
				else if (i == NAME)
				{
					pRoleInfo->set_name(row[i]);
				}
				else if (i == GRADE)
				{
					pRoleInfo->set_grade((uint64_t)atoi(row[i]));
				}
				else if (i == RANK)
				{
					pRoleInfo->set_rank((uint64_t)atoi(row[i]));
				}
				else if (i == STATUS)
				{
					pRoleInfo->set_status((uint64_t)atoi(row[i]));
				}
				else if (i == VIP)
				{
					pRoleInfo->set_vip((uint64_t)atoi(row[i]));
				}
				else if (i == HEAD)
				{
					pRoleInfo->set_head((uint64_t)atoi(row[i]));
				}
				else if (i == CHATFRAME)
				{
					pRoleInfo->set_chatframe((uint64_t)atoi(row[i]));
				}
				else if (i == FRIENDLIST)
				{
					//把所有好友列表UID字段取出来，然后反序列化成protobuf数据
					if (row[i] != NULL)
					{
						if (!FriendUidList.ParseFromArray(row[i],strlen(row[i])))
						{
							std::cout << "Msg Friend ParseFromArray Fail！" << std::endl;
							MYLOG.printflog("Msg Friend ParseFromArray Fail！");
							mysql_free_result(res);
							return -1;
						}
						//添加新的好友到protobuf数组中
						FriendList NewFriendUidList;
						NewFriendUidList.add_frienduid(Uid1);
						for (int j = 0; j < FriendUidList.frienduid_size(); j++)
						{
							NewFriendUidList.add_frienduid(FriendUidList.frienduid(j));
						}
						//把新的protobuf数组进行序列化存入数组中
						mysql_free_result(res);
						UpFriendList(Uid,&NewFriendUidList);
						break;
					}

				}
			}
			printf("\n");
		}
	}
	mysql_free_result(res);

	return 0;
}




void MysqlKu::ChangeFriendListNULL(uint64_t Uid)
{
	int t;
	string str="update "+table[0]+" set Friend=NULL";
	str=str+" where Uid="+to_string(Uid)+";";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("ChangeFriendListNULL : Error making query");
	}
	res = mysql_use_result(&mysql);
	mysql_free_result(res);
	return;
}


void MysqlKu::MsgDeleteFriend(uint64_t Uid,uint64_t Uid1)
{
	FriendList FriendUidList;		//protobuf结构体，该结构是用来反序列化拿到所有好友UID
	string str="select Friend from "+table[0]+" where Uid="+to_string(Uid)+";";
	int t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("MsgDeleteFriend : Error making query");
	}
	res = mysql_use_result(&mysql);
	if(res)
	{
		//拿到该用户的所有字段的数据
		if((row = mysql_fetch_row(res)))
		{
			//把所有好友列表UID字段取出来，然后反序列化成protobuf数据
			if (row[0] != NULL)
			{
				if (!FriendUidList.ParseFromArray(row[0],strlen(row[0])))
				{
					std::cout << "Msg Friend ParseFromArray Fail！" << std::endl;
					MYLOG.printflog("Msg Friend ParseFromArray Fail！");
					mysql_free_result(res);
					return;
				}
				//添加新的好友到protobuf数组中
				FriendList NewFriendUidList;
				NewFriendUidList.add_frienduid(Uid1);
				for (int j = 0; j < FriendUidList.frienduid_size(); j++)
				{
					if (FriendUidList.frienduid(j) != Uid1)
					{
						NewFriendUidList.add_frienduid(FriendUidList.frienduid(j));
					}
					
				}
				if (NewFriendUidList.frienduid_size() == 0)
				{
					//特殊情况，就是说他没有好友了，好友列表为空,更改好友列表为NULL
					mysql_free_result(res);
					ChangeFriendListNULL(Uid);
				}
				else
				{
					//把新的protobuf数组进行序列化存入数组中
					mysql_free_result(res);
					UpFriendList(Uid,&NewFriendUidList);
				}
				
			}			

		}
			printf("\n");
		}

	mysql_free_result(res);
	return;
}



int MysqlKu::MsgFindName(const string & name,CSMsgFindNameRsp * pRsp)
{
	CSMsgRoleInfo * pRoleInfo = pRsp->mutable_roleinfo();
	HANDCHECH_P(pRoleInfo,-1);
	string str="select * from "+table[0]+" where Name='"+name+"';";
	int t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("MsgFindName : Error making query");
	}
	res = mysql_use_result(&mysql);
	if(res)
	{
		if((row = mysql_fetch_row(res)))
		{
			for(int i=0;i < (int)mysql_num_fields(res);i++)
			{
				if (i == UID)
				{
					pRoleInfo->set_uid((uint64_t)atoi(row[i]));
				}
				else if (i == NAME)
				{
					pRoleInfo->set_name(row[i]);
				}
				else if (i == GRADE)
				{
					pRoleInfo->set_grade((uint64_t)atoi(row[i]));
				}
				else if (i == RANK)
				{
					pRoleInfo->set_rank((uint64_t)atoi(row[i]));
				}
				else if (i == STATUS)
				{
					pRoleInfo->set_status((uint64_t)atoi(row[i]));
				}
				else if (i == VIP)
				{
					pRoleInfo->set_vip((uint64_t)atoi(row[i]));
				}
				else if (i == HEAD)
				{
					pRoleInfo->set_head((uint64_t)atoi(row[i]));
				}
				else if (i == CHATFRAME)
				{
					pRoleInfo->set_chatframe((uint64_t)atoi(row[i]));
				}
				
			}
			printf("\n");
		}
		
	}
	mysql_free_result(res);
	return 0;
}



int MysqlKu::SendRoleChangeStatus(struct RoleInfo* pRole,FriendList* pList)
{
	int t;
	HANDCHECH_P(pRole,-1);
	string str;
	str = "select Uid from msgServer where Uid in(";
	str = str + to_string(pList->frienduid(0));
	for (int j = 1; j < pList->frienduid_size();j++)
	{
		str = str + ",";
		str = str + to_string(pList->frienduid(j));
	}
	str = str + ") and Status in(1,2);";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("SendRoleChangeStatus : Error making query");
	}
	res = mysql_use_result(&mysql);
	if(res)
	{
		while((row = mysql_fetch_row(res)))
		{
			//把该用户的RoleInfo向在线的好友发送离线通知
			GameMysqlHandler::OnSendChangeStatus(pRole,(uint64_t)atoi(row[0]));

		}


	}

	return 0;
}


int MysqlKu::MsgChangeStatus(uint64_t Uid,uint32_t value,uint32_t type)
{
	struct RoleInfo rRole;			//保存用户的基本信息
	FriendList FriendUidList;		//protobuf结构体，该结构是用来反序列化拿到所有好友UID
	string str="call MsgLoginSuccess("+to_string(Uid)+","+to_string(value)+","+to_string(type)+");";
	int t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("MsgChangeStatus : Error making query");
	}
	res = mysql_use_result(&mysql);
	if(res)
	{
		//拿到该用户的所有字段的数据
		if((row = mysql_fetch_row(res)))
		{
			for(int i=0;i < (int)mysql_num_fields(res);i++)
			{
				if (i == UID)
				{
					rRole.Uid = (uint64_t)atoi(row[i]);
				}
				else if (i == NAME)
				{
					rRole.Name = row[i];
				}
				else if (i == GRADE)
				{
					rRole.Grade = (uint32_t)atoi(row[i]);
				}
				else if (i == RANK)
				{
					rRole.Rank = (uint32_t)atoi(row[i]);
				}
				else if (i == STATUS)
				{
					rRole.Status = (uint32_t)atoi(row[i]);
				}
				else if (i == VIP)
				{
					rRole.Vip = (uint32_t)atoi(row[i]);
				}
				else if (i == HEAD)
				{
					rRole.Head = (uint32_t)atoi(row[i]);
				}
				else if (i == CHATFRAME)
				{
					rRole.ChatFrame = (uint32_t)atoi(row[i]);
				}
				else if (i == FRIENDLIST)
				{
					//把所有好友列表UID字段取出来，然后反序列化成protobuf数据
					if (row[i] != NULL)
					{
						if (!FriendUidList.ParseFromArray(row[i],strlen(row[i])))
						{
							std::cout << "Msg Friend ParseFromArray Fail！" << std::endl;
							MYLOG.printflog("Msg Friend ParseFromArray Fail！");
							mysql_free_result(res);
							return 0;
						}
						//拿该用户上线的好友的基本信息
						if (FriendUidList.frienduid_size() > 0)
						{
							//向好友广播状态改变通知
							mysql_free_result(res);
							SendRoleChangeStatus(&rRole,&FriendUidList);
							break;
						}

					}

				}
			}
			printf("\n");
		}
	}
	mysql_free_result(res);

	return 0;
}



void MysqlKu::MsgInsertAccount(uint64_t Uid,const string & name)
{
	int t;
	string str="insert into "+table[0]+" (Uid,Name) values (";
	str=str+to_string(Uid)+",'"+name+"');";
	t=mysql_query(&mysql,str.c_str());
	if(t)
	{
		printf("Error making query:%s\n",mysql_error(&mysql));
		MYLOG.printflog("MsgChangeStatus : Error making query");
	}
	res = mysql_use_result(&mysql);
	mysql_free_result(res);
}

