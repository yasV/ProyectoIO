#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


char buffer[9];

int inputMatrixNumber = 0;
FILE * fileTableData;

typedef struct  {
   int rows;
   int columns;
} MatrixObject;