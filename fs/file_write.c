#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int main(int argc,char *argv[])
{
	if(argc<2)
		return 0;

	//用读写追加方式打开一个已经存在的文件
	int fd = open(argv[1], O_RDWR | O_APPEND | O_CREAT | O_SYNC);

	if(fd==-1) {
		printf("error is %s\n", strerror(errno));
	} else {
		//打印文件描述符号
		printf("success fd = %d\n",fd);
		char buf[100];
		memset(buf,0,sizeof(buf));
		strcpy(buf,"hello world\n");
		write(fd,buf,strlen(buf));
		close(fd);
	}
	
	return 0;
}
