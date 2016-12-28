#include <ncurses.h>
#include <panel.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <linux/ipc.h>
#include <linux/msg.h>
#include <pthread.h>
#define MAX_SEND_SIZE 80
#define MAX_USER 10

typedef struct mymsgbuf {
  long mtype;
  int int_num;
  int name;
  char mess[MAX_SEND_SIZE];
} mess_t;

int main() {
  char message[MAX_SEND_SIZE];
  mess_t sent;
  mess_t received;
  key_t msgkey;
  int qid;
  msgkey = ftok(".",'m');
  qid = msgget(msgkey, IPC_CREAT | 0660);

  FILE *file;
  file = fopen("QID.txt","w");
  fprintf(file, "%d\n", qid);
  fprintf(file, "%d\n", 2);   
  fclose(file);

  int length;
  length = sizeof(mess_t) - sizeof(long);
  sent.mtype = 2;
  sent.int_num = 0;
  sent.name = getpid();

  bool exit = false;
  pid_t pid;
  if (!(pid = fork())) {
	while (exit != true) {      	
	  fflush(stdin);
	  fgets(message, MAX_SEND_SIZE, stdin);
	  strcpy(sent.mess, message);
	    for (int i = 3; i < MAX_USER; ++i) {
	      msgsnd(qid, &sent, length, 0);
	      sent.mtype = i;
	    }
	  if (strcmp(message, "exit") == 0)
	    exit = true;
	}
    return 0;
  }
  pid_t new_pid;
  int count_exit = 0;
  while (exit != true) {
  	msgrcv(qid, &received, length, 1, 0);
	printf("%d %s", received.name, received.mess);
	if (strcmp(received.mess, "exit") == 0) {
      count_exit++;
    }
    if (count_exit == MAX_USER) {
    	msgctl(qid, IPC_RMID, 0);
    	exit = true;
    }
  }
  //msgctl(qid, IPC_RMID, 0);
  return 0;
}
