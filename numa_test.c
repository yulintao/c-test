#include <stdio.h>
#include <numa.h>

int main(void)
{
	printf("%d\n",numa_max_node());
}
