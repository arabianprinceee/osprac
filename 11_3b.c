#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define LAST_MESSAGE 255 // Message type for termination of program 11-1b.c

int main(void)
{
  int msqid;      // IPC descriptor for the message queue
  char pathname[]="11_3a.c"; // The file name used to generate the key.
                             // A file with this name must exist in the current directory.
  key_t  key;     // IPC key
  int i,len;      // Cycle counter and the length of the informative part of the message

  struct mymsgbuf // Custom structure for the message
  {
    long mtype;
    struct{
      long pid;
      float mes;
    }values;
  } mybuf;
  int maxlen;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }
  //
  // Trying to get access by key to the message queue, if it exists,
  // or create it, with read & write access for all users.
  //
  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  /* Send information */
    while(1){
    // Fill in the structure for the message and
    // determine the length of the informative part.
    //
    //receive message
    maxlen = sizeof(mybuf.values);
    if (( len = msgrcv(msqid, (struct msgbuf *) &mybuf,len, 1 , 0)) < 0) {
      printf("Can\'t receive message from queue\n");
      exit(-1);
    }
    printf("Server receive message: type = %ld,pid = %ld, number = %f\n", mybuf.mtype,mybuf.values.pid, mybuf.values.mes);
    //send message
    mybuf.mtype = mybuf.values.pid;
    mybuf.values.pid = 1;
    mybuf.values.mes = mybuf.values.mes*mybuf.values.mes;
    len = sizeof(mybuf.values);
    //
    // Send the message. If there is an error,
    // report it and delete the message queue from the system.
    //
    if (msgsnd(msqid, (struct msgbuf *) &mybuf, len, 0) < 0) {
      printf("Can\'t send message to queue\n");
      msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
      exit(-1);
    }

  }



  return 0;
}
