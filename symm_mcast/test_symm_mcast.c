#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <sys/socket.h>  
#include <netinet/in.h>  
#include <arpa/inet.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/types.h>  
#include <strings.h>
#include <syslog.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ctype.h>

#include <arpa/inet.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <netinet/if_ether.h>
#include <signal.h>
int recv_fd, send_fd;
int raw_sock;
struct sockaddr_in mcast_group;
char mcast_ip[16]="224.0.1.2";
int port = 5100;
uint32_t gip,ghash,myip;
char mac[18];
char gw_mac[18] = {0};

char dev[12];
unsigned int alias[256];
char mask[16];

static int get_gw_mac(unsigned int ip)
{
	FILE *fp;
	struct in_addr s;
	char *p;
	char buf[256],tmp_ip[256],tmp_mac[256];

	fp = fopen("/proc/net/arp","r");
	if(!fp)
		return(-1);

	memcpy(&s, &ip, 4);
	p = inet_ntoa(s);

	while (fgets(buf, 256, fp)){
		if (sscanf(buf, "%s %*s %*s %s %*s %*s", tmp_ip, tmp_mac) > 0){
			if (strcmp(tmp_ip, p) == 0){
				fclose(fp);
				strncpy(gw_mac, tmp_mac, sizeof(gw_mac)-1);
				return(0);
			}
		}
	}

	fclose(fp);
	return(-1);
}

int get_default_routedev(unsigned int *gw)
{
	FILE *fp;
	char tmp1[256],tmp2[256],tmp3[256];

	fp = fopen("/proc/net/route","r");
	if(!fp)
		return(-1);

	while (!feof(fp)){
		if (fscanf(fp,"%s %s %s %*s %*s %*s %*s %*s %*s %*s %*s",tmp1,tmp2,tmp3) > 0){
			if (strcmp(tmp2,"00000000")==0){
				fclose(fp);
				strncpy(dev, tmp1, sizeof(dev));
				dev[sizeof(dev)-1] = '\0';
				*gw = strtoul(tmp3,0,16);

				get_gw_mac(*gw);

				return(0);
			}
		}
	}

	fclose(fp);
	return(-1);
}

