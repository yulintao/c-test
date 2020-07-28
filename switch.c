#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int a;

	a = atoi(argv[1]);
	switch (a) {
	default:
		printf("default\n");
	case 1:
		printf("1\n");
		break;
	case 2:
		printf("2\n");
		break;
	}

	return 0;
}
