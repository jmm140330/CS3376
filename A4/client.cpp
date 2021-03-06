﻿#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 2800 /* server port – you need to change this */

int main(int argc, char **argv) {          // the argument is the server's IP address

	int sockfd;
	struct sockaddr_in servaddr;
	char sendline[MAXLINE], recvline[MAXLINE];
	
	//basic check of the arguments
	//additional checks can be inserted
	if (argc !=2) {
		perror("Usage: TCPClient <IP address of the server.");
		exit(1);
	}

	//Create a socket for the client
	//If sockfd<0 there was an error in the creation of the socket
	if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
		perror("Problem in creating the socket");
		exit(2);
	}
		
	//Creation of the socket
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr= inet_addr(argv[1]);
	servaddr.sin_port =  htons(SERV_PORT); //convert to big-endian order
	
	//Connection of the client to the socket
	if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
		perror("Problem in connecting to the server");
		exit(3);
	}

	std::ofstream client_log;
	client_log.open("client_log.txt", std::ios::app);

	std::cout << "Enter request: ";

	while (fgets(sendline, MAXLINE, stdin) != NULL) {
		client_log << "Request sent to server: " << sendline << std::endl;
		send(sockfd, sendline, strlen(sendline), 0);
		if (recv(sockfd, recvline, MAXLINE,0) == 0){
			//error: server terminated prematurely
			perror("The server terminated prematurely");
			exit(4);
		}
		
		printf("%s", "\nString received from the server: ");
		std::cout << recvline << std::endl;
		client_log << "Result received from the server: " << recvline << std::endl;
		std::cout << "\nEnter request: ";
	}
	
	client_log.close();

	exit(0);
}
