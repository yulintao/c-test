#include <stdio.h>
#include <linux/types.h>
#define uint16_t unsigned short 
#define uint32_t unsigned int
#define uint64_t unsigned long long int


typedef struct {    
            uint64_t key[2];
} siphash_key_t;    

static siphash_key_t syncookie_secret[2] ={  
    {0x75ULL,0x15ULL},        
    {0x705ULL,0x71ULL},         
};                                                         

static inline void get_random_once(uint32_t saddr, 
				   uint32_t daddr, 
				   uint16_t sport, 
				   uint16_t dport)  
{                                                                   
syncookie_secret[0].key[0] ^= syncookie_secret[0].key[1];   
syncookie_secret[1].key[0] ^= syncookie_secret[1].key[1];   
syncookie_secret[0].key[1] ^= syncookie_secret[1].key[0];   
syncookie_secret[1].key[1] ^= syncookie_secret[0].key[0];   

}                                                                   



int main(void)
{
    int i,j;
    uint32_t tk=0;

    while(1)
    {
	for(i = 0;i < 2;i++){
	    for(j = 0; j < 2; j++){
		printf("continue:%llx\n",syncookie_secret[i].key[j]);
		if(syncookie_secret[i].key[j] == 0)
		    printf("i%d ,j%d\n",i,j);
	    }
	}
	if(syncookie_secret[0].key[0] == syncookie_secret[0].key[1] == 
	   syncookie_secret[1].key[0] == syncookie_secret[1].key[1])
	    break;
	tk++;
	get_random_once(0,0,0,0);
	usleep(10000);
    }
    printf("over:%d\n",tk);
}
