#include "../inc/op.h"

//注册
int do_register(int cntfd, struct msg recv_msg, sqlite3 *staff_db)
{
	char* errmsg = NULL;
	char sql[N] = "";
	sprintf(sql, "insert into staff(number,name,text,mod) values(\"%s\",\"%s\",\"%s\",\"%c\");",recv_msg.number, recv_msg.name, recv_msg.text,recv_msg.mod);
	printf("%s\n", sql);

	printf("staff_db = %p __%d__\n", staff_db, __LINE__);
	if(sqlite3_exec(staff_db, sql, NULL, NULL, &errmsg) !=SQLITE_OK)
	{
	//	printf("sqlite3_exec:%s __%d__\n", errmsg, __LINE__);
		if(strcmp("UNIQUE constraint failed: staff.number",errmsg)==0)	
		{
			strcpy(recv_msg.text, "**EXISTS**");
		}
		else
		{
			strcpy(recv_msg.text, "**ERROR**");
		}
	
	}
	else
	{
		printf("注册成功\n");
		strcpy(recv_msg.text, "**OK**");

		sprintf(sql,"update staff set stage=0 where number=\"%s\";",recv_msg.number);
		if(sqlite3_exec(staff_db,sql,NULL,NULL,&errmsg)!=0)
		{
			printf("%s  __%d__ \n",errmsg,__LINE__);
			return -1;
		}
	}

	if(send(cntfd, &recv_msg, sizeof(recv_msg), 0) <0)
	{
		ERR_LOG("send");
		return -1;
	}
	return 0;
}

//登录
int do_login(int cntfd, struct msg staff_msg, sqlite3 *staff_db)
{
	char sql[256]="";
	char *errmsg=NULL;
	bzero(sql,256);
	char **pres = NULL;
	int row,column;
	sprintf(sql,"select * from staff where number=\"%s\" and text=\"%s\"",staff_msg.number,staff_msg.text);
	strcpy(name,staff_msg.number);	
	printf("%c\n",staff_msg.mod);
	if(sqlite3_get_table(staff_db,sql,&pres,&row,&column,&errmsg)!=SQLITE_OK)
	{
		printf("没有此员工\n");
		return -1;
	}
	if(row == 0)
	{
		strcpy(staff_msg.text,"notexits");
	}
	else
	{
		if(strcmp(pres[(row+1)*column-1],"0")!=0)
		{
			strcpy(staff_msg.text,"exists");
		}
		else
		{
			strcpy(staff_msg.text,"OK");
			printf("成功登录\n");
			bzero(sql,256);
			sprintf(sql,"update staff set stage=1 where number=\"%s\"",staff_msg.number);
			if(sqlite3_exec(staff_db,sql,NULL,NULL,&errmsg)!=0)
			{
				printf("%s\n",errmsg);
				return -1;
			}
		}

	}

	if(send(cntfd,&staff_msg,sizeof(staff_msg),0)<0)
	{
		ERR_LOG("send");
		return -1;
	}
    sqlite3_free_table(pres);       

}


//增加
int do_add(int cfd,struct msg staff_msg,sqlite3* staff_db)
{
	char **pres = NULL;
	int row,column;
	char* errmsg = NULL;
	
	printf("进入增加函数\n");
	//组一个sql语句
	char sql[256]="";
	bzero(sql,256);
	sprintf(sql,"select * from staff where number=\"%s\"",staff_msg.number);
	printf("正在添加工号%s员工的信息\n",staff_msg.number);
	if(sqlite3_get_table(staff_db,sql,&pres,&row,&column,&errmsg)!=0)
	{
		printf("用户未注册\n");
		return -1;
	}
	if(row == 0)
	{
		strcpy(staff_msg.text,"notexits");
		if(send(cfd,&staff_msg,sizeof(staff_msg),0)<0)
		{
			ERR_LOG("send");
			return -1;
		}
		printf("没有这个员工\n");
	}
	else
	{
		if(staff_msg.mod == 'r')
		{
			printf("开始增加信息\n");
			sprintf(sql,"update staff set sex=\"%s\",age=\"%s\",address=\"%s\",phone=\"%s\",salary=\"%s\" where number=\"%s\"", staff_msg.sex,staff_msg.age,staff_msg.address,staff_msg.phone,staff_msg.salary,staff_msg.number);         
//			staff_msg.sex,staff_msg.age,staff_msg.address,staff_msg.phone,staff_msg.number,staff_msg.salary);
//			sprintf(sql,"update staff set sex=\"%s\",age=\"%s\",address=\"%s\",phone=\"%s\" where number=\"%s\"",\
//					staff_msg.sex,staff_msg.age,staff_msg.address,staff_msg.phone,staff_msg.number);

			if(sqlite3_exec(staff_db,sql,NULL,NULL,&errmsg)!=0)
			{
				printf("%s\n",errmsg);
				return -1;
			}	
		}
		strcpy(staff_msg.text,"OK");
		if(send(cfd,&staff_msg,sizeof(staff_msg),0)<0)
		{
			ERR_LOG("send");
			return -1;
		}
		printf("增加信息成功\n");

	}

	sqlite3_free_table(pres);
	return 0;
}


