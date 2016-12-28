#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <linux/ipc.h>
#include <linux/msg.h>
#define MAX_SEND_SIZE 80
#define MAX_USER 10

/* Redefines the struct msgbuf */
typedef struct mymsgbuf
{
  long mtype;
  int int_num;
  int name;
  char mess[MAX_SEND_SIZE];
} mess_t;

void create_new_proc(int qid, mess_t received, int length, int left_proc, int n);

int main()
{
  int qid;
  char message[MAX_SEND_SIZE];
  key_t msgkey;
  pid_t pid;

  mess_t sent;
  mess_t received;

  int length;
  int n[1], i = 0;
  FILE *file = fopen("QID.txt","r");
  while( !feof(file) ) {
    fscanf( file, "%d", &n[i] );
    //printf("rez scan %d\n", n[i]);
    i++;
  }
  fclose(file);
  FILE *file1 = fopen("QID.txt","w");
  fprintf(file1, "%d\n", n[0]);
  fprintf(file1, "%d\n", (n[1] + 1));
  fclose(file1);

  length = sizeof(mess_t) - sizeof(long);
  msgkey = ftok(".",'m');

  sent.mtype = n[1];
  sent.int_num = 0;
  sent.name = getpid();

  printf("my type message %d\n", sent.mtype);

  bool exit = false;

  if(!(pid = fork())){
    while (exit != true) {
      fflush(stdin);
      fgets(message, MAX_SEND_SIZE, stdin);
      //scanw("%s", &message);
      //getstr(message);
      strcpy(sent.mess, message);
      for (int i = 1; i < MAX_USER; ++i) {
        if (i != n[1]) {
          msgsnd(qid, &sent, length, 0);
          sent.mtype = i;
        }
      }
      sent.int_num++;
      if (strcmp(message, "exit") == 0)
        exit = true;
    }

    return 0;
  }

  while (exit != true) {
    msgrcv(qid, &received, length, n[1], 0);
    printf("%d %s", received.name, received.mess);
    if (strcmp(received.mess, "stop") == 0) {
      printf("!!!!!!!!!!");
    }
    //create_new_proc(qid, received, length, 5, n[i]);
  }

}

void create_new_proc(int qid, mess_t received, int length, int left_proc, int n) {
  for (int i = 1; i < left_proc; i+=2) {
    pid_t new_pid;
    if (!(new_pid = fork()) && i != n) {
        msgrcv(qid, &received, length, i, 0);
      printf("%d %s", received.name, received.mess);
        return 0;
    }
    if ( n + 1 != i) {
      msgrcv(qid, &received, length, i + 1, 0);
      printf("%d %s", received.name, received.mess);
    }
  }
}