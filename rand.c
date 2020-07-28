#include <sys/time.h>
#include <linux/tcp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define odp_handle_t struct { uint8_t unused_dummy_var; } *
/* * C/C++ helper macro for strong typing */
#define ODP_HANDLE_T(type) odp_handle_t type

/* * ODP buffer */
typedef ODP_HANDLE_T(odp_buffer_t);
#define _odp_cast_scalar(type, val) ((type)(uintptr_t)(val))

/* * Invalid buffer */
#define ODP_BUFFER_INVALID _odp_cast_scalar(odp_buffer_t, 0xffffffff)


int main(void)
{
	char *p[3] = {0};

	srand(time(NULL));
	printf("sizeof_odp_handle:%ld\n",sizeof(odp_buffer_t));
	printf("ODP_BUFFER_INVALID:%x\n",ODP_BUFFER_INVALID);
	while(1){
		sleep(2);
		printf("%u\n",rand());
	}
}
