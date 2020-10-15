
#include <stdio.h>
int divided(int n, int m) //注意该递归函数的定义
{
	if (n / 5 == 0 || n % 5 != 1)
		return 0;
	if (m == 1)
		return 1;
	return divided(n - n / 5 - 1, m - 1);
}
int main(int argc, char *argv[])
{
	int n;
	int m = 3;
	for (n = 1;; n++)
		if (divided(n, m)) {
			printf("%d\n", n);
			break;
		}
	printf("dafffad \v  %d\n",n);
}
