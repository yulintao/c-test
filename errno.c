#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

extern int errno;

int main(int argc,char *argv[])
{
 char *msg = NULL;

 errno = atoi(argv[1]);
 //assert(errno == 1);
 msg = strerror(errno);

 printf("error:%s\t:%d\n",msg,errno);
 //printf("%s\n",getenv("HOME"));
}
