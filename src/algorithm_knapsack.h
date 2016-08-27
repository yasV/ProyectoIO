#include <stdio.h>
#include <math.h>

int totalObjects = 0;
FILE * fileTableData;
char buffer[9];
char fila[9];



/*Read Files*/
void fillBuffer(int _val) {
	if (strlen(buffer) == 0) {
		char dato= (char)_val;
		char auxiliar[] = {dato,'\0'};
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

	while (feof(fileTableData)==0) {   
		ch = fgetc(fileTableData);
		if (ch=='\n') {
			return 0;
		}
		if (ch==';') {
			return 1;
		}
		fillBuffer(ch);
	}
}

void setMatriz(int matrizD[][4]) {
	int fila = 0;
	int columna = 0;
	int flag=0;
	int accion = escaner();

	while (fila < totalObjects-1) {
	 	while (columna < 4) {
		 	accion = escaner();
		 	if (flag == 0) {
		 		flag=1;
		 		accion=escaner();
		 	}
		 	if (accion == 1) {
			 	int valor = atoi(buffer);
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

int countObjectsFiles(char * address){
	fileTableData = fopen(address,"r");
	int ch;

	while(feof(fileTableData) == 0) {  
		ch = fgetc(fileTableData);
		if (ch == '\n'){
			totalObjects ++;
		}
	}
	
	fclose(fileTableData);
	return totalObjects;
}

void startFill(int matrizD[][4],char *address){
	fileTableData = fopen(address,"r");
	setMatriz(matrizD);
}