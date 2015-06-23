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

void P(){
	send(1);
	receive(2);
}

void V(){
	send(3);
}