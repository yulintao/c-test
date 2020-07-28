#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dump.h"

int main(int argc, char *argv[])
{
    char *p = NULL, *buf = NULL;
    FILE *fp = NULL;
    size_t size, fsize = 14376;

    printf("file_name:%s %d\n",argv[1],argc);
    fp = fopen(argv[1], "r+");
    buf = (char *)malloc(fsize);
    size = fread(buf,1,fsize,fp);
    //buf[size] = '\0';
    dump(stdout, "readfile", buf, fsize + 1);
    fclose(fp);
    printf("file_length:%lu real:%lu\n",fsize,size);
    free(buf);
}

