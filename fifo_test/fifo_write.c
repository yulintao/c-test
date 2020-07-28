/* fifo_write.c */  

#include <sys/types.h>  
#include <sys/stat.h>  
#include <errno.h>  
#include <fcntl.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <limits.h>         

#define MYFIFO          "myfifo"   /* 共享的fifo文件*/  
#define MAX_BUFFER_SIZE     PIPE_BUF    /*在limits.h中定义*/  

int main(int argc, char * argv[]) /*参数为用户即将输入的字符*/  
{  
	int fd;  
	char buff[MAX_BUFFER_SIZE];  
	int nwrite;  

	if(argc <= 1)    //用户没有输入参数  
	{  
		printf("Usage: ./fifo_write string\n");  
		exit(1);  
	}  
	sscanf(argv[1], "%s", buff);  

	/* 打开fifo管道文件*/  
	fd = open(MYFIFO, O_WRONLY);  
	if (fd == -1)  
	{  
		printf("Open fifo file error\n");  
		exit(1);  
	}  
    while(1){
		/*将用户输入的参数写入fifo*/  
		if ((nwrite = write(fd, buff, MAX_BUFFER_SIZE)) > 0)  
		{  
			printf("Write '%s' to FIFO %d\n", buff,nwrite);  
		}
		sleep(1);
	}

	close(fd);  
	exit(0);  
}  
