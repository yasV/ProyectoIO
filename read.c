#include <string.h>
#include <stdio.h>
#include <stdlib.h>


FILE * fPtr;
int nodos;
char buffer[9];
char fila[9];

void fillBuffer(int _val){
	if (strlen(buffer)==0){
		char dato=(char)_val;
		char auxiliar[]={(char)_val,'\0'};
		strcat(buffer,auxiliar);
	}
	else{
		char dato=(char)_val;
		char auxiliar[]={dato,'\0'};
		strcat(buffer,auxiliar);
}
}

void cleanBuffer(){
	memset(buffer,'\0',strlen(buffer));
}

int escaner(){
	cleanBuffer();

	int ch;

	while(feof(fPtr)==0)
	{   
		ch = fgetc(fPtr);
		if (ch=='\n'){
			return 0;
		}
		if (ch==';'){
			return 1;
		}
		
		fillBuffer(ch);
		 
	 }

}


void test(int matrizD[][nodos-1]){
	 	int fila = 0;
	 int columna = 0;
	 int flag=0;

	 		int accion = escaner();
	 		int var = 0;
	 		while (var<nodos){
	 				 		
	 			if (accion==1){
	 				 	var ++;
	 			}
	 			if (accion==0){
	 				break;
	 			}
	 			accion=escaner();

	 		}

	 		while (fila<nodos-1){
	 			while (columna<nodos-1){
		 			 	accion=escaner();

		 		if (flag==0){
		 			flag=1;
		 			accion=escaner();
		 		}
		 		if (accion==1){
			 		int valor = atoi(buffer);
		 			printf("%d\n",valor);
		 			matrizD[fila][columna] = valor;
		 			columna ++;
		 		}	
	 		}
			columna =0;
	 		fila ++;
	 		flag=0;
	 		accion =escaner();
	 		
	 	}
	 }


	  
void main (){
	fPtr = fopen("MatrizD.cvs","r");
	char enunciado[nodos];
	int ch;
	while(feof(fPtr)==0)
	{   ch = fgetc(fPtr);
		if (ch=='\n'){
			break;
		}
		if (ch==';'){
			nodos ++;
		}
		char dato =(char)ch;
		 
	 }
	 fclose(fPtr);
	 fPtr = fopen("MatrizD.cvs","r");
	 int matrizD[nodos-1][nodos-1];
	 for (int i=0;i<nodos-1;i++){
	 	for (int j=0;j<nodos-1;j++){
	 		matrizD[i][j] = 0;
	 	}
	 }


	
	

	 test(matrizD);
	 }