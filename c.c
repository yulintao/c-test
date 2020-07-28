//#include <stdio.h>
//
//
// int fun(int a)
//{
//	if(a==0){
//		printf("%d :%d\n",a,__LINE__);
//		return 0;
//	}
//	else{
//		printf("%d :%d\n",a,__LINE__);
//		return (a/5-1);
//	}
// return	fun(a);
//}
//
//
// int main(void)
//{
//	int i= 1;
//	while(i++){
//	if(0==fun(i))
//		printf("%d\n",i);
//	}
//}
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define ABCD 1234
#define ab   1234
#define acd  1234
#define ad   1234

#define A         \
	int aaaa; \
	int b;    \
	int dddddddddd;

//注意该递归函数的定义
static int divided(int n, int m)
{
	if (n / 5 == 0 || n % 5 != 1)
		return 0;
	if (m == 1)
		return 1;
	return divided(n - n / 5 - 1, m - 1);
}

void str_to_dom(char *name)
{
	int i, token = 0, len;
	char *ptr, *lable;

	len = strlen(name);
	ptr = len + name - 1;
	for (i = 0; i < len; i++) {
		// more .
		ptr--;
		if (*ptr == '.' && (++token > 1)) {
			lable = ptr + 1; // find lable
			printf("__LINE__:%d dom:%s\n", __LINE__, lable);
			continue;
		}
		if (i == len - 1) {
			lable = name;
			printf("__LINE__:%d dom:%s\n", __LINE__, lable);
			return;
		}
	}
}

int main(int argc, char *argv[])
{
	int n;
	int m = 5;
	for (n = 1;; n++)
		if (divided(n, m)) {
			printf("%d\n", n);
			break;
		}
	char *ca = "123456789012345";
	char *bf = "111111111111111";
	int ln	 = strlen(ca);
	bf	 = (char *)malloc(ln);
	memcpy(bf, ca, 14);
	printf(":%s :%d\n", bf, ln);
	// clang-format off
// str_to_dom("baidu.com");
// str_to_dom("bbbbbbba.com");
str_to_dom("ccc.a.b.com");
str_to_dom("a.b.cn");
	// clang-format on

	assert(ca);
	assert(0);
	char *abcc = NULL;
	assert(abcc);

	char *a = "bbb";

	for (a = 0; a; a = ca)
		printf("xxx%s\n", a);
	printf("Nil:%d\n", a == NULL ? 1 : 0);
	while (1) {
		printf(afadf);
	}
}
