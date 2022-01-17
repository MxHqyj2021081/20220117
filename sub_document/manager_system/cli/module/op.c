#include "../inc/socket.h"
#include "../inc/op.h"


//操作
int operation(int sfd)
{
    while(1)
    {
err:
        printf("*********************\n");
        printf("********0.增*********\n");
        printf("********1.删*********\n");
        printf("********2.改*********\n");
        printf("********3.查*********\n");
        printf("********4.返回*******\n");
        printf("*********************\n");
        int choose;
        printf("输入你的选择>>>>>");
        scanf("%d",&choose);
        while(getchar()!=10);
        switch(choose)
        {
        case 0:
            //增
            add_staff(sfd);
            break;
        case 1:
            //删
            del_staff(sfd);
            break;                                                                                                                                              
        case 2:
            //改
            modify_staff(sfd);
            break;
        case 3:
            //查
            search_staff(sfd);
            break;
        case 4:
            //返回
            do_quit(sfd);
            return -1;
        default:
            printf("输入有误,请重新输入\n");
            goto err;
        }
    }
}


//登录
int do_login(int sfd)
{
	struct msg staff_all;
	staff_all.type = 'l';
	system("clear");
	printf("********************\n");	
	printf("欢迎进入登录界面\n");
err:
	printf("请输入你的身份  p:员工 r：管理员>>>");
	scanf("%c",&staff_all.mod);
	while(getchar()!=10);
	if(staff_all.mod !='p' && staff_all.mod !='r')
	{
		printf("输入的权限有误,请重新输入\n");
		goto err;
	}

	printf("请输入工号>>>>>");
	scanf("%s",staff_all.number);
	while(getchar()!=10);

	printf("请输入账户名>>>");
	scanf("%s", staff_all.name);
	while(getchar()!=10);

	printf("请输入密码>>>");
	scanf("%s", staff_all.text);
	while(getchar()!=10);

	strcpy(num,staff_all.number);

	//发送,等待接收应答
	if(send(sfd, &staff_all, sizeof(staff_all), 0)<0)
	{
		ERR_LOG("send");
		return -1;
	}

	memset(&staff_all, 0, sizeof(staff_all));
	int recv_len = recv(sfd, &staff_all, sizeof(staff_all), 0);
	if(recv_len < 0)
	{
		ERR_LOG("recv");
		return -1;
	}
	else if(0 == recv_len)
	{
		printf("服务器关闭\n");
		exit(0);
	}

	if(strcmp(staff_all.text, "notexits") == 0)
	{
		printf("不存在该员工\n");
		flag = 0;
	}
	else if(strcmp(staff_all.text, "exists") == 0)
	{
		printf("该员工已经登录!\n");
	}
	else
	{
		if(strcmp(staff_all.text, "OK") == 0)
		{	
			printf("成功登录!\n");
			right=staff_all.mod;
			flag = 1;

		}

	}
	return 0;
}


//注册 
int do_register(int sfd)
{
	struct msg staff_all;
	staff_all.type = 'z';
	system("clear");
	printf("********************\n");	
	printf("欢迎进入注册界面\n");
err:
	printf("请输入你的身份  p:员工 r：管理员>>>");
	scanf("%c",&staff_all.mod);
	while(getchar()!=10);
	if(staff_all.mod !='p' && staff_all.mod !='r')
	{
		printf("输入的权限有误,请重新输入\n");
		goto err;
	}

	printf("请输入工号>>>>>");
	scanf("%s",staff_all.number);
	while(getchar()!=10);

	printf("请输入账户名>>>");
	scanf("%s", staff_all.name);
	while(getchar()!=10);

	printf("请输入密码>>>");
	scanf("%s", staff_all.text);
	while(getchar()!=10);

	//发送,等待接收应答
	if(send(sfd, &staff_all, sizeof(staff_all), 0)<0)
	{
		ERR_LOG("send");
		return -1;
	}

	memset(&staff_all, 0, sizeof(staff_all));
	int recv_len = recv(sfd, &staff_all, sizeof(staff_all), 0);
	if(recv_len < 0)
	{
		ERR_LOG("recv");
		return -1;
	}
	else if(0 == recv_len)
	{
		printf("服务器关闭\n");
		exit(0);
	}

	if(strcmp(staff_all.text, "**OK**") == 0)
	{
		printf("注册成功!\n");
	}
	else if(strcmp(staff_all.text, "**EXISTS**") == 0)
	{
		printf("注册失败,该账户已经被注册!\n");
	}
	else
	{
		printf("注册失败,未知错误请联系管理员!\n");
	}
	return 0;
}


//退出
int do_quit(int sfd)
{
	struct msg staff_all;
	staff_all.type = 'q';
	strcpy(staff_all.number,num);	
	if(send(sfd,&staff_all,sizeof(staff_all),0)<0)
	{
		ERR_LOG("send");
		return -1;
	}
	return 0;
}


