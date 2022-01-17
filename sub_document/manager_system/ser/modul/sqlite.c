#include "../inc/socket.h"
#include "../inc/sqlite.h"



//数据库初始化
int init_sqlite(sqlite3** pstaff_db)
{
	char sql[N] = "";
	char* errmsg = NULL;
	if(sqlite3_open("./staff_db", pstaff_db)!=0)
	{
		printf("sqlite3_open:%s %d\n", sqlite3_errmsg(*pstaff_db), __LINE__);
		return -1;
	}

	bzero(sql, sizeof(sql));
	sprintf(sql,"create table if not exists staff(mod char,name char,text char,sex char,age int,address char,salary int,number char primary key,phone char,stage int)");
	if(sqlite3_exec(*pstaff_db, sql, NULL, NULL, &errmsg) != 0)
	{
		printf("sqlite3_exec:%s %d\n", errmsg, __LINE__);
		return -1;
	}
	printf("员工数据库打开成功\n");
	printf("staff_db = %p __%d__\n", *pstaff_db, __LINE__);

	bzero(sql, sizeof(sql));
	sprintf(sql,"update staff set stage=0;");
	if(sqlite3_exec(*pstaff_db, sql, NULL, NULL, &errmsg) != 0)
	{
		printf("sqlite3_exec:%s %d\n", errmsg, __LINE__);
		return -1;
	}
	printf("用户状态清空成功\n");

	return 0;
}

