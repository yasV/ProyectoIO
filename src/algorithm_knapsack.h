#include <stdio.h>
#include <math.h>

int totalObjectsCount = 0;
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

void setMatriz(int matrizD[totalObjectsCount-1][3],char ** headers) {
	int fila = 0;
	int columna = 0;
	int flag=0;
	int accion = escaner();

	while (fila < totalObjectsCount) {
	 	while (columna < 4) {
	 		if (accion == 1 && fila!=0 && flag==1) {
			 	int valor = atoi(buffer);
		 		matrizD[fila-1][columna-1] = valor;
		 	}
		 	if (flag == 0 && accion==1 && fila!=0) {
		 		char * data = malloc(1000);
	 			strcpy(data,buffer);
				headers[fila] = data;
		 		flag = 1;
		 	}
		 	columna ++;
		 	accion = escaner();	
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
			totalObjectsCount ++;
		}
	}

	
	fclose(fileTableData);
	return totalObjectsCount;
}

void startFill(int matrizD[][3],char *address,char ** headers){
	fileTableData = fopen(address,"r");
	setMatriz(matrizD,headers);
}