//删除
int	del_staff(int cfd,struct msg staff_msg,sqlite3* staff_db)
{
	char **pres = NULL;
	int row,column;
	char* errmsg = NULL;
	
	printf("进入服务器端删除函数\n");
	//组一个sql语句
	char sql[256]="";
	bzero(sql,256);
	sprintf(sql,"select * from staff where number=\"%s\"",staff_msg.number);
	printf("正在删除工号%s员工的信息\n",staff_msg.number);
	if(sqlite3_get_table(staff_db,sql,&pres,&row,&column,&errmsg)!=0)
	{
		printf("用户未注册\n");
		return -1;
	}
	if(row == 0)
	{
		strcpy(staff_msg.text,"notexits");
		if(send(cfd,&staff_msg,sizeof(staff_msg),0)<0)
		{
			ERR_LOG("send");
			return -1;
		}
		printf("没有这个员工\n");
	}
	else
	{
		if(staff_msg.mod == 'r')
		{
			printf("开始删除信息\n");
			sprintf(sql,"delete from staff where number=\"%s\"", staff_msg.number);         
//			staff_msg.sex,staff_msg.age,staff_msg.address,staff_msg.phone,staff_msg.number,staff_msg.salary);
//			sprintf(sql,"update staff set sex=\"%s\",age=\"%s\",address=\"%s\",phone=\"%s\" where number=\"%s\"",\
//					staff_msg.sex,staff_msg.age,staff_msg.address,staff_msg.phone,staff_msg.number);

			if(sqlite3_exec(staff_db,sql,NULL,NULL,&errmsg)!=0)
			{
				printf("%s\n",errmsg);
				return -1;
			}	
		}
		strcpy(staff_msg.text,"OK");
		if(send(cfd,&staff_msg,sizeof(staff_msg),0)<0)
		{
			ERR_LOG("send");
			return -1;
		}
		printf("删除信息成功\n");

	}
		return 0;

}

//修改
int	modify_staff(int cfd,struct msg staff_msg,sqlite3* staff_db)
{
	char **pres = NULL;
	int row,column;
	char* errmsg = NULL;
	
	printf("进入服务器端修改函数\n");
	//组一个sql语句
	char sql[256]="";
	bzero(sql,256);
	sprintf(sql,"select * from staff where number=\"%s\"",staff_msg.number);
	printf("正在修改工号%s员工的信息\n",staff_msg.number);
	if(sqlite3_get_table(staff_db,sql,&pres,&row,&column,&errmsg)!=0)
	{
		printf("用户未注册\n");
		return -1;
	}
	if(row == 0)
	{
		strcpy(staff_msg.text,"notexits");
		if(send(cfd,&staff_msg,sizeof(staff_msg),0)<0)
		{
			ERR_LOG("send");
			return -1;
		}
		printf("没有这个员工\n");
	}

	else
	{
		if(staff_msg.mod == 'r')
		{
			printf("管理员开始修改员工信息\n");
			sprintf(sql,"update staff set name=\"%s\",sex=\"%s\",age=\"%s\",address=\"%s\",phone=\"%s\",salary=\"%s\" where number=\"%s\"", staff_msg.name,staff_msg.sex,staff_msg.age,staff_msg.address,staff_msg.phone,staff_msg.salary,staff_msg.number);         

			if(sqlite3_exec(staff_db,sql,NULL,NULL,&errmsg)!=0)
			{
				printf("%s\n",errmsg);
				return -1;
			}	
		}
		else if(staff_msg.mod == 'p')
		{
			printf("员工开始修改个人信息\n");
			sprintf(sql,"update staff set name=\"%s\",sex=\"%s\",address=\"%s\",phone=\"%s\" where number=\"%s\"", staff_msg.name,staff_msg.sex,staff_msg.address,staff_msg.phone,staff_msg.number);         

			if(sqlite3_exec(staff_db,sql,NULL,NULL,&errmsg)!=0)
			{
				printf("%s\n",errmsg);
				return -1;
			}	
		}

		strcpy(staff_msg.text,"OK");
		if(send(cfd,&staff_msg,sizeof(staff_msg),0)<0)
		{
			ERR_LOG("send");
			return -1;
		}
		printf("修改信息成功\n");
	}

	sqlite3_free_table(pres);
	return 0;
}


