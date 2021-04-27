#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

struct mymsgbuf // Custom structure for the message
{
  long mtype;
  struct {
    char  mtext[30];
    int   mnumber;
  } info;
} mybuf;

void send_message(int msqid, int number) {
  mybuf.mtype = 1;
  strcpy(mybuf.info.mtext, "Program A message");
  mybuf.info.mnumber = number;
  if (msgsnd(msqid, (struct msgbuf *) &mybuf, sizeof(mybuf.info), 0) < 0) {
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }
}

void send_last_message(int msqid) {
  mybuf.mtype = 1;
  strcpy(mybuf.info.mtext, "Last message from program A");
  mybuf.info.mnumber = 0;
  if (msgsnd(msqid, (struct msgbuf *) &mybuf, sizeof(mybuf.info), 0) < 0) {
    printf("Can\'t send message to queue\n");
    msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
    exit(-1);
  }
}

void receive_message(int msqid) {
  if (msgrcv(msqid, (struct msgbuf *) &mybuf, sizeof(mybuf.info), 2, 0) < 0) {
    printf("Can\'t receive message from queue\n");
    exit(-1);
  }
}

int main(void)
{
  int msqid;
  char pathname[] = "2a.c";
  key_t  key;
  int i;

  if ((key = ftok(pathname,0)) < 0) {
    printf("Can\'t generate key\n");
    exit(-1);
  }
  if ((msqid = msgget(key, 0666 | IPC_CREAT)) < 0) {
    printf("Can\'t get msqid\n");
    exit(-1);
  }

  int multiplier_a = 2;
  int current_value = 1;

  /* Send and receive information */

  for (i = 0; i < 5; i++) {
    send_message(msqid, current_value);

    receive_message(msqid);

    printf("Program A: type = %ld, text = %s, value = %d\n", mybuf.mtype, mybuf.info.mtext, mybuf.info.mnumber);
    
    current_value = mybuf.info.mnumber * multiplier_a;
  }

  /* Send the last message */

  send_last_message(msqid);

  return 0;
}
