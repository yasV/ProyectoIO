#include <stdio.h>
#include <math.h>

int totalUsefulLife = 0;
int initialCost = 0;
int timeLimit = 0;

char buffer[9];
char row[9];

FILE * fileTableData;

typedef struct  {
  int proximo[31];
  int value;	
  int min_year;
} FinalTable;

typedef struct  {
  int year;	
  int sale;
  int maintenance;
  int profit;
} InitialTable;

void setTotalObjectsCount(int pTotalUsefulLife,int pInitialCost, int pTimeLimit) {
	totalUsefulLife = pTotalUsefulLife;
	initialCost = pInitialCost;
	timeLimit = pTimeLimit;
}	

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
		fillBuffer(ch);
	}
	return 0;
}

void setMatriz(int matrizD[totalUsefulLife-1][3]) {
	int row = 0;
	int column = 0;
	int flag=0;
	int action = scanner();

	while (row < totalUsefulLife) {
	 	while (column < 4) {
	 		if (action == 1 && row!=0 && flag==1) {
			 	int value = atoi(buffer);
		 		matrizD[row-1][column-1] = value;
		 	}
		 	if (flag == 0 && action==1 && row!=0) {
		 		flag = 1;
		 	}
		 	column ++;
		 	action = scanner();	
	 	}
		column = 0;
	 	row ++;
	 	flag = 0;
	 	action = scanner();
	}
}

int countUsefulLifeFiles(char * address) {
	fileTableData = fopen(address,"r");
	int ch;

	while(feof(fileTableData) == 0) {  
		ch = fgetc(fileTableData);
		if (ch == '\n'){
			totalUsefulLife ++;
		}
	}

	fclose(fileTableData);
	return totalUsefulLife;
}

void startFill(int matrizD[][3],char *address) {
	fileTableData = fopen(address,"r");
	setMatriz(matrizD);
}

void replaceAlgorithm(InitialTable initialData[totalUsefulLife], FinalTable finalData[timeLimit+1]) {

	int cost[totalUsefulLife];
	int mantain = 0;
	
	for (int year = 0; year < totalUsefulLife; year++) {
		InitialTable matrizYear = initialData[year];
		for (int rango=0; rango <=year; rango++) {
			InitialTable matrizRango = initialData[rango];
			mantain = mantain + matrizRango.maintenance;
		}
		cost[year] = initialCost + mantain - matrizYear.sale;
		mantain = 0;
	}

	for (int i = timeLimit; i>=0 ;i--) {
		int posible = 0;
		int flag = 0;
		FinalTable object;
		object.value = 0;
		object.min_year = 0;

		for (int x = i+1; x <= timeLimit; x++) {
			FinalTable lastObject = finalData[x];
			if (flag < totalUsefulLife) {
				flag ++;
				if (object.value == 0){
					object.value = cost[(x-i)-1] + lastObject.value;
					object.min_year = x;
				} 
				else {
					posible = cost[(x-i)-1] + lastObject.value;
					if (posible<object.value){
						object.value = posible;
						object.min_year = x;
					}
				}
			}
		}
		finalData[i] = object;
	}
}
