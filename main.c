#include <ncurses.h>
#include <panel.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main() {

    WINDOW *my_wins[3];
    PANEL  *my_panels[3];

    initscr();
    cbreak();
    noecho();
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
    // инициализируются окна
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
    update_panels();
    doupdate();

    for (int i = 0; i < 3; ++i)
        my_panels[i] = new_panel(my_wins[i]);

    pid_t pid;
    pid_t test;

    test = getpid();

  	pid = fork();
  	if (pid == 0) {
	  	for (int i = 0; i < 5; ++i) {
	  		mvwprintw(my_wins[2], 1, 1, "list process \n pid %d\n", test);
	  	}
	  	return 0;
	}

	for (int i = 0; i < 5; ++i) {
		mvwprintw(my_wins[0], 1, 1, "window new pid\n");
	}
    
    //mvwprintw(my_wins[0], 1, 1, "window chat message");
    mvwprintw(my_wins[1], 1, 1, "window new message");
    //mvwprintw(my_wins[2], 1, 1, "list process \n pid %d", pid);

    update_panels();
    doupdate();

    int ch = getch(), type_f = 1;
        pid_t result;

    switch ( ch ) {
        case KEY_F(2):
            for (int i = 0; i < 3; ++i) {
                del_panel(my_panels[i]);
                delwin(my_wins[i]);
            }
            endwin();
            break;
        case '\n':
        	update_panels();
    		doupdate();
    		break;
    }
    return 0;
}
