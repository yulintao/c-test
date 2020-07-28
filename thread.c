
#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <sys/types.h>
#include <sys/syscall.h> /* must include this file */  
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/prctl.h>

__thread char *thread_var = NULL;
pthread_t a,b;


pid_t gettid(void)  
{  
    return syscall(SYS_gettid);  
}  

void *thread_loop(void *arg)
{
	while(1)
	{
		printf("%s	cpu is %d thread %lu id:%d\n",__FUNCTION__,sched_getcpu(),pthread_self(),gettid());
		sleep(1);
	}
}
void *thre(void *arg)
{
	static int i =0;
//	int fd = *(int *)(arg);
//	pthread_create(&pid,NULL,thread_loop,NULL);
	thread_var = (char *)malloc(strlen("123456789"));
	memset(thread_var,0,9);
printf("var:%s  addr:%p  arg:%u\n",thread_var,thread_var,*(unsigned *)arg);
	pthread_setname_np(a, "th111");

	while(1)
	{
	//	write(fd,"123456\n",strlen("123456\n"));
		printf(" %s	cpu is %d thread %lu id:%d  thread_var:%p %s\n",__FUNCTION__,sched_getcpu(),pthread_self(),gettid(),thread_var,thread_var);
		memcpy(thread_var,"12345678",strlen("12345678"));
		printf(" %s	cpu is %d thread %lu id:%d  thread_var:%p %s\n",__FUNCTION__,sched_getcpu(),pthread_self(),gettid(),thread_var,thread_var);
		sleep(1);
	}

}

void *thre1(void *arg)
{
	static int i =0;
	pthread_setname_np(b, "th222");

	while(1)
	{
		sleep(5);
		printf("%s	cpu is %d thread %lu id:%d  thread_var:%p %s\n",__FUNCTION__,sched_getcpu(),pthread_self(),gettid(),thread_var,thread_var);
		memcpy(thread_var,"qwertyui",strlen("12345678"));
		printf("%s	cpu is %d thread %lu id:%d  thread_var:%p %s\n",__FUNCTION__,sched_getcpu(),pthread_self(),gettid(),thread_var,thread_var);
	}

}


int main(void)
{
	int i = 4;
	int fd;
	thread_var = (char *)malloc(strlen("123456789"));
	memset(thread_var,0,9);
	printf("var:%s  addr:%p\n",thread_var,thread_var);
#if 0
	fd = open("a.txt",O_CREAT|O_WRONLY|O_EXCL,0444);
	flock(fd,LOCK_EX);
#endif
	pthread_create(&a,NULL,thre,&i);
	pthread_create(&b,NULL,thre1,NULL);
		//pthread_create(a+1,NULL,thre1,NULL);
	pthread_join(a,NULL);
	pthread_join(b,NULL);
	//sleep(10);
//	close(fd);
	printf("xxxxxxx\n");
	//while(1)
	//sleep(1);
	return 0;
}


