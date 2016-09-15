#include <stdio.h>

int matriz[3][3];
int life = 3;
int buyCost = 500;
int time = 5;

//quiz
/*int matriz[4][3];
int life = 4;
int buyCost = 9999;
int time = 8;*/

typedef struct  {
  int proximo[31];
  int value;	
  int min_year;
} Objects;


void main(int argc, char const *argv[])
{
	
	//Visto en Clase
	matriz[0][0] = 1;
	matriz[0][1] = 30;
	matriz[0][2] = 400;

	matriz[1][0] = 2;
	matriz[1][1] = 40;
	matriz[1][2] = 300;

	matriz[2][0] = 3;
	matriz[2][1] = 60;
	matriz[2][2] = 250;

	//Quiz
	/*matriz[0][0] = 1;
	matriz[0][1] = 305;
	matriz[0][2] = 7125;

	matriz[1][0] = 2;
	matriz[1][1] = 530;
	matriz[1][2] = 5000;

	matriz[2][0] = 3;
	matriz[2][1] = 800;
	matriz[2][2] = 3700;

	matriz[3][0] = 4;
	matriz[3][1] = 1100;
	matriz[3][2] = 3310;*/

	int cost[life];
	int mantain = 0;

	for (int year = 0;year < life;year ++){
		for (int rango=0;rango<=year;rango ++){
			mantain = mantain + matriz[rango][1];
		}
		cost[year] = buyCost + mantain - matriz[year][2];
		mantain = 0;
		printf("Costo del a;o %d es %d\n", year+1, cost[year] );
	}	

	//VA A CONTENER CADA PLAN 
	Objects G[time + 1];	

	//time == plazo del proyecto
	for (int i = time;i>=0;i--){
		int min = 0;
		int posible = 0;
		int flag = 0;
		int min_year = 0;
		Objects object;
		object.value = 0;
		object.min_year = 0;

		for (int x = i+1;x<=time;x++){
			Objects lastObject = G[x];
			if (flag < life){
				flag ++;
				if (object.value == 0){
					object.value = cost[(x-i)-1] + lastObject.value;
					object.min_year = x;
			}
			else{
				posible = cost[(x-i)-1] + lastObject.value;
				if (posible<object.value){
					object.value = posible;
					object.min_year = x;
				}
			}
			}
		}
		G[i] = object;
	}

	for(int x =0 ; x < time+1; x++) {
			Objects result = G[x];
			printf("Estamos con el x: %d \n %d|%d\n",x,result.value,result.min_year);
		}



}