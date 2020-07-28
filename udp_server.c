#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define PORT 1111 

unsigned long long int c1;

void cnt_thread(void *argv)
{
   static unsigned long long int cnt_all, cnt_old;
   unsigned long long int cnt_curr;

   cnt_curr = c1 - cnt_old;
}

int main(){
	int sockfd,len;
	struct sockaddr_in addr;
	int addr_len = sizeof(struct sockaddr_in);
	char buffer[2560];

    c1 = 0;
	if((sockfd=socket(AF_INET,SOCK_DGRAM,0))<0){
		perror ("socket");
		exit(1);
	}
	bzero ( &addr, sizeof(addr) );
	addr.sin_family=AF_INET;
	addr.sin_port=htons(PORT);
	addr.sin_addr.s_addr=htonl(INADDR_ANY) ;
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr))<0){
		perror("connect");
		exit(1);
	}
	while(1){
		/*bzero(buffer,sizeof(buffer));*/
		len = recvfrom(sockfd,buffer,sizeof(buffer), 0 , (struct sockaddr *)&addr ,&addr_len);
        if(len != 0){
            c1++;
        }
		/*printf("receive from %s\n" , inet_ntoa( addr.sin_addr));*/
		/*sleep(1);*/
		//sendto(sockfd,buffer,len,0,(struct sockaddr *)&addr,addr_len);
	}
	return 0;
}