int get_iplist(unsigned int *pip, unsigned int *pimask, char *iplist, unsigned *pgw)
{
	struct sockaddr_in *s_ip = NULL;
	struct sockaddr_in *s_mask = NULL;
	struct ifconf ifc; 
	struct ifreq ifr[1024], ifrmask; 
	struct ifreq req;
	char ethname[16]="0";
	char ethip[16];
	char ethmask[16];
	int  i, n, numif, fd, id; 
	unsigned int gw;
	char *p = iplist;
	char *palias;
	unsigned char m[ETH_ALEN]={0};

	memset(ifr, 0, sizeof(ifr));
	if ((fd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		close(fd);
		printf( "Can't create socket for get_iplist, so exit");
		exit(-1);
	}
	ifc.ifc_len = sizeof(ifr);
	ifc.ifc_req = ifr;
	if (ioctl(fd, SIOCGIFCONF, &ifc) < 0) {
		close(fd);
		printf( "Can't ioctl for get_iplist, so exit");
		exit(-1);
	}
	numif = ifc.ifc_len / sizeof(struct ifreq);

	if(get_default_routedev(&gw)){
		printf( "Can't get default gateway, so exit");
		exit(-1);
	}

	*pgw = gw;

	for (i = 0, n = 0; i < numif; i++)
	{
		if(strncmp(ifr[i].ifr_name, dev, strlen(dev)))
			continue;
		n++;
		strncpy(ethname, ifr[i].ifr_name, sizeof(ethname));
		strcpy(ifrmask.ifr_name, ethname);

		ioctl(fd, SIOCGIFNETMASK, &ifrmask);

		s_ip = (struct sockaddr_in *)&ifr[i].ifr_addr;
		strncpy(ethip, inet_ntoa(s_ip->sin_addr), sizeof(ethip));
		palias = strchr(ethname, ':');
		if(palias++){
			id = atoi(palias);
			if((id > 255) || (id < 0)) 
				printf( "can't handle dev: %s  ip: %s", dev, ethip);
			else if(alias[id] == 0)
				alias[id] = 1;
			else{
				printf( "can't handle exist ip: %s  ip: %s", dev, ethip);
				continue;
			}
		}

		s_mask = (struct sockaddr_in *)&ifrmask.ifr_addr;
		strncpy(ethmask, inet_ntoa(s_mask->sin_addr), sizeof(ethmask)); 
		//printf("%-10s%-17s%-17s\n", ethname, ethip, ethmask);

		if((*pip == 0) && (s_ip->sin_addr.s_addr & s_mask->sin_addr.s_addr) == (gw & s_mask->sin_addr.s_addr)){
			strcpy(mask, ethmask);
			*pimask = s_mask->sin_addr.s_addr;
			*pip = s_ip->sin_addr.s_addr;
		}
		sprintf(p, "%s,", inet_ntoa(s_ip->sin_addr));
		p += strlen(p);
	}
	*(p+strlen(p)-1) = '\0';

	//get mac
	strcpy(req.ifr_name, dev);
	req.ifr_hwaddr.sa_family = ARPHRD_ETHER;
	if (ioctl(fd, SIOCGIFHWADDR, &req) < 0) {
		close(fd);
		printf( "ioctl for get mac error, so exit.\n");
		exit(-1);
	}

	close(fd);

	memcpy(m, req.ifr_hwaddr.sa_data, ETH_ALEN); 
	sprintf(mac,"%02x:%02x:%02x:%02x:%02x:%02x", m[0], m[1], m[2], m[3], m[4], m[5]);
	fprintf(stderr, "myself ip=%u %s mac=%s iplist=%s\n", *pip, inet_ntoa(*(struct in_addr*)pip), mac, iplist);

	return 0;
}

int symmetric_condition_init(void)
{
	u_int yes = 1;
	u_char enable = 1;
	struct ip_mreq mreq;
	//unsigned int ip=0;
	//unsigned int imask;
	//unsigned int gw;
	//char iplist[1280];


	//get_iplist(&ip, &imask, iplist, &gw);

	memset(&mcast_group,0,sizeof(mcast_group));
	mcast_group.sin_family = AF_INET;
	mcast_group.sin_port = htons(port);
	mcast_group.sin_addr.s_addr = inet_addr(mcast_ip);

	if ((send_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("can't create send socket\n");
		return -1;
	}

	if ((recv_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("can't create recv socket\n");
		return -1;
	}

	if (setsockopt(recv_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
		printf("can't set reuseaddr setsockopt\n");
		return -1;
	}

	if (setsockopt(recv_fd, IPPROTO_IP, IP_MULTICAST_LOOP, &enable, sizeof(enable)) < 0) {
		printf("can't set multicast_loop setsockopt\n");
		return -1;
	}

	if (bind(recv_fd, (struct sockaddr*)&mcast_group, sizeof(mcast_group)) < 0) {
		printf("can't bind recv_fd\n");
		return -1;
	}

	mreq.imr_multiaddr = mcast_group.sin_addr;
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	if (setsockopt(recv_fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
		printf("can't set add_membership setsockopt\n");
		return -1;
	}

}
#define LINE_LEN 128

void
rte_hexdump(FILE *f, const char * title, const void * buf, unsigned int len)
{
    unsigned int i, out, ofs;
    const unsigned char *data = buf;
    char line[LINE_LEN];    /* space needed 8+16*3+3+16 == 75 */

    fprintf(f, "%s at [%p], len=%u\n", (title)? title  : "  Dump data", data, len);
    ofs = 0;
    while (ofs < len) {
        /* format the line in the buffer, then use printf to output to screen */
        out = snprintf(line, LINE_LEN, "%08X:", ofs);
        for (i = 0; ((ofs + i) < len) && (i < 16); i++)
            out += snprintf(line+out, LINE_LEN - out, " %02X", (data[ofs+i] & 0xff));
        for(; i <= 16; i++)
            out += snprintf(line+out, LINE_LEN - out, " | ");
        for(i = 0; (ofs < len) && (i < 16); i++, ofs++) {
            unsigned char c = data[ofs];
            if ( (c < ' ') || (c > '~'))
                c = '.';
            out += snprintf(line+out, LINE_LEN - out, "%c", c);
        }
        fprintf(f, "%s\n", line);
    }
    fflush(f);
}


void *symmetric_recv_process(__attribute__((unused)) void *dummy)
{
#define MAXLEN 1024
	char buffer[MAXLEN + 1];
	struct sockaddr_in addr;
	int addr_len,ret;
	uint32_t ip,hash;

	while(1){
		memset(buffer, 0, sizeof(buffer));
		ret = recvfrom(recv_fd, buffer, MAXLEN, 0,
				(struct sockaddr *)&addr, (socklen_t *)&addr_len);
		if(ret > 0){
			sscanf(buffer,"%u %u",&ip,&hash);
			if((addr.sin_addr.s_addr == myip)||(addr.sin_addr.s_addr == 0))
				continue;
//rte_hexdump(stdout,"recv",buffer,32);
			//	set_white(hash);
			printf("symmetric white list done,ip:%u %u hash:%u\n",(addr.sin_addr.s_addr),ip,hash);
		}
	}

}


void *send_process(__attribute__((unused)) void *dummy)
{

	char buff[MAXLEN + 1];

	while(1){
		sprintf(buff,"%u %u",gip,ghash); 
//rte_hexdump(stdout,"send",buff,32);
		if(sendto(send_fd, buff, 32, 0, (const struct sockaddr *)&mcast_group, sizeof(mcast_group)) == -1) {
			printf("send detail_msg error, type= %u msg='%u'",gip,ghash);
		}
		sleep(2);
	}

}

int main(int argc,char *argv[])
{
	pthread_t thread_recv_id = 0;
	pthread_t thread_send_id = 0;
	char iplist[1280];
	unsigned int imask;
	unsigned int gw;

struct in_addr inp;
	if(argc < 3)
		printf("%s ip hash\n",argv[0]);
	inet_aton(argv[1],&inp);
        gip=inp.s_addr;
	ghash = atoi(argv[2]);
	printf("gip:%u  ghash:%u\n",gip,ghash);

	get_iplist(&myip,&imask,iplist,&gw);
	symmetric_condition_init();
	if(pthread_create(&thread_recv_id, NULL, symmetric_recv_process, NULL) < 0){
		printf("error : can't create thread of handle_recv_process\n");
		return -1;
	}
	if(pthread_create(&thread_send_id, NULL, send_process, NULL) < 0){
		printf("error : can't create thread of handle_send_process\n");
		return -1;
	}
	pthread_join(thread_recv_id,NULL);
	pthread_join(thread_send_id,NULL);
}