//增加
int add_staff(int sfd)
{
	struct msg staff_all;
	staff_all.type = 'a';
	int res;
	
	printf("进入增加函数\n");
	if(right == 'p')
	{
		printf("员工没有增加权限\n");
		return 0;
	}
	else if(right == 'r')
	{
		staff_all.mod=right;
		printf("请输入需要添加的员工/管理员的工号\n");
		scanf("%s",staff_all.number);
		while(getchar()!=10);

//		printf("请输入需要添加的员工/管理员的账户名\n");
//		scanf("%s",staff_all.name);
//		while(getchar()!=10);

		printf("请输入需要添加的员工/管理员的性别\n");
		scanf("%s",staff_all.sex);
		while(getchar()!=10);

		printf("请输入需要添加的员工/管理员的手机号\n");
		scanf("%s",staff_all.phone);
		while(getchar()!=10);

		printf("请输入需要添加的员工/管理员的地址\n");
		scanf("%s",staff_all.address);
		while(getchar()!=10);

		printf("请输入需要添加的员工/管理员的薪资\n");
		scanf("%s",staff_all.salary);
		while(getchar()!=10);

		printf("请输入需要添加的员工/管理员的年龄\n");
		scanf("%s",staff_all.age);
		while(getchar()!=10);
	}

		if(send(sfd,&staff_all,sizeof(staff_all),0)<0)
		{
			ERR_LOG("send");
			return -1;
		}
		
		res = recv(sfd,&staff_all, sizeof(staff_all), 0); 
		if(res < 0)
		{
			ERR_LOG("recv");
			return -1;
		}
		else if(res ==0)
		{
			printf("服务器关闭\n");
		}
		else
		{
			if(strcmp(staff_all.text,"notexits")==0)
			{
				printf("该员工不存在\n");
			}
				else if(strcmp(staff_all.text,"OK")==0)
			{
				printf("增加成功\n");
			}
		}
		return 0;
}


//删除函数
int del_staff(int sfd)
{
	struct msg staff_all;
	staff_all.type = 'd';
	int res;

	if(right == 'p')
	{
		printf("员工没有删除权限\n");
		return 0;
	}
	else if(right == 'r')
	{
		staff_all.mod=right;
		printf("输入你想删除的员工工号\n");
		scanf("%s",staff_all.number);
		while(getchar()!=10);
	}

		//发送消息
		if(send(sfd,&staff_all,sizeof(staff_all),0)<0)
		{
			ERR_LOG("send");
			return -1;
		}
		//接收消息		
		res = recv(sfd,&staff_all, sizeof(staff_all), 0); 
		if(res < 0)
		{
			ERR_LOG("recv");
			return -1;
		}
		else if(res ==0)
		{
			printf("服务器关闭\n");
		}
		else
		{
			if(strcmp(staff_all.text,"notexits")==0)
			{
				printf("该员工不存在\n");
			}
				else if(strcmp(staff_all.text,"OK")==0)
			{
				printf("删除成功\n");
			}
		}
	return 0;
}


//改
int modify_staff(int sfd)
{
	struct msg staff_all;
	staff_all.type = 'm';
	int res;


	if(right == 'p')
	{

		staff_all.mod=right;
		strcpy(staff_all.number,num);

		printf("请输入你想修改成的本人信息\n");
		printf("请输入你想修改成的本人名字\n");
		scanf("%s",staff_all.name);
		while(getchar()!=10);

		printf("请输入你想修改成的本人性别\n");
		scanf("%s",staff_all.sex);
		while(getchar()!=10);

		printf("请输入你想修改成的本人地址\n");
		scanf("%s",staff_all.address);
		while(getchar()!=10);

		printf("请输入你想修改成的本人电话\n");
		scanf("%s",staff_all.phone);
		while(getchar()!=10);
	}
	else if(right == 'r')
	{
		staff_all.mod=right;
		printf("输入你想修改的员工工号\n");
		scanf("%s",staff_all.number);
		while(getchar()!=10);

		printf("请输入此工号你想修改成的员工名字\n");
		scanf("%s",staff_all.name);
		while(getchar()!=10);

		printf("请输入此工号你想修改成的员工性别\n");
		scanf("%s",staff_all.sex);
		while(getchar()!=10);

		printf("请输入此工号你想修改成的员工住址\n");
		scanf("%s",staff_all.address);
		while(getchar()!=10);

		printf("请输入此工号你想修改成的员工薪资\n");
		scanf("%s",staff_all.salary);
		while(getchar()!=10);

		printf("请输入此工号你想修改成的员工年龄\n");
		scanf("%s",staff_all.age);
		while(getchar()!=10);

		printf("请输入此工号你想修改成的员工电话号码\n");
		scanf("%s",staff_all.phone);
		while(getchar()!=10);
	}

		if(send(sfd,&staff_all,sizeof(staff_all),0)<0)
		{
			ERR_LOG("send");
			return -1;
		}
		
		res = recv(sfd,&staff_all, sizeof(staff_all), 0); 
		if(res < 0)
		{
			ERR_LOG("recv");
			return -1;
		}
		else if(res ==0)
		{
			printf("服务器关闭\n");
		}
		else
		{
			if(strcmp(staff_all.text,"notexits")==0)
			{
				printf("该员工不存在\n");
			}
				else if(strcmp(staff_all.text,"OK")==0)
			{
				printf("修改信息成功\n");
			}
		}

		return 0;
}




//查询
int search_staff(int sfd)
{
	struct msg staff_all;
	staff_all.type = 's';
	int res;

	if(right == 'p')
	{
		printf("员工查询自己的个人信息\n");
		staff_all.mod=right;
		strcpy(staff_all.number,num);

	}
	else if(right == 'r')
	{
		printf("管理员查询员工的个人信息\n");
		staff_all.mod=right;
		printf("输入你想查询的员工工号\n");
		scanf("%s",staff_all.number);
		while(getchar()!=10);
	}
		if(send(sfd,&staff_all,sizeof(staff_all),0)<0)
		{
			ERR_LOG("send");
			return -1;
		}
		
		res = recv(sfd,&staff_all, sizeof(staff_all), 0); 
		if(res < 0)
		{
			ERR_LOG("recv");
			return -1;
		}
		else if(res ==0)
		{
			printf("服务器关闭\n");
		}
		else
		{
			if(strcmp(staff_all.text,"notexits")==0)
			{
				printf("该员工不存在\n");
			}
			else 
			{
				printf("查询信息成功\n");
				printf("%s\n",staff_all.text);
			}
		}

		return 0;
}


