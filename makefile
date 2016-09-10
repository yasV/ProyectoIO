CC = gcc
GTKLIB = `pkg-config --cflags --libs gtk+-3.0`
WARN = -Wall 

all:
	$(CC) -o menu src/main.c $(WARN) $(GTKLIB) -export-dynamic
	$(CC) -o pending src/pending.c $(WARN) $(GTKLIB) -export-dynamic
	$(CC) -o floyd src/floyd/floyd.c $(WARN) $(GTKLIB) -export-dynamic -w
	$(CC) -o knapsack src/knapsack/knapsack.c $(WARN) $(GTKLIB) -export-dynamic -w
	$(CC) -o reemplazoEquipos src/reemplazoEquipos/reemplazoEquipos.c $(WARN) $(GTKLIB) -export-dynamic