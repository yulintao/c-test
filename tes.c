#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>


static uint32_t vip_alloced = 111;
static uint32_t vip_start = 0x12345678;
static uint32_t vip_end = 0x1234ffff;

static uint32_t
mtd_alloc_vip(void)
{
    uint32_t vip = 0;
    uint8_t end;

    while(1){
    if(vip_alloced >= vip_start && vip_alloced < vip_end){

        vip_alloced += lrand48() % 10000;                        
        vip = vip_alloced;
        end = (vip & 0xff) + 1;
        if(end <= 1)
            vip_alloced++ ;
        else
            break;
    }
    else
        vip_alloced = vip_start;
    }

    return htonl(vip);
}


int main(void)
{
    uint32_t ip_l;
    char str[128];
    unsigned char buff[INET6_ADDRSTRLEN];

    while(1){
        ip_l = mtd_alloc_vip();
        memcpy(buff, &ip_l, 4);
        inet_ntop(AF_INET,buff ,str,INET6_ADDRSTRLEN );
        printf("IP:%s\n",str);
#if 0
        printf("%hhx, %hhx, %hhx, %hhx\n",
               (ip_l & 0xff000000) >> 24,
               (ip_l & 0xff0000) >> 16,
               (ip_l & 0xff00) >> 8,
               (ip_l & 0xff));
#endif
        usleep(500000);
    }
return 0;
}
