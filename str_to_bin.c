#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>


#define LINE_LEN 128

void
rte_hexdump(FILE *f, const char *title, const void *buf, unsigned int len)
{
    unsigned int i, out, ofs;
    const unsigned char *data = buf;
    char line[LINE_LEN];    /* space needed 8+16*3+3+16 == 75 */

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

static int
mtd_build_ptr_map(uint8_t *ip)
{
    uint8_t dot_ip[64] = {0}, len = 0;
    uint8_t tmp_ip[4];
    int i,j = 0,s = 0,pos = 0;

    for(i = 0; i < 4; i++){

        if(ip[i] == 0){
            dot_ip[j++] = 1;
            dot_ip[j++] = '0';
       //rte_hexdump(stdout, "hex", dot_ip, 64);
           s = j;
            continue;
        }

       while(ip[i]){
           tmp_ip[pos] = ip[i] % 10 + '0';
           ip[i] /= 10;
           len++;
           pos++;
       }
       tmp_ip[pos] = '\0';

       printf("j:%d  pos:%d %s\n",j, pos, dot_ip);
       while(pos >= 0){
           dot_ip[j++] = tmp_ip[pos--];
       }
       printf("j:%d  pos:%d %s\n",j, pos, dot_ip);
       //rte_hexdump(stdout, "hex", dot_ip, 64);

       dot_ip[s] = len;
       pos = 0;
       len = 0;
       s = j;
    }

    dot_ip[j++] = 7;
    memcpy(dot_ip + j, "in-addr", 7);
    j += 7;

    dot_ip[j++] = 4;
    memcpy(dot_ip + j, "arpa", 4);
    j += 4;

    dot_ip[j++] = 0;

   rte_hexdump(stdout, "hex", dot_ip, j);
    return j;
    
}

void
mtd_build_dns_map(uint64_t hash)
{
    uint8_t tail_bt, ns_name[64], tmp_name[64];
    int len, i;

    i = 0;
    while(hash){
        tail_bt = hash & 0xf;

        printf("%d ",tail_bt);
        if(tail_bt < 10)
            tail_bt += '0';
        else
            tail_bt += 'a' - 0xa;

        tmp_name[i++] = tail_bt;
        hash >>= 4;
    }
        printf("\n");
    printf("%s  %d\n",tmp_name,i);

    len = 0;
    while(i>=0){
        ns_name[len++] = tmp_name[i--];
    }
    printf("%s\n",ns_name+1);


    ns_name[0] = len;
    ns_name[len] = 3;
    len += 1;
    memcpy(ns_name + len, "com", 3);
    len += 3;
    ns_name[len++] = 0;
    
   rte_hexdump(stdout, "ns_name", ns_name, len);
}

int main(void)
{
    uint32_t ip = 1062654075;
    uint64_t hash = 0xee400B6C816220CB;

    mtd_build_ptr_map(&ip);
    mtd_build_dns_map(hash);
}
