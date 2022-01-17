#include "./inc/socket.h"
#include "./inc/op.h"


int sockfd = 0;
char right = 0;
char num[20]="";
int flag=0;      

typedef void(*sighandler_t)(int);
void handler(int sig)
{
	do_quit(sockfd);
	close(sockfd);
	exit(0);
}

int main(int argc, const char *argv[])
{
	
	sighandler_t s = signal(2,handler);
	if(argc < 3)
	{
		fprintf(stderr,"请输入ip 端口号\n");
		return -1;
	}

	//给ctrl+c注册信号处理函数
	if(s == SIG_ERR)
	{
		ERR_LOG("signal");
		return -1;
	}

	/*网络初始化*/
	int sfd = 0;
	if(init_socket(&sfd,atoi(argv[2]),argv[1]) < 0)
	{
		return -1;
	}

	sockfd = sfd;

	while(1)
	{
		system("clear");
		printf("****************\n");	
		printf("*****1.注册*****\n");	
		printf("*****2.登录*****\n");	
		printf("*****3.退出*****\n");	
		printf("****************\n");

		int choose = 0;
		scanf("%d",&choose);
		while(getchar()!=10);

		switch(choose)
		{
		case R:
			do_register(sfd);
			break;
		case L:
			do_login(sfd);
			if(flag == 1)
			{
				operation(sfd);
			}
			break;
		case Q:
		//	do_quit();
			close(sfd);
			exit(0);
		}

		printf("请输入任意字符清屏>>>");
		while(getchar()!=10);
	}
	return 0;
}







