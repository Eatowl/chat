#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <linux/ipc.h>
#include <linux/msg.h>
#define MAX_SEND_SIZE 80

/* Redefines the struct msgbuf */
typedef struct mymsgbuf
{
  long mtype;
  int int_num;
  int name;
  char mess[MAX_SEND_SIZE];
} mess_t;

int main()
{
  int qid;
  char message[MAX_SEND_SIZE];
  key_t msgkey;
  pid_t pid;

  mess_t sent;
  mess_t received;

  int length;
  FILE *file = fopen("QID.txt","r");
  fscanf(file, "%d", &qid);
  fclose(file);

  length = sizeof(mess_t) - sizeof(long);
  msgkey = ftok(".",'m');

  sent.mtype = 2;
  sent.int_num = 0;
  sent.name = getpid();

  bool exit = false;

  if(!(pid = fork())){
    while (exit != true) {
      fflush(stdin);
      fgets(message, MAX_SEND_SIZE, stdin);
      //scanw("%s", &message);
      //getstr(message);
      strcpy(sent.mess, message);
      msgsnd(qid, &sent, length, 0);
      sent.int_num++;
      if (strcmp(message, "exit") == 0)
        exit = true;
    }

    return 0;
  }

  while (exit != true) {
    msgrcv(qid, &received, length, 1, 0);
    printf("%d %s", received.name, received.mess);
  }

}