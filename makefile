all:
	gcc -o menu src/main.c -Wall `pkg-config --cflags --libs gtk+-3.0` -export-dynamic