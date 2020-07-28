#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef unsigned int uint32_t;

uint32_t in_aton(const char *str)
{
	unsigned long l;
	unsigned int val;
	int i;

	l = 0;
	for (i = 0; i < 4; i++)	{
		l <<= 8;
		if (*str != '\0') {
			val = 0;
			while (*str != '\0' && *str != '.' && *str != '\n') {
				val *= 10;
				val += *str - '0';
				str++;
			}
			l |= val;
			if (*str != '\0')
				str++;
		}
	}
	return htonl(l);
}



int main(void)
{
	FILE *f;
	char buffer[1024],ip[16],num[16];

	f = fopen("./chinaip.txt","r+");
	if(f == NULL)
		return -1;
	while(fgets(buffer,sizeof(buffer),f) != NULL){
		sscanf((const char *)buffer,"%*[^|]|%*[^|]|%*[^|]|%[^|]|%[^|]\n",ip,num);
		printf("ip:%-16s num:%s  %u\n",ip,num,in_aton(ip));
	}
	fclose(f);

	return 0;
}
