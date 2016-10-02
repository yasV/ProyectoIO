#include <stdio.h>
#include <math.h>


char buffer[9];

int inputNumberGames = 0;
float ph = 0.0;
float pr = 0.0;
float qh = 0.0;
float qr = 0.0;
int *formatSerie;

FILE * fileData;


int getTotalCharacter(char * address) {
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
		if (ch=='%') {
			return 1;
		}
		if (ch=='^') {
			return 2;
		}
		if (ch=='#') {
			return 3;
		}
		fillBuffer(ch);
	}
	return 0;
}

void setData(char *address) {
	int totalCh = getTotalCharacter(address) - 1;
	fileData = fopen(address,"r");
	
	int row = 0;
	int flag=0;
	int formatPosition = 0;
	int action = scanner();

	while (row < totalCh) {
		if (action == 3) {
			action = scanner();
			row ++;
			if (action == 0) {
				action =  scanner();
				row ++;
				inputNumberGames = atoi(buffer);
				formatSerie = calloc(inputNumberGames, sizeof(int));
			}
		}
		if (action == 1) {
			action = scanner();
			row ++;
			if (action == 0) {
				action =  scanner();
				row ++;
				float value = atof(buffer);
				if (flag == 0) {
					ph = value;
					flag = 1;
				}
				else{
					pr = value;
				}
			}
		}
		if (action == 2) {
			int value = atoi(buffer);
			formatSerie[formatPosition] = value;
			formatPosition++;
		}
	 	
	 	row ++;
	 	action = scanner();
	}
}