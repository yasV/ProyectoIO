#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


char buffer[9];

int inputMatrixNumber = 0;
FILE * fileTableData;

typedef struct  {
   int rows;
   int columns;
} MatrixObject;

int *dValueArray;

FILE * fileData;


int getTotalCharacter(char *address) {
	fileData = fopen(address,"r");
	int total = 0;

	int c;
	if (fileData) {
	    while ((c = getc(fileData)) != EOF)
	        total++;
	    fclose(fileData);
	}
	return total;
}

int getMatrixNumber(char *address) {
	fileData = fopen(address,"r");
	int total = 0;

	int c;
	if (fileData) {
	    while ((c = getc(fileData)) != EOF) {
	    	if( c == ';'){
	    		total++;
	    	}
	    }
	    fclose(fileData);
	}
	total = total - 1;
	return total;
}

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

void cleanBuffer() {
	memset(buffer,'\0',strlen(buffer));
}

int scanner() {
	cleanBuffer();

	int ch;

	while (feof(fileData)==0) {   
		ch = fgetc(fileData);
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

void setData(char *address) {
	int totalCh = getTotalCharacter(address);
	fileData = fopen(address,"r");
	int row = 0;
	int formatPosition = 0;
	int action = scanner();
	dValueArray = calloc(inputMatrixNumber + 1, sizeof(int));

	while (row < totalCh) {
		if (action == 1) {
			int value = atoi(buffer);
			dValueArray[formatPosition] = value;
			formatPosition++;
		}
	 	row ++;
	 	action = scanner();
	}

	fclose(fileData);
}

void startAlgorithm(){
	inputMatrixNumber = getMatrixNumber("ejemplo1.cvs");
	setData("ejemplo1.cvs");
	
}


void optimalMatrix(int matrix[inputMatrixNumber+1][inputMatrixNumber+1],int p[inputMatrixNumber+1][inputMatrixNumber+1]){
for (int i=1;i<inputMatrixNumber+1;i++){
			for (int j=1;j<inputMatrixNumber+1;j++){
				matrix[i][j] = 0;
				p[i][j] = 0;
			}
	}


	for (int i=1;i<inputMatrixNumber+1;i++){
		for (int j=1;j<inputMatrixNumber+1;j++){
			if (j==i+1){
				int value = dValueArray[i-1] * dValueArray[i] * dValueArray [j];
				for (int x=j-1;x>0;x--){
					matrix[x][j]= -1;

				}
				p[i][j] = j-1;
				matrix[i][j] = value ;

			}
		}
	}

	for(int col=1;col<inputMatrixNumber+1;col++){
		for (int row=inputMatrixNumber;row>0;row--){
			if (matrix[row][col]==-1){
				int min = 0;
				int k_p = 0;
				for (int k=row;k<=col-1;k++){
					int actualValue = matrix[row][k]+matrix[k+1][col]+(dValueArray[row-1]*dValueArray[col]*dValueArray[k]);
					if (min == 0){
						min = actualValue;
						k_p = k;
					}
					if (min > actualValue){
						min = actualValue;
						k_p = k;
							
					}
			}
				matrix[row][col] = min;
				p[row][col] = k_p;
				
			}

		}
	}



	
	



}

