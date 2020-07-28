#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dump.h"

int main(int argc, char *argv[])
{
    int fd;
    size_t size;
    struct stat sb;
    char *buf;

    fd = open(argv[1], O_RDONLY);
    stat(argv[1], &sb);
    buf = (char *)malloc(sb.st_size);
    size = read(fd, buf, sb.st_size);
    dump(stdout, "readfile", buf, size+1);
    close(fd);
    printf("buf:%p  long:%lu, size:%lu\n",buf, sb.st_size, size);
    free(buf);



}
