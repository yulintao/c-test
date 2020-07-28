/*
 * =====================================================================================
 *
 *       Filename:  getopt.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/27/2016 10:12:23 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Titan Bao (), leightonbao@gmail.com
 *        Company:  Superlink
 *
 * =====================================================================================
 */


#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <mtcp_api.h>
int
main(int argc , char **argv)
{
	int sock, rc;
	socklen_t optlen;
	struct sockaddr_in server;
	unsigned char optval;
	struct mtcp_conf mcfg;
	mctx_t mctx;


	mtcp_getconf(&mcfg);
	mcfg.num_cores = 1;
	mtcp_setconf(&mcfg);
	/*  initialize the mtcp context */
	if (mtcp_init("mtcp.conf")) {
		fprintf(stderr, "Failed to initialize mtcp\n");
		exit(0);
	}
	mtcp_getconf(&mcfg);
	mcfg.max_concurrency = mcfg.max_num_buffers = 500000;
	mtcp_setconf(&mcfg);
	mctx = mtcp_create_context(0);

	/*  create socket */
	sock = mtcp_socket(mctx,AF_INET , SOCK_STREAM , 0);
	if (sock == -1) {
		printf("Could not create socket");
		exit(EXIT_FAILURE);
	}

	server.sin_addr.s_addr = inet_addr("192.168.1.99");
	server.sin_family = AF_INET;
	server.sin_port = htons(80);

	fprintf(stdout, "rc returns: %d, optval is: %d, optlen is: %d\n",
			rc, optval, optlen);
	/*  Try connecting to an arbitrary closed port */
	mtcp_connect(mctx,sock , (struct sockaddr *)&server , sizeof(server));

	rc = mtcp_getsockopt(mctx,sock, SOL_SOCKET, SO_ERROR,(void *)&optval, &optlen);
	//TRACE_CONFIG("rc returns: %d, optval is: %d, optlen is: %d.\n",rc, optval, optlen);

	fprintf(stdout, "rc returns: %d, optval is: %d, optlen is: %d\n",
			rc, optval, optlen);
while(1)
	sleep(10);
	return EXIT_SUCCESS;
}
