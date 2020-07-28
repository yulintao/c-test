#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	char *p, *q;

	p = argv[1];
	q = p + strlen(p);
	*q = ',';
	q = strchr(argv[1], ',');
	do {
		*q = '\0';
		printf("%s\n", p);
		q++;
		if (!q)
			break;
		p = q;
		q = strchr(p, ',');
	} while (q);
}
