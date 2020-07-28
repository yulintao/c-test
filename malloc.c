#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/queue.h>

#define _10M (1024 * 1024 * 100)
#if 0
int pool_insert(struct pool_list *add_list, void *argv)
{
}

void thr1(void *argv)
{
    char *add = NULL;
    int v = *(int *)argv;
    int ret = 0;

    if(v == 0){
	add = malloc(_1M);
	ret = pool_insert(add_list, add);
	if(ret == -1)
	    syslog(LOG_INFO,"pool full\n");
    } else {
	pool_delete(add_list);
    } 
}
#endif

struct abc {
	int a;
	void *b;
};

#define __rte_aligned(a) __attribute__((__aligned__(a)))

typedef struct aaa {
	struct abc *b;
	struct abc *b1;
	struct abc *b2;
} __attribute__((__aligned__(64))) aaa_t;

typedef struct pf_stats_info_s {
	uint64_t app_add_task_success;
	uint64_t app_add_task_failed;
	uint64_t master_dequeue_task;
	uint64_t send_task_aiid_invalid;
	uint64_t send_task_queue_full;
	uint64_t send_task_success;
	uint64_t master_get_result;
	uint64_t master_enqueue_result_success;
	uint64_t master_enqueue_result_failed;
	uint64_t app_get_result_success;
	uint64_t app_get_result_failed;
} __attribute__((__aligned__(64))) pf_stats_info_t;

#if 0
__attribute__((section(".fini"), used)) static void fun_fini(void)
{
    printf("fini destructor1!\n");
}
#endif

__attribute__((destructor)) static void fun_fini2(void)
{
	printf("fini destructor2!\n");
}

__attribute__((constructor)) static void fun_init(void)
{
	printf("init constructor!\n");
}

int main(void)
{
	char *del = NULL;
	struct aaa *add = NULL;

	unsigned long int madd = 0;
	unsigned long int mdel = 0;

	fprintf(stdout, "\033[0;37;40m");
	//fprintf(stdout, "\033[0;47;36m");
	printf("xxxxxxxxxxxxxxxxxxxxxxx %lu  sz:%lu %lu\n",
	       sizeof(struct aaa) / sizeof(struct abc *), sizeof(aaa_t),
	       sizeof(struct aaa));
	printf("sizeof:%lu  %lu\n", sizeof(struct pf_stats_info_s),
	       sizeof(pf_stats_info_t));
	fprintf(stdout, "\033[0m");

	add = malloc(_10M);
	while (1) {
		printf("%p  %p\n", (add), (add + 1));
		sleep(1);
	}
	free(add);
	printf("xxxxx\n");

	//exit(1);
}
