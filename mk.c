#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <pthread.h>
#include <sys/queue.h>

#define _10M (1024*1024*100UL)


int main(void)
{
    char *add = NULL;
    size_t sz = _10M *10;

	add = malloc( sz );
    while(1){
printf("%p  %p\n",(add), (add+1));
	sleep(1);
    }
	free(add);
    printf("xxxxx\n");
   
    //exit(1);
	
}
