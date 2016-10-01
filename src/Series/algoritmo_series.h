#include <stdio.h>
#include <math.h>


char buffer[9];

int inputNumberGames = 0;
float ph = 0.0;
float pr = 0.0;
float qh = 0.0;
float qr = 0.0;
int *formatSerie;

FILE * fileTableData;


int getTotalLines(char * address) {
	fileTableData = fopen(address,"r");
	int ch;
	int total = 0;

	while(feof(fileTableData) == 0) {  
		ch = fgetc(fileTableData);
		if (ch == '\n'){
			total ++;
		}
	}

	fclose(fileTableData);
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

	while (feof(fileTableData)==0) {   
		ch = fgetc(fileTableData);
		if (ch=='\n') {
			return 0;
		}
		if (ch==';') {
			return 1;
		}
		if (ch=='%') {
			return 2;
		}
		if (ch=='^') {
			return 3;
		}
		if (ch=='#') {
			return 4;
		}
		fillBuffer(ch);
	}
	return 0;
}

void setData(char *address) {
	int totalLines = getTotalLines(address);

	fileTableData = fopen(address,"r");
	inputNumberGames = 0;
	ph = 0.0;
	pr = 0.0;
	int flag=0;
	int row = 0;
	int action = scanner();

	while (row < ) {
		printf("ActionL %d\n", action );
	 		if (flag==1) {
			 	int value = atoi(buffer);
		 		printf("Numero de juegos:%d\n", value);
		 	}
		 	if (flag == 0 && action==4) {
		 		flag = 1;
		 	}
		 	action = scanner();	
	 	
	 	row ++;
	 	flag = 0;
	 	action = scanner();
	}
	
}