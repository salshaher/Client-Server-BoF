/* hackedserver.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 44444
#define BUFFERSIZE  1024

unsigned char inputBuffer[BUFFERSIZE];
unsigned char outputBuffer[BUFFERSIZE];


int handleConnection(unsigned int new_socket) {
	
	unsigned char str[] = "\r\nWelcome to Crypto server. What's your name ?\r";
	int position, len, nbytes, i, *test;
	unsigned char buf[80], *cc;


	if (send(new_socket, str, sizeof(str), 0) > 0) {
		printf("First message sent...\n");
	}
	else {
		perror("Cannot read from socket: ");
		close(new_socket);
		return (-1);	
	}
	if (nbytes = recv (new_socket, inputBuffer, sizeof(inputBuffer) , 0) > 0) { 

	    /* Prints the new return address (without the offset) */
		printf("%p the add\n",&buf[0]);    
		/* Prints the content of inputBuffer at location 88 (to get indications) */ 
		printf("%d\n",inputBuffer[88]);
		/* Prints the content of inputBuffer at location 0 (to get indications) */ 
		printf("%d\n",inputBuffer[0]);

		cc =(unsigned char * ) memchr(inputBuffer, '|', BUFFERSIZE);
	
		if (cc) {
			*cc = '\0';
		}
		else {
			printf("Invalid Request!!\n");
			close(new_socket);
			return (-1);
		}

		len = (int) (cc - inputBuffer);

		memcpy(buf, inputBuffer, (len+1));
		// to know where len is stored
		printf("%d the len",len);
		sprintf(outputBuffer,"\r\nHello %s, can you overflow me???\r\n", buf);
		
		if(send(new_socket, outputBuffer, strlen(outputBuffer), 0) > 0) {
			printf("Response sent...\n");
		}
		else {
			perror("Cannot write to socket: ");
			close(new_socket);
			return (-1);
		}
	}
	else {
		perror("Cannot read from socket: ");
		close(new_socket);
		return (-1);
	}
	return 0;
}




int main(void)  {

	unsigned int sockfd_in, new_socket,addrlen;
	struct sockaddr_in address;
	int res;

        if((sockfd_in = socket(AF_INET, SOCK_STREAM, 0)) > 0) 
                printf("Socket assigned -> %d\n", sockfd_in);
        else {
		perror("Cannot create Socket: ");
                exit(0);
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr(SERVER_IP);
        address.sin_port = htons(SERVER_PORT);

        /* Bind socket */
        if(bind(sockfd_in, (struct sockaddr *)&address, sizeof(address)) >= 0) 
	{
                printf("Awaiting connections ...\n");
		fflush(stdout);
        }

        while (1) { /* Keep line open all time */


                /* listen for connection */
                listen(sockfd_in, 4);

                addrlen = sizeof(struct sockaddr_in);

                /* accept connection request from the client */
                new_socket = accept(sockfd_in, (struct sockaddr *)&address, &addrlen);

		if (new_socket > 0) 
			printf("New connection established with client !\n\n");
		else {
			perror("Error while creating new_socket");
			return (-1);
		}


		res = handleConnection(new_socket);

		if (res<0) 
			printf("Connection was not handled!!\n");
		else 
			printf("Connection handled successfully\n");
	}

	return 0;
}


