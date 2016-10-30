CC = gcc
GTKLIB = `pkg-config --cflags --libs gtk+-3.0`
WARN = -Wall 

all:
	$(CC) -o menu src/main.c $(WARN) $(GTKLIB) -export-dynamic
	$(CC) -o pending src/pending.c $(WARN) $(GTKLIB) -export-dynamic
	#$(CC) -o floyd src/Floyd/floyd.c $(WARN) $(GTKLIB) -export-dynamic -w
	#$(CC) -o knapsack src/Knapsack/knapsack.c $(WARN) $(GTKLIB) -export-dynamic -w
	#$(CC) -o reemplazoEquipos src/ReemplazoEquipos/reemplazoEquipos.c $(WARN) $(GTKLIB) -export-dynamic 
	#$(CC) -o seriesDeportivas src/Series/series.c $(WARN) $(GTKLIB) -export-dynamic
	$(CC) -o arbolBinario src/Arboles/arboles.c $(WARN) $(GTKLIB) -export-dynamic