#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


char buffer[9];

int inputNumberKeys = 0;
FILE * fileTableData;

typedef struct  {
   char texto[40];
   float peso;
   float probabilidad;
} Objects;

Objects *KeyList;

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

	 	while (columna < 2) {
	 		if (accion == 1 && fila!=0) {
	 			if(otherFlag == 0){
	 				char * data = malloc(1000);
		 			strcpy(data,buffer);
					strcpy(key.texto,data);
					printf("%s\n", data );
					otherFlag = 1;
	 			}
	 			else{
	 				printf("Lei el numero: %s\n", buffer );
	 				float valor = atof(buffer);
	 				printf("Conversion: %f\n", valor );
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
	fclose(fileTableData);
}

void alphabeticalOrder() {
	Objects temp;

	for(int i=0; i < inputNumberKeys; i++) {
		for(int j=i+1; j<inputNumberKeys; j++) {
			Objects keyList1 = KeyList[i];
			Objects keyList2 = KeyList[j];
			if(strcmp(keyList1.texto, keyList2.texto) > 0) {
				temp = keyList1;
				KeyList[i] = keyList2;
				KeyList[j] = temp;
			}
		}
  }

  /*for (int i = 0; i < inputNumberKeys; ++i)
  {
  	Objects key = KeyList[i];
  	printf("Llave[%d]: texto: %s peso: %f proba: %f\n", i, key.texto, key.peso, key.probabilidad);
  }*/
}