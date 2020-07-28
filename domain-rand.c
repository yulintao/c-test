#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MTD_RND_MAX 32
#define MTD_RND_LOW 6

static int
mtd_build_random_str(char *domain)
{
    int i, dlen = lrand48() % (MTD_RND_MAX - MTD_RND_LOW + 1) + MTD_RND_LOW;
    static const char str[] = "0123456789abcdefghijklmnopqrstuvwxyz";


    for(i = 0; i < dlen; i++){
        domain[i] = str[lrand48()%(strlen(str))];
        /*printf("[lo:%d  domain:%hhx] ",i, domain[i]);*/
    }
    /*printf("\n");*/

    return dlen;
}

#define LINE_LEN 128
/* dump a mbuf on console */
void
rte_hexdump(FILE *f, const char *title, const void *buf, unsigned int len)
{
	unsigned int i, out, ofs;
	const unsigned char *data = buf;
	char line[LINE_LEN];	/* space needed 8+16*3+3+16 == 75 */

	fprintf(f, "%s at [%p], len=%u\n",
		title ? : "  Dump data", data, len);
	ofs = 0;
	while (ofs < len) {
		/* format the line in the buffer */
		out = snprintf(line, LINE_LEN, "%08X:", ofs);
		for (i = 0; i < 16; i++) {
			if (ofs + i < len)
				snprintf(line + out, LINE_LEN - out,
					 " %02X", (data[ofs + i] & 0xff));
			else
				strcpy(line + out, "   ");
			out += 3;
		}


		for (; i <= 16; i++)
			out += snprintf(line + out, LINE_LEN - out, " | ");

		for (i = 0; ofs < len && i < 16; i++, ofs++) {
			unsigned char c = data[ofs];

			if (c < ' ' || c > '~')
				c = '.';
			out += snprintf(line + out, LINE_LEN - out, "%c", c);
		}
		fprintf(f, "%s\n", line);
	}
	fflush(f);
}
static void
mtd_build_dns_map(void)
{
    //uint64_t hash;
    uint8_t /*tail_bt,*/ ns_name[64];
    char tmp_name[64] = {'\0'};
    int len, i;

#if 0
    hash = XXH64((void *)(&dhcp_msg->rip_mac), MTD_ETH_ALEN, dhcp_msg->rip);

    i = 0;
    while(hash){
        tail_bt = hash & 0xf;
        if(tail_bt < 10)
            tail_bt += '0';
        else
            tail_bt += 'a' - 0xa;
        tmp_name[i++] = tail_bt;
        hash >>= 4;
    }
#endif
    i = mtd_build_random_str(tmp_name);
    printf("create :%d name:%s \n",i, tmp_name);
    len = 0;
    while(i >= 0){
        ns_name[len++] = tmp_name[i--];
    }

    ns_name[0] = --len;
    ns_name[++len] = 3;
    len += 1;
    memcpy(ns_name + len, "com", 3);
    len += 3;
    ns_name[len++] = 0;

    /*rte_hexdump(stdout, "domain", ns_name, len);*/
    //dhcp_msg->dns_option.domain_len = len;
    //dhcp_msg->dns_option.label_count = 2;
    /*memcpy(dhcp_msg->dns_option.val, ns_name, len);*/
}


int
main(void)
{
    while(1){
        mtd_build_dns_map();
        usleep(500000);
    }
}
