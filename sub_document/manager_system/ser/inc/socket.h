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
#include <time.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>        

#define ERR_LOG(msg) do{\
    fprintf(stderr, "%d %s ", __LINE__, __func__);\
    perror(msg);\
}while(0)

#define N 256

#define R   1       //注册
#define L   2       //登录
#define Q   3       //退出
                                                                                                                       
#define DATABASE "staff.db"
typedef struct
{
    int fd ;
    struct sockaddr_in cin;
    sqlite3* staff_db;

}CliCnt;

struct msg
{
    char type;
    char name[20];
    char text[128];

    char mod;//权限
    char sex[10];
    char age[10];
    char address[20];
    char salary[10];
    char number[20];//工号
    char phone[20];
    int stage;
    int operate;
};

extern char name[20];

int init_socket(int* , int  , const char* );     

#endif
