#include <stdio.h>

int main(void)
{
	char *p = "1.1.1.232:123455";
	char buf1[128]="12345";
	int buf2;
	unsigned char id;
	const char *ptr = buf1;
	char *core_dump = (void *)0x1;

	printf("%s\n",ptr);
	printf("%c %c\n",*ptr,*core_dump);
	
	sscanf(p,"%[^:]:%d\n",buf1,&buf2);
	sscanf(buf1,"%*[^.].%*[^.].%*[^.].%hhu\n",&id);
	printf("%s  %d  %d\n",buf1,buf2,id);

}
