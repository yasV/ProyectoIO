CC = gcc
GTKLIB = `pkg-config --cflags --libs gtk+-3.0`
WARN = -Wall -std=c99

all:
	$(CC) -o menu src/main.c $(WARN) $(GTKLIB) -export-dynamic
	$(CC) -o pending src/pending.c $(WARN) $(GTKLIB) -export-dynamic
	$(CC) -o floyd src/floyd.c $(WARN) $(GTKLIB) -export-dynamic -w
	$(CC) -o knapsack src/knapsack.c $(WARN) $(GTKLIB) -export-dynamic