//查询
int	search_staff(int cfd,struct msg staff_msg,sqlite3* staff_db)
{
	char **pres = NULL;
	int row,column;
	char* errmsg = NULL;
	int line = 0, list=0, j=0;

	printf("进入服务器端查询函数\n");
	//组一个sql语句
	char sql[256]="";
	bzero(sql,256);
	sprintf(sql,"select * from staff where number=\"%s\"",staff_msg.number);
	printf("正在查询工号%s员工的信息\n",staff_msg.number);
	if(sqlite3_get_table(staff_db,sql,&pres,&row,&column,&errmsg)!=0)
	{
		printf("用户未注册\n");
		return -1;
	}
	if(row == 0)
	{
		strcpy(staff_msg.text,"notexits");
		if(send(cfd,&staff_msg,sizeof(staff_msg),0)<0)
		{
			ERR_LOG("send");
			return -1;
		}
		printf("没有这个员工\n");
	}
	else
	{
		if(staff_msg.mod == 'r')
		{
			printf("管理员开始查询员工信息\n");
			sprintf(sql,"select mod,name,sex,age,address,phone,salary from staff where number=\"%s\"",staff_msg.number);         

			if(sqlite3_get_table(staff_db,sql,&pres,&row,&column,&errmsg)!=0)
			{
				printf("%s\n",errmsg);
				return -1;
			}
			sprintf(staff_msg.text, "mod:%s name:%s sex:%s age:%s address:%s phone:%s salary:%s",\
						pres[column], pres[column+1], pres[column+2],pres[column+3],pres[column+4],pres[column+5],pres[column+6]);

			if(send(cfd,&staff_msg,sizeof(staff_msg),0)<0)
			{
				ERR_LOG("send");
				return -1;
			}
			printf("管理员查询员工信息成功\n");
		}
		else if(staff_msg.mod == 'p')
		{
			printf("员工开始查询自己的个人信息\n");
			sprintf(sql,"select mod,name,sex,age,address,phone,salary from staff where number=\"%s\"",staff_msg.number);         

			if(sqlite3_get_table(staff_db,sql,&pres,&row,&column,&errmsg)!=0)
			{
				printf("%s\n",errmsg);
				return -1;
			}
			sprintf(staff_msg.text, "mod:%s name:%s sex:%s age:%s address:%s phone:%s salary:%s",\
						pres[column], pres[column+1], pres[column+2],pres[column+3],pres[column+4],pres[column+5],pres[column+6]);	

			if(send(cfd,&staff_msg,sizeof(staff_msg),0)<0)
			{
				ERR_LOG("send");
				return -1;
			}
			printf("员工查询个人信息成功\n");
		}
	}
	return 0;
}	

//退出
int do_quit(int cfd,struct msg staff_msg,sqlite3* staff_db)
{
	char sql[256]="";
	char *errmsg=NULL;
	sprintf(sql,"update staff set stage=0 where number=\"%s\";",staff_msg.number);
	
	if(sqlite3_exec(staff_db,sql,NULL,NULL,&errmsg)!=0)
	{
		printf("%s  __%d__ \n",errmsg,__LINE__);
		return -1;
	}	
	printf("工号%s:退出\n",staff_msg.number);
//	close(cfd);
	return 0;

}



