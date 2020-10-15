#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/queue.h>

#define _10M (1024 * 1024 * 100UL)

#define abcdef     \
	_(ab, "1") \
	_(cd, "2") \
	_(ef, "3")

enum {
#define _(name, str) sys_##name,
	abcdef
#undef _
};

void set_mem_val(char **addr, int val)
{
	printf("addr:%p  *addr:%p  val:%d\n", addr, *addr, val);
	*(int *)*addr = val;
}

int main(void)
{
	char *add = NULL;
	size_t sz = _10M * 10;

	add = malloc(sz);

	set_mem_val(&add, 10);

	printf("set val over:%d\n", *(add));

	while (1) {
		printf("add addr:%p add addr &:%p %p\n", (add), &add,
		       (add + 1));
		sleep(1);
	}
	free(add);
	printf("xxxxx\n");

	//exit(1);
}
