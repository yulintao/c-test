#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#include "thpool.h"
#include "libcli.h"

void *handle_cli_process (void *arg);

int main(void)
{
	int rst;
	pthread_t ptid = 0;

	rst = pthread_create(&ptid, NULL, handle_cli_process, NULL);
	if (rst) {
		fprintf(stderr, "start handle_cli_process pthread error");
		return -1;
	}
	rst = pthread_detach(ptid);
	if (rst) {
		fprintf(stderr, "detach handle_cli_process pthread error");
		return -1;
	}

	while(1)
		sleep(10);
}

