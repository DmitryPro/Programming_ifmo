#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semaphore.h"

#define key 1234
#define MSGSZ     128

struct msgbuf {
	long mtype;
	char mtext[MSGSZ];
};

void send(short type){
	int msqid;
	int msgflg = IPC_CREAT | 0666;
	struct msgbuf sembuf;

	if ((msqid = msgget(key, msgflg )) < 0) {
        perror("msgget");
        exit(1);
    } else {
    	sembuf.mtype = type;
    	(void) strcpy(sembuf.mtext, "Did you get this?");
    	if (msgsnd(msqid, &sembuf, strlen(sembuf.mtext) + 1, IPC_NOWAIT) < 0) {
    		perror("msgsnd");
    		exit(1);
    	}
    }
}

void receive(short type){
	int msqid;
	int msgflg = 0666;
	struct msgbuf sembuf;

	if ((msqid = msgget(key, msgflg )) < 0) {
        perror("msgget");
        exit(1);
    } else {
		if (msgrcv(msqid, &sembuf, MSGSZ, type, 0) < 0) {
	        perror("msgrcv");
	        exit(1);
	    }
	}
}


/**
from msgrcv() docs:

If no message of the requested type is available and IPC_NOWAIT isn't specified in msgflg,
 the calling process is blocked until one of the following conditions occurs:
 1. A message of the desired type is placed in the queue.
 2. The message queue is removed from the system. In this case the system call fails with errno set to EIDRM.
 3. The calling process catches a signal. In this case the system call fails with errno set to EINTR. (msgrcv()
  is never automatically restarted after being interrupted by a signal handler, regardless of the setting of the
   SA_RESTART flag when establishing a signal handler.)
   
from control server :
   	receive(1);
	send(2);
	
next operations p() will be blocked until current thread don't execute V().


**/

void P(){
	send(1);
	receive(2);
}

void V(){
	send(3);
}
