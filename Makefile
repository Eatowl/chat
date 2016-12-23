all:
	gcc -o server main.c -lpanel -lncurses
	gcc -o client sendProc.c