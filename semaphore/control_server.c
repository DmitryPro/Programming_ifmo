#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <stdio.h>
#include "semaphore.h"

int main(int argc, char*argv[]){
	printf("Semaphore initialized.\n");
	while(1){
		receive(1);
		send(2);
		// fprintf(stderr,"P(S) operation performed.\n");
		printf("P(S) operation performed.\n");
		receive(3);
		// fprintf(stderr,"V(S) operation performed.\n");
		printf("V(S) operation performed.\n");
	}
	return 0;
}