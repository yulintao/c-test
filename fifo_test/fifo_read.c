/* fifo_read.c */  

#include <sys/types.h>  
#include <sys/stat.h>  
#include <errno.h>  
#include <fcntl.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <limits.h>         

#define MYFIFO          "myfifo" //共享的文件名  
#define MAX_BUFFER_SIZE     PIPE_BUF /*在limits.h中定义*/  

int main()  
{  
	char buff[MAX_BUFFER_SIZE];  
	int  fd;  
	int  nread;  

	/* 不存在则创建fifo*/  
	if (access(MYFIFO, F_OK) == -1)   
	{  
		if ((mkfifo(MYFIFO, 0666) < 0) && (errno != EEXIST))  
		{  
			printf("Cannot create fifo file\n");  
			exit(1);  
		}  
	}  

	/* 打开fifo并一直读*/  
	fd = open(MYFIFO, O_RDONLY|O_NONBLOCK);  
	if (fd == -1)  
	{  
		printf("Open fifo file error\n");  
		exit(1);  
	}  

	while (1)  
	{  
		memset(buff, 0, sizeof(buff));  
		if ((nread = read(fd, buff, MAX_BUFFER_SIZE)) > 0)  
		{  
			printf("Read '%s' from FIFO,num:%d\n", buff,nread);  
		}         
	}     

	close(fd);    
	exit(0);  
}     
