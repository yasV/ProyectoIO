#include <stdio.h>
#include <math.h>

int totalUsefulLife = 0;
int initialCost = 0;
int timeLimit = 0;

char buffer[9];
char row[9];

int contado_x;

FILE * fileTableData;

typedef struct  {
  int year[31];
  int value;	
  int position;
  int profit;
} FinalTable;


typedef struct {
	int value;
	int year;
} candidate;

typedef struct  {
  int year;	
  int sale;
  int maintenance;
} InitialTable;

typedef struct 
{
	int year[30];
	int position;
} plans;

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

void setMatriz(int matrizD[totalUsefulLife-1][2]) {
	int row = 0;
	int column = 0;
	int flag=0;
	int action = scanner();

	while (row < totalUsefulLife) {
	 	while (column < 3) {
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

void startFill(int matrizD[][2],char *address) {
	fileTableData = fopen(address,"r");
	setMatriz(matrizD);
}


void mostrar_respuesta(FinalTable finalData[timeLimit+1],plans planesPosibles[300],int cont_plans){
	int stop = 0;
	int contador = 0;
	 
	plans planes;
	planes.position = 0;
	planes.year[0] = 0;

	FinalTable answer = finalData[contador];
	while(stop==0){ 	
	 	if (answer.position==0){
	 		stop = 1;
	 	}
	 	else{
	 		planes.position = planes.position +1;
	 		planes.year[planes.position] = answer.year[answer.position-1]; 
	 		
	 		answer = finalData[answer.year[answer.position-1]];
	 	}
	}
	planesPosibles[contado_x] = planes;
	cont_plans ++;
	contado_x ++;
}

int returnContador (){
	return contado_x;
}

void planes(FinalTable G[timeLimit+1],plans planesPosibles[300],int cont_plans){
	for (int i=0;i<=timeLimit;i++){
		if (G[i].position > 1){
			for (int x = G[i].position;x>=1;x--){
				mostrar_respuesta(G,planesPosibles, cont_plans);
				G[i].position = G[i].position - 1;

				if (G[i].position == 0){
				 	G[i].position = 1;
				}
			
			}
		}
	}
	cont_plans = contado_x;
}


int repeat (plans planesPosibles[300],int indice){
	int result = 0;
	int coincidencia = 0;
	if (indice > 0){
		for (int i=0;i<indice;i++){
			if (planesPosibles[i].position == planesPosibles[indice].position){
				for (int x =0;x<planesPosibles[indice].position;x++){
					if (planesPosibles[indice].year[x] == planesPosibles[i].year[x]){
						result = 1;
						coincidencia ++;
					}
					else{
						result = 0;
						coincidencia = 0;
					}
				}
			}
		}
	}

	if (result == 1 && coincidencia==planesPosibles[indice].position+1){
		return 1;
	}
	else{
		return 0;
	}
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

	for (int i = timeLimit;i>=0;i--){
		int min = 0;
		int flag = 0;
		candidate options[totalUsefulLife];
		candidate posibilites;
		posibilites.value = 0;
		posibilites.year = i;
		FinalTable answer;
		for (int x = i+1;x<=timeLimit;x++){
			if (flag < totalUsefulLife){
				posibilites.value = cost[(x-i)-1] + finalData[x].value;
				posibilites.year = x;
				options[flag] = posibilites;
				flag ++;
			}
		}


		if (flag == 0){
			answer.value = posibilites.value;
			answer.year[0] = posibilites.year;
			answer.position = 0;
			answer.profit = initialCost;
		}
		for (int i = 0;i<flag;i++){
			candidate aux;
			aux = options[i];
			if (i==0){
				min = aux.value;
				answer.year[0] = aux.year;
				answer.position = 1;
			}
			else{
				if (aux.value == min){
					answer.year[answer.position] = aux.year;
					answer.position ++;
				}

				if (aux.value<min){
					answer.year[0] = aux.year;
					answer.position = 1;
					min = aux.value;
				}

			}
			answer.value = min;
			answer.profit = initialCost - min;
		}
		finalData[i] = answer;
	}
}
