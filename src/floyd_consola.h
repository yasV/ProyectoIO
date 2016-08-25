#include <stdio.h>
#include <math.h>


int infinito = 10000000;
int cantidadNodos = 0;
char buffer[9];
char fila[9];
FILE * archivo;


/*All Floyd */

void setCantidadNodos(int Nodos) {
	cantidadNodos = Nodos;
}

void floyd(int matrizD[][cantidadNodos], int matrizP[][cantidadNodos], int vuelta) {
	//int matrizP[cantidadNodos][cantidadNodos];
	printf("%d\n", cantidadNodos);

	for (int i = 0; i < cantidadNodos; i++) {
		for(int j = 0; j < cantidadNodos; j++) {
			int rutaAlterna = (matrizD[i][vuelta] + matrizD[vuelta][j]);
			if (matrizD[i][j] > rutaAlterna) {
				matrizD[i][j] = rutaAlterna;
				matrizP[i][j] = vuelta+1;
			}
		}
	}

}


/*Read Files*/
void fillBuffer(int _val) {
	if (strlen(buffer) == 0) {
		char dato=(char)_val;
		char auxiliar[] = {(char)_val,'\0'};
		strcat(buffer,auxiliar);
	} else{
		char dato = (char)_val;
		char auxiliar[]={dato,'\0'};
		strcat(buffer,auxiliar);
	}
}

void cleanBuffer(){
	memset(buffer,'\0',strlen(buffer));
}

int escaner() {
	cleanBuffer();

	int ch;

	while (feof(archivo)==0) {   
		ch = fgetc(archivo);
		if (ch=='\n') {
			return 0;
		}
		if (ch==';') {
			return 1;
		}
		fillBuffer(ch);
	}
}

void setMatriz(int matrizD[][cantidadNodos-1]) {
	int fila = 0;
	int columna = 0;
	int flag=0;
	int accion = escaner();
	int var = 0;

	while (var < cantidadNodos) {
		if (accion == 1){
			var ++;
	 	}
		if (accion == 0){
			break;
	 	}
	 	accion = escaner();
	}

	while (fila < cantidadNodos-1) {
	 	while (columna < cantidadNodos-1) {
		 	accion = escaner();
		 	if (flag == 0) {
		 		flag=1;
		 		accion=escaner();
		 	}
		 	if (accion == 1) {
			 	int valor = atoi(buffer);
		 		printf("%d\n",valor);
		 		matrizD[fila][columna] = valor;
		 		columna ++;
		 	}	
	 	}
		columna = 0;
	 	fila ++;
	 	flag = 0;
	 	accion = escaner();
	}
}


int countNodesFiles(char * direccion){
	archivo = fopen(direccion,"r");
	int ch;

	while(feof(archivo) == 0) {  
		ch = fgetc(archivo);
		if (ch == '\n'){
			break;
		}
		if (ch ==';'){
			cantidadNodos ++;
		}
		}
	

	fclose(archivo);
	return cantidadNodos;
}

void startFill(int matrizD[][cantidadNodos-1],char *direccion){
	archivo = fopen(direccion,"r");
	setMatriz(matrizD);
}




