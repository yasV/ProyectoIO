#include <stdio.h>
#include <math.h>


int infinito = 10000000;
int cantidadNodos = 0;


void mostrarMatrizIntermedia(int matrizD[][cantidadNodos]){
	for (int i=0;i<cantidadNodos;i++){
		printf("\n");
		for(int j=0;j<cantidadNodos;j++){
			printf("%d|",matrizD[i][j]);
		}
	}
	printf("----------------------------\n");
}


void mostrarMatrizP(int matrizP[][cantidadNodos]){
	for (int i=0;i<cantidadNodos;i++){
		printf("\n");
		for(int j=0;j<cantidadNodos;j++){
			printf("%d|",matrizP[i][j]);
		}
	}
}


void setCantidadNodos(int Nodos){
	cantidadNodos = Nodos;
	printf("%d\n",cantidadNodos );
}

void floyd(int matrizD[][cantidadNodos], int vuelta){
	printf("Entre en floyd\n");

	int matrizP[cantidadNodos][cantidadNodos];

	for (int i=0;i<cantidadNodos;i++){
		for (int j=0;j<cantidadNodos;j++){
			matrizP[i][j] = 0;
		}
	}

	printf("D\n");
	mostrarMatrizIntermedia(matrizD);
	printf("-----------------\n" );

	//for (int k=0;k<5;k++){
		for (int i=0;i<cantidadNodos;i++){
			for(int j=0;j<cantidadNodos;j++){
				int rutaAlterna = (matrizD[i][vuelta] + matrizD[vuelta][j]);
				if (matrizD[i][j] > rutaAlterna){
					matrizD[i][j] = rutaAlterna;
					matrizP[i][j] = vuelta+1;
				}
			}
		}
		//mostrarMatrizIntermedia(matrizD);
	//}

	mostrarMatrizP(matrizP);
	

}

/*
void main(){
	int matrizD[cantidadNodos][cantidadNodos];		
	
	matrizD[0][0] = 0;
	matrizD[0][1] = 6;
	matrizD[0][2] = infinito;
	matrizD[0][3] = 4;
	matrizD[0][4] = 7;

	matrizD[1][0] = 9;
	matrizD[1][1] = 0;
	matrizD[1][2] = 7;
	matrizD[1][3] = infinito;
	matrizD[1][4] = infinito;

	matrizD[2][0] = infinito;
	matrizD[2][1] = 5;
	matrizD[2][2] = 0;
	matrizD[2][3] = infinito;
	matrizD[2][4] = 14;

	matrizD[3][0] = 8;
	matrizD[3][1] = 1;
	matrizD[3][2] = infinito;
	matrizD[3][3] = 0;
	matrizD[3][4] = 15;

	matrizD[4][0] = 2;
	matrizD[4][1] = infinito;
	matrizD[4][2] = 2;
	matrizD[4][3] = 19;
	matrizD[4][4] = 0;

	floyd(matrizD);
}

*/



