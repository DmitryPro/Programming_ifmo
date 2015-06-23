#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <stdio.h>
#include <unistd.h>
#include "semaphore.h"

int main()
{
	int i;
	if(fork() == 0) {
		// child
		printf("Hi. I am child.\n");
		P();
		for(i = 0; i < 10; i ++){
			printf("Hello world from child.\n");
		}
		V();
	} else {
		// parent
		printf("Hi. I am parent.\n");
		P();
		for(i = 0; i < 10; i ++){
			printf("Hello world from parent.\n");
		}
		V();
	
	}
	return 0;
}