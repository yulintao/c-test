#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define PORT	  4100
#define SERVER_IP "0.0.0.0"
//#define SERVER_IP "192.168.1.5"
main()
{
	int s;
	struct sockaddr_in addr;
	char buffer[256] = "";
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}
	/* 填写sockaddr_in结构*/
	bzero(&addr, sizeof(addr));
	addr.sin_family	     = AF_INET;
	addr.sin_port	     = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(SERVER_IP);

	if (0 != bind(s, &addr, sizeof(addr))) {
		perror("bind failed !");
		exit(-1);
	}
	if (0 != listen(s, 0)) {
		perror("listen failed !");
		exit(-1);
	}
	/* 接收由server端传来的信息*/
	int conn_fd;
	printf("%s\n", buffer);
	// 这个位置决定了可以接受多少个链接
	conn_fd = accept(s, (struct sockaddr *)NULL, NULL);
	printf("conn_fd %d\n",conn_fd);
	while (1) {
	//每次有新的链接进入conn_fd都会变化
	/*conn_fd = accept(s, (struct sockaddr *)NULL, NULL);*/
	/*printf("conn_fd %d\n",conn_fd);*/
		//bzero(buffer,sizeof(buffer));
		if (0 < read(conn_fd, buffer, sizeof(buffer)))
			/* 将字符串传给server端*/
			printf("%s\n", buffer);
		sleep(1);
	}
}
