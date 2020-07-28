/*
 * =====================================================================================
 *
 *       Filename:  test_gtopt.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  07/28/2016 06:35:09 AM
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

	int
main(int argc , char **argv)
{
	int sock, rc;
	socklen_t optlen;
	struct sockaddr_in server;
	unsigned char optval;

	/*  create socket */
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1) {
		printf("Could not create socket");
		exit(EXIT_FAILURE);
	}

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	//server.sin_addr.s_addr = inet_addr("10.10.10.83");
	server.sin_family = AF_INET;
	server.sin_port = htons(8099);

	/*  Try connecting to an arbitrary closed port */
	connect(sock , (struct sockaddr *)&server , sizeof(server));

	rc = getsockopt(sock, SOL_SOCKET, SO_ERROR,
			&optval, &optlen);

	fprintf(stderr, "rc returns: %d, optval is: %d, optlen is: %d\n",
			rc, optval, optlen);

	return EXIT_SUCCESS;
}
