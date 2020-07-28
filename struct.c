#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define RTE_CACHE_LINE_SIZE 64                  /**< Cache line size. */
#define uint32_t unsigned int 
#define uint16_t unsigned short
#define uint64_t unsigned long long int 
#if 0
struct ip_proxy{
	uint32_t proxy_ip;
	uint32_t syn_anti;
	uint32_t threshold;/**< if syn_cnt >= threshold then drop */
	uint32_t syn_ticks; /**< ticks when equal jiffes anti begin */
	uint32_t syn_cnt; /**< total syn cnt per second */
	uint32_t syn_pass;/**< total passed syn */
	uint32_t ack_anti;
	uint32_t ack_cnt; /**< total ack cnt per second */
	uint32_t ack_pass;/**< total passed ack */
	uint32_t ack_ticks;/**< ticks when equal jiffes anti begin */
	uint32_t icmp_cnt;
	uint32_t udp_cnt;
}__attribute__((__aligned__(64)));
#endif
typedef union epoll_data {
	void         *ptr;
	int           fd;
	uint32_t      u32;
	uint64_t      u64;
} epoll_data_t;

struct ip_{
	uint32_t udp_cnt;
	void *ptr[4];
	//    epoll_data_t  data;
};//__attribute__((__aligned__(4)));

typedef struct hds_k5v{
	uint32_t saddr,taddr;
	uint16_t sport,tport,proto;
}__attribute__((__aligned__(4))) hds_k5v_t;

enum hds_tcp_state {
	HDS_CLOSED      = 0,
	HDS_LISTEN      = 1,
	HDS_SYN_SENT    = 2,
	HDS_SYN_RCVD    = 3,
	HDS_ESTABLISHED = 4,
	HDS_FIN_WAIT_1  = 5,
	HDS_FIN_WAIT_2  = 6,
	HDS_CLOSE_WAIT  = 7,
	HDS_CLOSING     = 8,
	HDS_LAST_ACK    = 9,
	HDS_TIME_WAIT   = 10
};
typedef struct hds_node{
	/*	struct hds_node *next; */
	uint32_t first_rx_time;
	uint32_t last_rx_time;
	uint16_t rcv_times;
	uint16_t rcv_cur_bkts;
	uint32_t rcv_nxt;
	enum hds_tcp_state state;
	struct hds_k5v kv;	
}hds_node_t;
typedef struct hds_pip_info{
	uint32_t ip;
	uint32_t time;
	uint32_t current_pkt;
	uint32_t nop;
	uint32_t syn[32];
	uint32_t ack[32];
	uint32_t icmp[32];
	uint32_t udp[32];
	uint64_t ibytes[32];
	uint64_t obytes[32];
}hds_pip_info_t;


#if 0
struct te{
	struct ip_proxy b[128];
};
#endif
struct rbnode {               
	struct rbnode *parent;    
	struct rbnode *left;      
	struct rbnode *right;     
	int color;                
	void *key;                
};

struct abc{
#define aaa(type, name) type name;
	aaa(int, abcd)
#undef aaa
};

struct abcb{
#define aaa(type, name) type name;
	aaa(float, acd)
#undef aaa
};

int main(void)
{
	//	struct ip_proxy aa,*b;
	//	struct te *c;
	//	c = (struct te*)malloc(256*sizeof(struct te));
	//	printf("%lu %lu %lu  %ld  %d\n",sizeof(aa),sizeof(b),sizeof(struct te),c[1].b - c[0].b,0xa^0xa);
	struct ip_ *a;
	int i ,j,k;
	struct rbnode b = {&i,&j,&k};

	struct rbnode *p = malloc(sizeof(struct rbnode));

	struct abcb abcd;
	printf("%lu  %lu\n",sizeof(abcd),sizeof(struct abc));
	*p = b;
	printf(":%p :%p :%d:%p :%p \n",p->parent,&i,p->color,p->right,&k);
	for(i=0;i<0;i++);
	printf("%d pack:%lu\n",i,sizeof(hds_k5v_t));
	printf("pack:%lu\n",sizeof(hds_pip_info_t));
#if 0
	a = malloc(sizeof(*a));
	a->udp_cnt = 1;
	a->ptr[0] = malloc(10);
	a->ptr[1] = malloc(10);
	sprintf(a->ptr[0],"%s","test");
	sprintf(a->ptr[1],"%s","test1");
	printf("sizeof(epoll):%lu _(0)_%s  _(2)_%s %d\n",sizeof(struct ip_),a->ptr[0],a->ptr[1],a->udp_cnt);
#endif
	printf("sizeof(epoll):%lu %lu\n",sizeof(struct ip_),sizeof(hds_node_t));
}
