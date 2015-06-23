#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	int listener, portno;
	socklen_t clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int  n;
   int bytes_read, bytes_write;

   listener = socket(AF_INET, SOCK_DGRAM, 0);
   if(listener < 0) {
   		perror("socket");
   		exit(1);
   }
   portno = 8080;
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
   serv_addr.sin_port = htons(portno);
    if (bind(listener, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
      perror("bind");
      exit(2);
     }

    while(1) {
    	clilen = sizeof(cli_addr);
    	if (recvfrom(listener, buffer, 256, 0, (struct sockaddr *) &cli_addr, &clilen) == -1)
        {
            perror("recvfrom");
            exit(3);
        }
      //  printf("Heey\n");
        printf("%s\n", buffer);
    	if (sendto(listener, buffer, 256, 0, (struct sockaddr*) &cli_addr, clilen) == -1)
        {
            perror("sendto");
        }
    }
    close(listener);
    return 0;
}