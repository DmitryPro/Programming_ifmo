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
	int listener, newsockfd, portno, clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int  n;
   int bytes_read, bytes_write;

   listener = socket(AF_INET, SOCK_STREAM, 0);
   if(listener < 0) {
   		perror("socket");
   		exit(1);
   }
   portno = 8080;
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);

    if (bind(listener, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
      perror("bind");
      exit(2);
     }
     
    listen(listener,1);
    while(1) {
    	newsockfd = accept(listener, NULL , NULL);
    	if(newsockfd < 0) {
    		perror("accept");
    		exit(3);
    	}
    	switch(fork()) {
    		case -1:
    			perror("fork");
    			break;
    		case 0:
    			close(listener);
    			while(1) {
            int proc = getpid();
            printf("%d\n", proc);
    				bytes_read = recv(newsockfd, buffer,256,0);
    				if(bytes_read < 0){
    					break;
    				}
    				int bytes_write = send(newsockfd,buffer,256,0);
    				if(bytes_write < 0) {
    					perror("write");
    					exit(4);
    				}
    			}
    			close(newsockfd);
    			_exit(0);
    		default:
    			close(newsockfd);
    		}
    	}
    close(listener);
    return 0;
}