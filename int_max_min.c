#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define LENGTH(array) sizeof(array) / sizeof(*array)

#define FOREACH(item, array)                                                   \
	int i, l, breaked;                                                     \
	for (i = 0, l = LENGTH(array), breaked = 0; i < l && !breaked;         \
	     ++i, breaked = !breaked)                                          \
		for (item = &array[i]; !breaked; breaked = !breaked)

enum { CONFIGURING_GLOBALS = 0x01,
       CONFIGURING_LOOP = 0x02,
       CONFIGURING_STACK = 0x04,
       CONFIGURING_IF = 0x08,
       CONFIGURING_APP = 0x10,
} config_state;

#define SB "SB"
#define INT_MAX ((int)(~0U >> 1))
#define INT_MIN (-INT_MAX - 1)
int main(void)
{
	int a[10];
	char ss[16];
	int j, *re;
	int xd = 88;

	sprintf(ss, SB "%s", "adb");
	printf("\n%s\n", ss);
	printf("int max:%d min:%d\n", INT_MAX, INT_MIN);
	printf("int max:%x min:%x\n", INT_MAX, INT_MIN);

	for (j = 0; j < 3; j++)
		a[j] = j;
	FOREACH(re, a)
	printf("%d\n", *re);
	config_state = CONFIGURING_GLOBALS;
	printf("bitMap:%x %d %d \n", config_state, config_state,
	       __builtin_clz(0xf));

	printf("%d\n", 11 % 2);

	long int *abc = malloc(1000);
	printf("abc:%p %p %lu %d\n", (abc), abc + 1, sizeof(abc), sizeof(*abc));
}
