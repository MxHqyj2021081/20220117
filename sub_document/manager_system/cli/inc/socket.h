#ifndef __SOCKET__H__
#define __SOCKET__H__


#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <pthread.h>
#include <signal.h>
#include <arpa/inet.h>

#define ERR_LOG(send_msg) do{\
	fprintf(stderr,"%d %s",__LINE__,__func__);\
	perror(send_msg);\
}while(0)

#define R 1 //注册
#define L 2 //登录
#define Q 3 //退出
#define DATABASE "staff.db"
#define N 128

extern int sockfd;
extern char right;
extern char num[20];
extern int flag;

typedef struct
{
	int fd ;
	struct sockaddr_in cin;
}CliCnt;


struct msg
{
	char type;
	char name[20];//账号名
	char text[N]; //存放信息和密码
	char mod; //管理员权限和员工权限
	char sex[10];
	char age[10];
	char address[20];
	char salary[10];//薪水
	char number[20];//工号
	char phone[20];//电话号码
	int stage;//状态位
	int operate;
};


int init_socket(int* , int  , const char* );
//int modify_staff(int sfd);
//int del_staff(int sfd);
//int do_history(int sfd);
//int do_login(int sfd);
//int add_staff(int sfd);
//int do_register(int sfd);
//int do_select(int sfd);
//int do_quit(int sfd);
//int operation(int sfd);
//int modify(int sfd);
//int search_staff(int sfd);

#endif

