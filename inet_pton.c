#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
mtd_is_local_ptr(uint8_t *lowerdomain[])
{
    uint32_t domain_ip;
    int i, j, l, k, len;
    uint8_t ip_buf[INET_ADDRSTRLEN],  *label;

    k = 0;
    for(i = 0; i < 4; i++){
        l = 0;
        label = lowerdomain[i];
        len = label[0];
        label++;
        while (len-- != 0) {
            ip_buf[k++] = label[l++] ;
        }
        ip_buf[k] = '.';
        k++;
    }
    ip_buf[--k] = '\0';
    /*domain_ip = (uint32_t *)ip_buf;*/

    rte_hexdump(stdout, "BBBBBBB", ip_buf,16);
    printf("ipxxxx: %s\n", ip_buf);
    inet_pton(AF_INET, ip_buf, &domain_ip);
    printf("ip:%x\n",domain_ip);

}

int
main(int argc, char *argv[])
{
    unsigned char buf[sizeof(struct in6_addr)];
    int domain, s;
    char str[INET6_ADDRSTRLEN];
    unsigned char ip_str[16];
    unsigned char *ip_str2[16];
    uint32_t ipaaa;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s {i4|i6|<num>} string\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    ip_str[0] = 1;
    ip_str[1] = '1';
    ip_str[2] = 1;
    ip_str[3] = '2';
    ip_str[4] = 1;
    ip_str[5] = '3';
    ip_str[6] = 1;
    ip_str[7] = '4';
    ip_str[8] = '\0';
    ip_str2[0] = ip_str;
    ip_str2[1] = ip_str + 2;
    ip_str2[2] = ip_str + 4;
    ip_str2[3] = ip_str + 6;

    mtd_is_local_ptr(ip_str2);

    domain = (strcmp(argv[1], "i4") == 0) ? AF_INET :
              (strcmp(argv[1], "i6") == 0) ? AF_INET6 : atoi(argv[1]);

    rte_hexdump(stdout, "aaaaaaa", argv[2],16);

    s = inet_pton(domain, argv[2], buf);
    if (s <= 0) {
        if (s == 0)
            fprintf(stderr, "Not in presentation format");
        else
            perror("inet_pton");
            exit(EXIT_FAILURE);
    }
    printf("%x\n",*(uint32_t *)buf);

    rte_hexdump(stdout, "CCCCCCCC", buf,16);
    if (inet_ntop(domain, buf, str, INET6_ADDRSTRLEN) == NULL) {
        perror("inet_ntop");
        exit(EXIT_FAILURE);
    }

    printf("%s\n", str);

    exit(EXIT_SUCCESS);
}

