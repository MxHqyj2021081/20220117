#ifndef __OP__H__
#define __OP__H__

#include "socket.h"




//int init_socket(int* , int  , const char* );
int modify_staff(int sfd);
int del_staff(int sfd);
int do_login(int sfd);
int add_staff(int sfd);
int do_register(int sfd);
int do_select(int sfd);
int do_quit(int sfd);
int operation(int sfd);
int modify(int sfd);
int search_staff(int sfd);

#endif

