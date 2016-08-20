#include <stdio.h>
#include <math.h>

int matrizD[5][5];
int matrizTemporal[5][5];
int matrizP[5][5];
int infinito=10000000;

void crearMatrices(){
	for (int i=0;i<5;i++){
		for (int j=0;i<5;i++){
			matrizTemporal[i][j] = 0;
			matrizP[i][j] = 0;
		}
	}

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
}

void mostrarMatrizIntermedia(){
	for (int i=0;i<5;i++){
		printf("\n");
		for(int j=0;j<5;j++){
			printf("%d|",matrizD[i][j]);
		}
	}
	printf("----------------------------\n");
}

void mostrarMatrizP(){
	for (int i=0;i<5;i++){
		printf("\n");
		for(int j=0;j<5;j++){
			printf("%d|",matrizP[i][j]);
		}
	}
}
void main(){
	crearMatrices();
	int k=0;
	for (k;k<5;k++){
		for (int i=0;i<5;i++){
			for(int j=0;j<5;j++){
				int rutaAlterna = (matrizD[i][k] + matrizD[k][j]);
				if (matrizD[i][j] > rutaAlterna){
					matrizD[i][j] = rutaAlterna;
					matrizP[i][j] = k+1;
				}
			}
		}
		//mostrarMatrizIntermedia();
	}
	mostrarMatrizP();
}

