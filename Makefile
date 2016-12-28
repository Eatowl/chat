all:
	gcc -o server main.c -lpanel -lncurses -lpthread
	gcc -o client sendProc.c