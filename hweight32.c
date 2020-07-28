#include <stdio.h>
#include <stdlib.h>

static unsigned int hweight32(unsigned int w)
{
	unsigned int res = w - ((w >> 1) & 0x55555555);
	res = (res & 0x33333333) + ((res >> 2) & 0x33333333);
	res = (res + (res >> 4)) & 0x0F0F0F0F;
	res = res + (res >> 8);
	return (res + (res >> 16)) & 0x000000FF;
}



int main()
{
	printf("%u\n",hweight32(0xfff));
	printf("%u\n",hweight32(0xff2));
	printf("%u\n",hweight32(0x10ff));
	printf("%u\n",hweight32(0xffffffff));
}
