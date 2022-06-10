/*	server.cpp
main file.
*******************************************************************/
#include <sys/types.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <string.h>
// #include <pthread.h>
#include <iostream>
#define ECHOMAX 516

using namespace std;
//This represents the list of accounts.

//**************************************************************************************
// function name: server
// Description: main function. get requests from clients, returns ACK packets to client
//**************************************************************************************


void error(char *msg) {
	perror(msg);
	exit(1);
}


int main(int argc, char *argv[]) {  
	int sock; /* Socket */
	struct sockaddr_in echoServAddr; /* Local server address */
	struct sockaddr_in echoClntAddr; /* Client address */
	unsigned int cliAddrLen; /* Length of incoming message */
	char echoBuffer[ECHOMAX]; /* Buffer for echo string */
	unsigned short echoServPort; /* Server port */
	int recvMsgSize; /* Size of received message */ 
	
	echoServPort = atoi(argv[1]);
	time_t timeout = atoi(argv[2]);
	unsigned int max_num_of_resends = atoi(argv[3]);

	/* Create socket for receiving datagrams and sending ACK packets */
	if ((sock = socket(AF_INET, SOCK_DGRAM,/* IPPROTO_UDP*/ 0)) < 0) {
		error("TTFTP_ERROR:");
	}

	/* Zero out structure */
	memset(&echoServAddr, 0, sizeof(echoServAddr)); 
	memset(&echoClntAddr, 0, sizeof(echoServAddr)); 

	/* Internet address family */
	echoServAddr.sin_family = AF_INET;

	/* Any incoming interface */
	echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Local port */
	echoServAddr.sin_port = htons(echoServPort);

	/* Bind to the local address */
	if (bind(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0) {
		error("TTFTP_ERROR:"); 
	}

	while (1) { /* Run forever */

		/* Set the size of the in-out parameter */
		cliAddrLen = sizeof(echoClntAddr);

		/* Block until receive message from a client */
		if ((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr *) &echoClntAddr, &cliAddrLen)) < 0) {
			error("recvfrom() failed");
		}
		printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr)); 

		/* Send received datagram back to the client */
		if (sendto(sock, echoBuffer, recvMsgSize, 0, (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) != recvMsgSize) {
			error("sendto() sent a different number of bytes than expected");
		}
	}
	/* NOT REACHED */
	return 0;
}

