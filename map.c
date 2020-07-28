
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define RSV_MEM 0x200000000
#define RSV_MEM_1 0x280000000
#define RSV_MEM_2 0xE00000000
#if 0

int main(void)
{
	char *p =NULL;
	int fd;
	size_t mp_len;

	mp_len = 0x200000000;
//	mp_len = 0x20000;
	fd = open("/dev/mem",O_RDWR|O_CREAT);
	p = (char *)mmap(NULL,mp_len,PROT_READ|PROT_WRITE,MAP_SHARED,fd,RSV_MEM_2);
	if(p==NULL)
		printf("wrong addr\n");
	else{
		memset(p,0,mp_len);
		printf("p addr is %p\n",p);
		munmap(p,mp_len);
	}
	close(fd);
	exit(EXIT_SUCCESS);
}
#else
       #include <sys/mman.h>
       #include <sys/stat.h>
       #include <fcntl.h>
       #include <stdio.h>
       #include <stdlib.h>
       #include <unistd.h>

       #define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

       int
       main(int argc, char *argv[])
       {
           char *addr;
           int fd;
           struct stat sb;
           off_t offset, pa_offset;
           size_t length;
           ssize_t s;

           printf("pagesize:%d\n",getpagesize());
           if (argc < 3 || argc > 4) {
               fprintf(stderr, "%s file offset [length]\n", argv[0]);
               exit(EXIT_FAILURE);
           }

           fd = open(argv[1], O_RDONLY);
           if (fd == -1)
               handle_error("open");

           if (fstat(fd, &sb) == -1)           /* To obtain file size */
               handle_error("fstat");

           offset = atoi(argv[2]);
           pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
               /* offset for mmap() must be page aligned */

           if (offset >= sb.st_size) {
               fprintf(stderr, "offset is past end of file\n");
               exit(EXIT_FAILURE);
           }

           if (argc == 4) {
               length = atoi(argv[3]);
               if (offset + length > sb.st_size)
                   length = sb.st_size - offset;
                       /* Can't display bytes past end of file */

           } else {    /* No length arg ==> display to end of file */
               length = sb.st_size - offset;
           }

           addr = mmap(NULL, length + offset - pa_offset, PROT_READ,
                       MAP_PRIVATE, fd, pa_offset);
           if (addr == MAP_FAILED)
               handle_error("mmap");

           s = write(STDOUT_FILENO, addr + offset - pa_offset, length);
           if (s != length) {
               if (s == -1)
                   handle_error("write");

               fprintf(stderr, "partial write");
               exit(EXIT_FAILURE);
           }

           exit(EXIT_SUCCESS);
       }
#endif
