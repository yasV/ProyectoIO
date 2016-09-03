#include <stdio.h>
#include <math.h>

int totalObjectsCount = 0;
FILE * fileTableData;
char buffer[9];
char fila[9];

typedef struct  {
   char name[100];
   char color[2];
   int value;	//cuento llevo acumulado de ganancia
   int cost;	//cuento me utiliza de espacio en la mochila
   int totalObjects;
} Objects;



void setTotalObjectsCount(int totalObjects) {
	totalObjectsCount = totalObjects;
}

void getFinalMatrix(Objects finalMatrix[][totalObjectsCount], int knapsackCapacit){
	char *color[2] = {"R","V"};
	char *nombres[3] = {"Agua","Calcetines","Galletas"};
	for (int i = 0; i < knapsackCapacit; ++i)
	{
		for (int j = 0; j < totalObjectsCount; ++j)
		{
			Objects object;
			int indexColor = rand() % 2;
			strcpy( object.name, nombres[j]);
			strcpy( object.color, color[indexColor]);
			object.value = rand() % 2;
			object.cost = rand() % 2;
			object.totalObjects = rand() % 2;
			finalMatrix[i][j] = object;
			printf("Se creo el objeto en la posicion [%d][%d]: nombre: %s, color: %s, valor: %d, costo: %d, cantidad: %d \n", 
				i,j, object.name, object.color, object.value, object.cost, object.totalObjects);
		}
	}

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

void startFill(int matrizD[][3],char *address,char **headers){
	fileTableData = fopen(address,"r");
	setMatriz(matrizD,headers);
}