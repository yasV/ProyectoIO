#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  char name[400];
  int valor;
  int costo;
  int cantidad;
}Objetos;

char *nombres[5]= {"Reloj","Anillo","Poster","Radio","Candelero"};
int valor[5] = {6,89,5,31,9};
int costo[5] = {2,4,5,6,7};
int cantidad[5] = {467,75,3,7,8};
Objetos todos[5];

int main() {

	for (int i = 0; i < 5; ++i)
	{
		Objetos objeto;
		objeto.valor = valor[i];
		objeto.costo = costo[i];
		objeto.cantidad = cantidad[i];
		strcpy(objeto.name, nombres[i]);
		todos[i] = objeto;
	}
	
	for (int i = 0; i < 5; ++i)
	{
		Objetos objeto = todos[i];
		printf("id) %d El objeto %s tiene un valor de %d , pero pesa %d tenemos %d unidades\n", i, objeto.name, 
		objeto.valor, objeto.costo, objeto.cantidad );
	}
}