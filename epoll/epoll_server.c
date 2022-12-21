/*
	端口号
	IP地址
	几路客户端  （一般为5）
1，创建套接字--同时指定协议和类型
		socket()
2，绑定 --将保存在相应地址结构中的地址信息，与套接字进行绑定
		2.1 填充struct sockaddr_in结构体变量
			（1）struct sockaddr_in ,
			（2）struct in_addr
			（3）htons()
			（4）inet_pton()
		2.2 绑定
			 bind() 
3，将主动套接字转换为被动套接字
		listen()
4，阻塞的等待客户端连接
		accept()
		获取刚建立客户端的ip地址和端口号
			inet_ntop()
			ntohs()
		
5，读写操作
*/
#include"net.h"

#define epoll_num 20
#define BUFLEN 128
int socket_server(char *addr,char *port)
{
	int sockfd;
	struct sockaddr_in sin;
	//创建套接字--同时指定协议和类型
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		perror("socket\n");
		return -1;
	}

	int b = -1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &b,sizeof(int));

	//绑定 --将保存在相应地址结构中的地址信息，与套接字进行绑定
	memset(&sin, 0 ,sizeof(sin));

	sin.sin_family = AF_INET;    //ipv4协议
	sin.sin_addr.s_addr = inet_addr(addr);   //IP地址
	sin.sin_port  = htons(atoi(port));       //端口号

	if(bind(sockfd, (struct sockaddr *)&sin,sizeof(sin)) < 0)
	{
		perror("bind\n");
		return -1;
	}

	//将主动套接字转换为被动套接字
	if(listen(sockfd, 5) < 0)
	{
		perror("listen\n");
		return -1;
	}
	return sockfd;
}


int main(int argc, char * argv [])
{
	char *addr , *port;
	int epoll_sockfd;
	int client_fd;
	int epoll_fd;
	int ret = 0;
	int i = 0; 
	int rdlen = 0;
	char buf[BUFLEN]= {0};
	
	struct sockaddr_in client_sin;
	struct epoll_event event ;
	struct epoll_event events[epoll_num];
	
	if(argc !=3)
	{
		printf("Usage:%s serverip port.\n",argv[0]);
		return -1;
	}

	addr = argv[1];
	port = argv[2];

	//获取连接的文件描述符
	epoll_sockfd = socket_server(addr, port);
	if(epoll_sockfd < 0)
	{
		printf("socket_server error\n");
		return -1;
	}

	
	//epoll连接
	memset(events, 0, epoll_num*sizeof(struct epoll_event)); //清空结构体
	event.events = EPOLLIN;
	event.data.fd = epoll_sockfd;
	
	epoll_fd = epoll_create(1);

	epoll_ctl(epoll_fd,EPOLL_CTL_ADD,epoll_sockfd,&event);

	while(1)
	{
		ret = epoll_wait(epoll_fd,events,20,-1);  //阻塞等待时间的到来      ，返回值为响应的fd的个数
		printf("epoll_wait return = %d\n",ret);

		for(i=0;i < ret ;i++)
		{
			printf("epoll_wait return = %d\n",events[i].data.fd);
			if(events[i].data.fd == epoll_sockfd)
			{
				client_fd = accept(events[i].data.fd,NULL,NULL);
				if(client_fd < 0)
				{
					printf("accept error\n");
					exit(1);
				}

				struct epoll_event event;
				event.events = EPOLLIN;
				event.data.fd = client_fd;
				epoll_ctl(epoll_fd,EPOLL_CTL_ADD,client_fd,&event);
			}
			else 
			{	
				memset(buf, 0 ,BUFLEN);
				rdlen = read(events[i].data.fd, buf,BUFLEN);
				if(rdlen > 0)
				{
					printf("read buf=%s\n",buf);
				}
				else if(rdlen == 0)
				{
					event.events = EPOLLIN;
					event.data.fd = events[i].data.fd;
					epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &event);
				}
			}
		}
	}
	
}
