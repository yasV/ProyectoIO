#include <stdio.h>
#include <math.h>

int totalObjectsCount = 0;
int Capacity = 0;
FILE * fileTableData;
char buffer[9];
char fila[9];

typedef struct  {
   char color[2];
   int value;	//cuento llevo acumulado de ganancia
   int cost;	//cuento me utiliza de espacio en la mochila
   int totalObjects;
} Objects;



void setTotalObjectsCount(int totalObjects,int knapsackCapacit) {
	totalObjectsCount = totalObjects;
	Capacity = knapsackCapacit;
}	


void getFinalMatrix(Objects finalMatrix[][totalObjectsCount], int knapsackCapacit){
	char *color[2] = {"R","V"};
	for (int i = 0; i < knapsackCapacit; ++i)
	{
		for (int j = 0; j < totalObjectsCount; ++j)
		{
			Objects object;
			int indexColor = rand() % 2;
			strcpy( object.color, color[indexColor]);
			object.value = rand() % 2;
			object.cost = rand() % 2;
			object.totalObjects = rand() % 2;
			finalMatrix[i][j] = object;
			printf("Se creo el objeto en la posicion [%d][%d]: color: %s, valor: %d, costo: %d, cantidad: %d \n", 
				i,j,  object.color, object.value, object.cost, object.totalObjects);
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
	return 0;
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

void knapsackAlgorithm(Objects matrizObjects[Capacity][totalObjectsCount],Objects todos[totalObjectsCount]){
	int matriz[Capacity][totalObjectsCount];
	int con = 0;

	Objects zero;
	zero.cost = 0;
	zero.value = 0;
	zero.totalObjects =0;

	for (int i=0;i<Capacity;i++){
		for (int j=0;j<totalObjectsCount;j++){
			matriz[i][j] =0;
		}
	}

	for (int i=0; i < Capacity; i++) {
		for (int j=0; j< totalObjectsCount; j++) {
			matrizObjects[i][j] = zero;
		}
	}
	
	for (int j=0; j<totalObjectsCount; j++){
		for (int i=0;i<Capacity;i++){
			Objects actual = todos[j];
			Objects guardar;
			
			if (j==0) {
				if (actual.cost<=i) {
					strcpy (guardar.color,"V");
					int aux = con +1;
					if (aux * actual.cost == i && i!=0 && con <= actual.cost){
						con ++;
					}	
				}
				else{ 
					strcpy (guardar.color,"R");
				}

				guardar.value = actual.value * con;
				guardar.totalObjects = con;
					
				matriz[i][j] = con * actual.value;
				matrizObjects[i][j] = guardar;
			}

			else {
				if (actual.cost > i) {
					matriz[i][j] = matriz[i][j-1];
					guardar = matrizObjects[i][j-1];
					guardar.totalObjects = 0;
					strcpy (guardar.color,"R");
					matrizObjects[i][j] =guardar;
				}
				else {
					strcpy (guardar.color,"R");		
					int optimo = matriz[i][j-1];
					int candidato = actual.value + matriz[i-actual.cost][j-1];
					int cuenta_candidato = 1;
					int cuenta_optimo = 0;  

					for ( int contador = 1; contador <= actual.totalObjects; contador++) {
						if (actual.cost * contador <= i) {
							if (candidato < (contador * actual.value + matriz[i-(contador*actual.cost)][j-1]) ) {
								candidato = contador * actual.value + matriz[i-(contador*actual.cost)][j-1];
								cuenta_candidato = contador;
							}
						}
						else {
							break;
						}
					}

					if (optimo < candidato) {
						optimo = candidato;
						cuenta_optimo = cuenta_candidato;
						strcpy (guardar.color,"V");
					}					
						
					guardar.value = optimo;
					guardar.totalObjects = cuenta_optimo;
					matriz[i][j] = optimo;
					matrizObjects[i][j] = guardar;
				}
			}
		}
	}
}

void getOptimalSolution(int *optimalSolution, Objects matrizObjects[Capacity][totalObjectsCount]) {
	printf("entro en getOptimalSolution\n");
	printf("getOptimalSolution objetos: %d , capacidad: %d \n",totalObjectsCount, Capacity);
	for (int i = 0; i < Capacity; ++i)
	{
		for (int j = 0; j < totalObjectsCount; ++j)
		{
			Objects object = matrizObjects[i][j];
			printf("Se creo el objeto en la posicion [%d][%d]: color: %s, valor: %d, costo: %d, cantidad: %d \n", 
				i,j,  object.color, object.value, object.cost, object.totalObjects);
		}
	}

	int row = Capacity-1;
	int column = totalObjectsCount-1;

	Objects object = matrizObjects[row][column];
	optimalSolution[0] = object.value; //Solucion optima

	for (int i = totalObjectsCount; i > 0; --i)
	{	
		printf("Se obtuvo el objeto en la posicion [%d][%d]: color: %s, valor: %d, costo: %d, cantidad: %d \n", 
				row,column,  object.color, object.value, object.cost, object.totalObjects);
		
		optimalSolution[i] = object.totalObjects;

		if (strcmp(object.color, "R") == 0) {
			column--;
		}
		else {
			column--;
			row = row - object.cost;
		}
		object = matrizObjects[row][column];
	}

	for (int j = 0; j < totalObjectsCount+1; ++j)
		{
			printf("Se creo el objeto en la posicion [%d]: valor: %d \n", 
				j, optimalSolution[j] );
		}
}