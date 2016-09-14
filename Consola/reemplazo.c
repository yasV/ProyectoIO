#include <stdio.h>

int matriz[3][3];
int life = 3;
int buyCost = 500;
int time = 5;

void main(int argc, char const *argv[])
{
	
	matriz[0][0] = 1;
	matriz[0][1] = 30;
	matriz[0][2] = 400;

	matriz[1][0] = 2;
	matriz[1][1] = 40;
	matriz[1][2] = 300;

	matriz[2][0] = 3;
	matriz[2][1] = 60;
	matriz[2][2] = 250;

	int cost[life];
	int mantain = 0;

	for (int year = 0;year < life;year ++){
		for (int rango=0;rango<=year;rango ++){
			 mantain =mantain + matriz[rango][1];
		}
		cost[year] = buyCost + mantain - matriz[year][2];
		mantain = 0;
	}

	int G[time + 1][2];	

	for (int i = time;i>=0;i--){
		int min = 0;
		int posible = 0;
		int flag = 0;
		int min_year = 0;
		for (int x = i+1;x<=time;x++){
			if (flag < life){
				flag ++;
				if (min == 0){
					min = cost[(x-i)-1] + G[x][0];
					min_year = x;
			}
			else{
				posible = cost[(x-i)-1] + G[x][0];
				if (posible<min){
					min = posible;
					min_year = x;
				}
			}
			}
		}
		G[i][0] = min;
		G[i][1] = min_year;
		printf("%d|%d\n",G[i][0],G[i][1]);
	}



}