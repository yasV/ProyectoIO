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
