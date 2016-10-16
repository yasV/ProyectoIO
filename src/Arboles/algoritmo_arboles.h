#include <stdio.h>
#include <math.h>


char buffer[9];

int inputNumberKeys = 0;
FILE * fileTableData;

typedef struct  {
   char texto[40];
   int peso;
   int probabilidad;
} Objects;

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
	return 0;
}

void setMatriz(Objects matrizD[inputNumberKeys]) {
	int fila = 0;
	int columna = 0;
	int otherFlag=0;
	int accion = escaner();

	while (fila < inputNumberKeys + 1) {

		Objects key;
		strcpy(key.texto, " ");
		key.peso = 0;
		key.probabilidad = 0;

	 	while (columna < 2) {
	 		if (accion == 1 && fila!=0) {
	 			if(otherFlag == 0){
	 				char * data = malloc(1000);
		 			strcpy(data,buffer);
					strcpy(key.texto,data);
					otherFlag = 1;
	 			}
	 			else{
	 				int valor = atoi(buffer);
			 		key.peso = valor;
			 		otherFlag = 0;
	 			}
		 		matrizD[fila-1] = key;
		 	}
		 	columna ++;
		 	accion = escaner();	
	 	}
		columna = 0;
	 	fila ++;
	 	accion = escaner();
	}
}

int countRowsFile(char * address){
	fileTableData = fopen(address,"r");
	int ch;

	while(feof(fileTableData) == 0) {  
		ch = fgetc(fileTableData);
		if (ch == '\n'){
			inputNumberKeys ++;
		}
	}

	fclose(fileTableData);
	return inputNumberKeys - 1;
}

void startFill(Objects matrizD[inputNumberKeys],char *address){
	fileTableData = fopen(address,"r");
	setMatriz(matrizD);
}