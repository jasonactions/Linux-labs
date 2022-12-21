/*
	端口号
	IP地址
	
1，创建套接字--同时指定协议和类型
		socket()
2，绑定 --将保存在相应地址结构中的地址信息，与套接字进行绑定
		2.1 填充struct sockaddr_in结构体变量
			（1）struct sockaddr_in ,
			（2）struct in_addr
			（3）htons()
			（4）inet pton()s
3，客户端连接
		connect()
4，读写数据
		write()
5，关闭套接字
*/

/*./clinet serv_ip serv_port*/

#include"net.h"

#define BUFLN 128

void usage(char *s)
{
	printf("\n%s serv_ip serv_port\n",s);
	printf("\n\t serv_ip : server ip address");
	printf("\n\t serv_port: server port (>5000)\n\n");
}

int main(int argc,  char **argv)
{
	int fd = -1;

	int port = -1;
	
	struct sockaddr_in sin;
	
	if(argc != 3)
	{
		usage(argv[0]);
		exit(1);
	}

	/*1.创建socket fd*/
	if((fd = socket(AF_INET,SOCK_STREAM,0)) < 0 )
	{
		perror("socket");
		exit(1);
	}
	
	port = atoi(argv[2]);
	if(port <5000)
	{
		usage(argv[0]);
		exit(1);
	}
	
	/*2,连接服务器*/

	/*2.1 填充struct sockaddr_in结构体变量*/
	memset(&sin, 0 ,sizeof(sin));

	
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port); //网络字节序的端口号
	sin.sin_addr.s_addr = inet_addr(argv[1]);

	if(connect(fd,(struct sockaddr*)&sin,sizeof(sin)) < 0 )
	{
		perror("connect");
		exit(1);
	}

	/*3，读写数据*/
	
	char  buf[BUFLN];

	int ret = -1;
	while(1)
	{
		printf("Please enter the:");
		memset(buf, 0 ,BUFLN);
		if(fgets(buf,BUFLN-1,stdin) == NULL)
		{
			continue;
		}
		
		do {
			ret = write (fd, buf, strlen (buf));
		} while (ret < 0 && EINTR == errno);

		if(!strncasecmp(buf,QUIT_STR,strlen(QUIT_STR)))
		{
			printf("Client is exiting\n");
			break;
		}
		
	}
	/*4，关闭套接字*/
	close(fd);
	return 0;
}



