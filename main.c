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
#define MAX_USER 100

typedef struct mymsgbuf
{
  long mtype;
  int int_num;
  int name;
  char mess[MAX_SEND_SIZE];
} mess_t;

struct thread_arg {
    int qid;
    mess_t received;
    int length;
    int type_proc;
};

void create_interface(WINDOW **my_wins, PANEL  **my_panels);

void create_new_proc(int qid, mess_t received, int length, int left_proc);

void *thread_test(void * arg) {
	struct thread_arg targ = *(struct thread_arg *) arg;

	msgrcv(targ.qid, &targ.received, targ.length, targ.type_proc, 0);
	printf("%d %s", targ.received.name, targ.received.mess);
}

int main() {
	WINDOW *my_wins[3];
    PANEL  *my_panels[3];
	//create_interface(my_wins, my_panels);
	char message[MAX_SEND_SIZE];
  	mess_t sent;
  	mess_t received;
  	key_t msgkey;
  	int qid;
  	msgkey = ftok(".",'m');
  	qid = msgget(msgkey, IPC_CREAT | 0660);

	FILE *file;
	file = fopen("QID.txt","w");
	fprintf(file, "%d", qid);    
	fclose(file);

  	int length;
  	length = sizeof(mess_t) - sizeof(long);
  	sent.mtype = 1;
  	sent.int_num = 0;
  	sent.name = getpid();

  	pthread_t thread;
    int result;
    struct thread_arg targ;
    targ.qid = qid;
    targ.length = length;
    targ.type_proc = 2;

  	bool exit = false;
  	pid_t pid;
  	if (!(pid = fork())) {
		while (exit != true) {      	
	      fflush(stdin);
	      fgets(message, MAX_SEND_SIZE, stdin);
	      //getstr(message);
	      strcpy(sent.mess, message);
	      msgsnd(qid, &sent, length, 0);
	      //printf("on -- %s\n", sent.mess);
	      //mvwprintw(my_wins[0], 1, 1, "%s", sent.mess);
	      //sent.int_num++;
	      if (strcmp(message, "exit") == 0)
	        exit = true;
		}
    return 0;
    }

  pid_t new_pid;
  while (exit != true) {
  	//msgrcv(qid, &received, length, 2, 0);
	//printf("%d %s", received.name, received.mess);
	//msgrcv(qid, &received, length, 3, 0);
	//printf("%d %s", received.name, received.mess);
  	//for (int i = 2; i < 4; ++i) {
  		/*if (!(pid = fork())) {
		    msgrcv(qid, &received, length, 2, 0);
			printf("%d %s", received.name, received.mess);
		    return 0;
	    }
	    msgrcv(qid, &received, length, 3, 0);
		printf("%d %s", received.name, received.mess);*/
	//}
    //mvwprintw(my_wins[0], 1, 1, "%s", received.mess);
    //update_panels();
    //doupdate();

    //result = pthread_create(&thread, NULL, &thread_test, &targ);
	create_new_proc(qid, received, length, 5);

  }
    
    return 0;
}

void create_interface(WINDOW **my_wins, PANEL  **my_panels) {

    initscr();
    cbreak();
    //noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    if (!has_colors()) {
        endwin();
        printf("\nОшибка! Не поддерживаются цвета\n");
    }
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_GREEN, COLOR_RED);
    init_pair(3, COLOR_BLUE, COLOR_GREEN);

    int row, col;
    getmaxyx(stdscr, row, col);
    if (col % 2 != 0) {
        col -= 1;
    }
    
    my_wins[0] = newwin(row - 4, col - 40, 0, 0);
    wbkgdset(my_wins[0], COLOR_PAIR(1));
    wclear(my_wins[0]);

    my_wins[1] = newwin(row, col + 1, row - 4, 0);
    wbkgdset(my_wins[1], COLOR_PAIR(2));
    wclear(my_wins[1]);

    my_wins[2] = newwin(row - 4, col, 0, col - 40);
    wbkgdset(my_wins[2], COLOR_PAIR(3));
    wclear(my_wins[2]);

    for (int i = 0; i < 3; ++i)
        box(my_wins[i], 0, 0);

    for (int i = 0; i < 3; ++i)
        my_panels[i] = new_panel(my_wins[i]);

    update_panels();
    doupdate();
}

void create_new_proc(int qid, mess_t received, int length, int left_proc) {
	for (int i = 2; i < left_proc; i+=2) {
		pid_t new_pid;
		if (!(new_pid = fork())) {
		    msgrcv(qid, &received, length, i, 0);
			printf("%d %s", received.name, received.mess);
		    return 0;
		}
		msgrcv(qid, &received, length, i + 1, 0);
		printf("%d %s", received.name, received.mess);
	}
}