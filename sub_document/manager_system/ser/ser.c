#include "./inc/socket.h"
#include "./inc/sqlite.h"
#include "./inc/op.h"

void* handle_cli_msg(void* );


char name[20]="";
int main(int argc, const char *argv[])
{
	if(argc < 3)
	{
		fprintf(stderr,"请输入ip 端口号\n");
		return -1;
	}

	/*数据库初始化*/
	sqlite3* staff_db = NULL;
//	sqlite3* usr_db = NULL;
//	sqlite3* usr_db = NULL;
	if(init_sqlite(&staff_db)<0)
	{
		return -1;
	}
	printf("staff_db = %p __%d__\n", staff_db, __LINE__);
/*
	if (sqlite3_open(DATABASE, &staff_db) != SQLITE_OK)
	{
		printf("error : %s\n", sqlite3_errmsg(staff_db));
		exit(-1);
	}
*/

	/*网络初始化*/
	int sfd = 0;
	if(init_socket(&sfd, atoi(argv[2]), argv[1]) < 0)
	{
		return -1;
	}

	//多线程并发服务器
	int newfd = 0;
	struct sockaddr_in cin;
	socklen_t clen = sizeof(cin);	
	while(1)
	{
		newfd = accept(sfd,(void*)&cin,&clen);
		if(newfd < 0)
		{
			ERR_LOG("accept");
			return -1;
		}
		printf("----[%s:%d]连接成功----\n", (char*)inet_ntoa(cin.sin_addr), ntohs(cin.sin_port));

	//连接成功,创建线程维护客户端
		CliCnt cli_msg = {newfd, cin, staff_db};

		pthread_t tid;
		if(pthread_create(&tid, NULL, handle_cli_msg, &cli_msg) !=0)
		{
			ERR_LOG("pthread_create");
			return -1;
		}


	}


	return 0;
}

//服务器与客户端交互的函数
void* handle_cli_msg(void* arg)
{
	//线程分离
	pthread_detach(pthread_self());

	CliCnt cli_msg = *(CliCnt*)arg;
	int cntfd = cli_msg.fd;
	struct sockaddr_in cin = cli_msg.cin;
	sqlite3* staff_db = cli_msg.staff_db;

	int recv_len;
	struct msg recv_msg;
	while(1)
	{
		recv_len = recv(cntfd,&recv_msg,sizeof(recv_msg),0);
		if(recv_len < 0)
		{
			ERR_LOG("recv_len");
			break;
		}

		else if(0 == recv_len)
		{
			printf("----[%s:%d]断开连接----\n", (char*)inet_ntoa(cin.sin_addr), ntohs(cin.sin_port));
			break;
		}
		char type =recv_msg.type;
	//	char type = ntohl(recv_msg.type);
		printf("%d\n",type);
		switch(type)
		{
			case 'z':
				do_register(cntfd, recv_msg,staff_db);
				break;
			case 'l':
				do_login(cntfd, recv_msg, staff_db);
				break;
			case 'q':
				do_quit(cntfd, recv_msg, staff_db);
				break;
			case 'a':
				//增加
				printf("开始真假\n");
				do_add(cntfd, recv_msg, staff_db);
				break;
			case 'd':
				//删除
				del_staff(cntfd,recv_msg,staff_db);
				break;
			case 'm':
				//修改
				modify_staff(cntfd,recv_msg,staff_db);
				break;
			case 's':
				//查
				search_staff(cntfd,recv_msg,staff_db);
				break;

		}
	}

	close(cntfd);
	pthread_exit(NULL);

}







