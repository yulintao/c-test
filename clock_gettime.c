/*
 * =====================================================================================
 *
 *       Filename:  clock_gettime.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  08/29/2016 10:45:35 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titan Bao (), leightonbao@gmail.com
 *        Company:  Superlink
 *
 * =====================================================================================
 */



#include <unistd.h>
#include <sys/time.h>

int main()
{
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
}
