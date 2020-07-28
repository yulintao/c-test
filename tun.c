#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/if_tun.h>
#include <sys/types.h>
#include <net/if.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int tun_alloc(int flags)
{
 
    struct ifreq ifr;
    int fd, err;
    FILE *f;
    unsigned major, minor;
    char *clonedev = "/dev/net/tun0";
    dev_t dev;

    f = fopen("/dev/net", "r");
    err = fscanf(f, "%u:%u", &major, &minor);
    printf("err:%d ma:%u mi:%u\n",err,major,minor);
    fclose(f);
    dev = makedev(major, minor); 
printf("%d\n",mknod(clonedev, S_IFCHR | S_IRUSR | S_IWUSR, dev));
    if ((fd = open(clonedev, O_RDWR)) < 0) {
perror("Error");
printf("Error%d\n",fd);
        return fd;
    }
 
    memset(&ifr, 0, sizeof(ifr));
    ifr.ifr_flags = flags;
 
    if ((err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
        close(fd);
        return err;
    }
 
    printf("Open tun/tap device: %s for reading...\n", ifr.ifr_name);
 
    return fd;
}

int main(void)
{
  int tun_fd, nread;
  char buffer[1500];

  tun_fd = tun_alloc(IFF_TAP | IFF_NO_PI);
  while (1) {
    nread = read(tun_fd, buffer, sizeof(buffer));
    if (nread < 0) {
       perror("Reading from interface");
       close(tun_fd);
       exit(1);
    }
 
    printf("Read %d bytes from tun/tap device\n", nread);
  }
  return 0;
}


