#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <assert.h>




typedef struct dataSet 
{
	//Attributes of the instance
	//Nombre d'objets
	int n;
	//Capacite b
	int b;
	//Tableau d'entiers de taille n contenant la valeur de chacun des objets
	int*c;
	//Tableau d'entiers de taille n contenant le poids de chacun des objets
	int*a;

	int*z;

	int*d;

	int*z1;

	int*x;

} dataSet;

int read_TP1_instance(FILE*fin,dataSet* dsptr);
int TP1_solve_exact(dataSet* dsptr);


