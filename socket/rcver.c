#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "proto.h"

int main()
{

	int sd;
	struct sockaddr_in laddr, raddr;
	struct msg_st *rbufp;
	char ipstr[IPSTRSIZE];
	int size;
	socklen_t raddr_len;
	
	size = sizeof(struct msg_st) + NAMEMAX - 1;

	rbufp = malloc(size);
	if ( rbufp == NULL )
	{
		perror("malloc()");
		exit(1);
	}

	sd = socket(AF_INET, SOCK_DGRAM, 0/*IPPROTO_UDP*/);
	if ( sd < 0 )
	{
		perror("socket()");
		exit(1);
	}

	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi(RCVPORT));
	printf("%d\n", atoi(RCVPORT));
	inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr);
	
	printf("ip is %lu\n", laddr.sin_addr.s_addr);
	
	if ( bind(sd, (void *)&laddr, sizeof(laddr)) < 0 )
	{
		perror("bind()");
		exit(1);
	}

	raddr_len = sizeof(raddr);
	while (1)
	{
		recvfrom(sd, rbufp, size, 0, (void *)&raddr, &raddr_len);
		inet_ntop(AF_INET, &raddr.sin_addr, ipstr, IPSTRSIZE);
		printf("---MESSAGE FROM %s:%d---\n", ipstr, ntohs(raddr.sin_port));
		printf("NAME = %s\n", rbufp->name);
		printf("MATH = %d\n", ntohl(rbufp->math));
		printf("CHINESS = %d\n", ntohl(rbufp->chinese));
	}

	close(sd);

	exit(0);
}

