all:
	gcc -o menu src/main.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic
	gcc -o pending src/pending.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic