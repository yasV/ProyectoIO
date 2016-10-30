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
					otherFlag = 1;
	 			}
	 			else{
	 				float valor = atof(buffer);
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


void calculateProbabilities(){
	Objects key;
	float total=0;
	for (int i=0;i<inputNumberKeys;i++){
		key = KeyList[i];
		total = total + key.peso;
	}

	if (total == 1){
		for (int i=0;i<inputNumberKeys;i++){
			KeyList[i].probabilidad = KeyList[i].peso;
			
		}		
	}
	else{
		for (int i=0;i<inputNumberKeys;i++){
			KeyList[i].probabilidad = KeyList[i].peso / total;
		}
	}

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
  calculateProbabilities();


}

void treeBinaryOpticalAlgorithm(float table[inputNumberKeys+2][inputNumberKeys+1],int R[inputNumberKeys+2][inputNumberKeys+1]){

	int max_cols = inputNumberKeys +1;
	int max_rows = inputNumberKeys +2;

	for (int i=0;i<max_rows;i++){
		for (int j=0;j<max_cols;j++){
			table[i][j] = 0;
			R[i][j] = 0;
		}
	}

	

	float keyListTest[inputNumberKeys];


	for (int i =0;i<inputNumberKeys;i++){
		Objects key = KeyList[i];
		keyListTest[i] = key.probabilidad;
	}




	for (int row=1;row<max_rows;row++){
		for (int col=0;col<max_cols;col++){
			if (row==col && row!=0){
				table[row][col] = keyListTest[col-1] ;
				R[row][col] = col;
				table[row+1][col] = 0;
				for (int x=row-1;x>0;x--){
					table[x][col] = -1.000;
				}
			}

		}
	}
	for(int col=0;col<max_cols;col++){
		for (int row=max_rows-1;row>0;row--){
			if (table[row][col]==-1){
				float min = 100;
				int min_k=0;
				float sum = 0;
				for(int proba=row-1;proba<col;proba++){
					sum = sum + keyListTest[proba];
					}
				for (int k=row;k<=col;k++){

					float actualValue = table[row][k-1] + table[k+1][col] + sum;
					if (min > actualValue){
						min = actualValue;
						min_k = k;
						
					}
			}
				table[row][col] = min;
				R[row][col] = min_k;
			}

		}
	}
	

	


}

