#ifndef __OP__H__
#define __OP__H__

#include "socket.h"
#include "sqlite.h"

int modify_staff(int cntfd,struct msg recv_msg,sqlite3* staff_db);//修改                                                                       
int search_staff(int cntfd,struct msg recv_msg,sqlite3* staff_db);//查询
int del_staff(int cntfd,struct msg recv_msg,sqlite3* staff_db);
int do_add(int cntfd,struct msg recv_msg,sqlite3* );
int do_quit(int ,struct msg ,sqlite3* );
int do_login(int ,struct msg ,sqlite3* );
int do_register(int , struct msg , sqlite3* );
    
#endif

