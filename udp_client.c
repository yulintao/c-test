#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define PORT	4100 
#define SERVER_IP "192.168.1.5"
int main()
{
	int s,len;
	struct sockaddr_in addr;
	int addr_len =sizeof(struct sockaddr_in);
	char buffer[256];
	/* ½¨bsocket*/
	if((s = socket(AF_INET,SOCK_DGRAM,0))<0){
		perror("socket");
		exit(1);
	}
	/* Ìдsockaddr_in*/
	bzero(&addr,sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	while(1){
		bzero(buffer,sizeof(buffer));
		printf("[type in character]:");
		fflush(stdout);
		len =read(STDIN_FILENO,buffer,sizeof(buffer));
		sendto(s,buffer,len,0,(struct sockaddr *)&addr,addr_len);
		printf("receive: %s",buffer);
	}
	return 0;
}
