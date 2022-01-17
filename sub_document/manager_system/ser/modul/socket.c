
#include "../inc/socket.h"


//网络初始化
int init_socket(int* psfd, int port , const char* ip)
{
	//socket
	*psfd = socket(AF_INET, SOCK_STREAM, 0);
	if(*psfd < 0)
	{
		ERR_LOG("socket");
		return -1;
	}

	//允许端口快速重用
	int values = 1;
	if(setsockopt(*psfd, SOL_SOCKET, SO_REUSEADDR, &values, sizeof(int))<0)
	{
		ERR_LOG("setsockopt");
		return -1;
	}

	//bind
	struct sockaddr_in sin;
	sin.sin_family 		= AF_INET;
	sin.sin_port  		= htons(port);
	sin.sin_addr.s_addr = inet_addr(ip);

	if(bind(*psfd, (void*)&sin, sizeof(sin)) < 0)
	{
		ERR_LOG("bind");
		return -1;
	}

	//listen
	if(listen(*psfd, 5) < 0)
	{
		ERR_LOG("listen");
		return -1;
	}
	
	return 0;
}


