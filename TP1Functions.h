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
	//Prog. dynamic : Tableau des solutions optimals de l'étape actuel
	int*z;
	//Prog. dynamic : Tableau de max{j ⩽ k : xj = 1 pour le probl`eme Pk(y)}
	int*d;
	//Prog. dynamic : Tableau des solutions optimals de l'étape précédent
	int*z1;
	//Prog. dynamic : La solution optimale
	int*x;

	//Relax. lin : La solution optimale
	float*x_lin;

	//Greedy : La solution optimale
	int*x_greedy;

} dataSet;

// un struct qui est utilisé pour faire la tri des objets
typedef struct ns_object{
	float c;
	float a;
} ns_object;

int read_TP1_instance(FILE*fin,dataSet* dsptr);
int TP1_solve_exact(dataSet* dsptr);
int generate_TP1_instance(dataSet* dsptr, int n, int b, int max_c, int max_a);

int TP1_sort(dataSet* d);

int TP2_linear_relaxation(dataSet* dsptr);

int TP2_greedy(dataSet* d);