#include "TP1Functions.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdio.h>

int max(int a, int b) {
	if (a > b) {
		return a;
	}
	return b;
}

int read_TP1_instance(FILE*fin,dataSet* dsptr)
{
	int rval = 0;

	//capacite b
	int b;
	//Nombre d'objets
	int n;
	rval = fscanf(fin,"%d,%d\n",&n,&b);
	dsptr->b = b;
	dsptr->n = n;
	dsptr->c = (int*)malloc(sizeof(int)*n);
	dsptr->a = (int*)malloc(sizeof(int)*n);


	int i;
	for( i = 0 ; i < n ; i++)
		rval = fscanf(fin,"%d,%d\n",&(dsptr->c[i]),&(dsptr->a[i]));

	fprintf(stderr,"\nInstance file read, we have capacity %d and there is %d items of values/weights:\n",
			b,n);
	for( i = 0 ; i < n ; i++)
		fprintf(stderr,"%d,%d\n",dsptr->c[i],dsptr->a[i]);
	fprintf(stderr,"\n");


	return rval;
}

// une fonction pour générer un problème aléatoirement
// n est le nombre des objets
// b est le volume maximum
// max_c est la valeur maximum de la valeur des objets
// max_a est la valeur maximum du poids des objets
int generate_TP1_instance(dataSet* dsptr, int n, int b, int max_c, int max_a) {
	// initialiser les variables
	dsptr->n = n;
	dsptr->b = b;
	dsptr->c = (int*)malloc(sizeof(int)*n);
	dsptr->a = (int*)malloc(sizeof(int)*n);

	// détérminer le seed
	srand(time(NULL));

	// générer n objets alétoirement
	for (int i = 0; i < n; i++) {
		dsptr->c[i] = rand() % max_c + 1;
		dsptr->a[i] = rand() % max_a + 1;
	}

	return 0;
}

// une fonction pour résoudre le problème avec la programmation dynamique
int TP1_solve_exact(dataSet* dsptr)
{
	// initialiser des variables avec des noms court pour faciliter l'écriture du code
	int b = dsptr->b;
	int n = dsptr->n;
	int* c = dsptr->c;
	int* a = dsptr->a;
	dsptr->z = (int*)malloc(sizeof(int)*(b+1));
	dsptr->d = (int*)malloc(sizeof(int)*(b+1));
	dsptr->z1 = (int*)malloc(sizeof(int)*(b+1));
	dsptr->x = (int*)malloc(sizeof(int)*n);

	int* z = dsptr->z;
	int* d = dsptr->d;
	int* z1 = dsptr->z1;
	int* x = dsptr->x;

	int y = 0;
	int k = 0;
	int j = 0;

	// start: code d'algo 1 de la fiche de TP1
	for (y = 0; y <= b; y++) {
		z[y] = 0;
		d[y] = 0;
	}

	for (k = 0; k < n; k++) {
		for(y = 0; y <= b; y++) {
			z1[y] = z[y];
		}
		for(y = a[k]; y <= b; y++) {
			if((z1[y - a[k]] + c[k]) > z1[y]) {
				d[y] = k;
				z[y] = max(z1[y], z1[y - a[k]] + c[k]);
			}
		}
	}

	for (j = 0; j <n; j++) {
		x[j] = 0;
	}

	y = b;

	while(y > 0) {
		while(z[y] == z[y - 1]) {
			y = y - 1;
		}

		x[d[y]] = 1;

		y = y - a[d[y]];
	}
	// end: code d'algo 1 de la fiche de TP1

	// start: affichage des résultats
	fprintf(stderr,"Results: Z = (");
	for (int i = 0; i <= b; i++) {
		fprintf(stderr,"%d", z[i]);
		if (i < b)
			fprintf(stderr,",");
	}
	fprintf(stderr,"), D = (");
	for (int i = 0; i <= b; i++) {
		fprintf(stderr,"%d", d[i]);
		if (i < b)
			fprintf(stderr,",");
	}
	fprintf(stderr,"), x* = (");
	for (int i = 0; i < n; i++) {
		fprintf(stderr,"%d", x[i]);
		if (i < n-1)
			fprintf(stderr,",");
	}
	fprintf(stderr,")\n");
	// end: affichage des résultats

	return 0;
}

// une fonction pour comparer deux objets par le rapport valuer/poids
// cette fonction est utilisé pour qsort
int TP1_compare_ns(const void* a, const void* b)
{
    const ns_object* const * arg1 = a;
    const ns_object* const * arg2 = b;

    if ((*arg1)->c/(*arg1)->a > (*arg2)->c/(*arg2)->a) return -1;
    if ((*arg1)->c/(*arg1)->a < (*arg2)->c/(*arg2)->a) return 1;
    return 0;
}

// une fonction pour trier les objets par le rapport valuer/poids qui utilise la fonction qsort
int TP1_sort(dataSet* d){
	// on crée un array de ns_object qui contient des valeur float
	ns_object** ns_objects = malloc(sizeof(struct ns_object *) * d->n);

	// on copie les objets dans l'array de ns_object
	for(int i=0;i<d->n;i++){
		ns_objects[i] = malloc(sizeof(struct ns_object *));
		ns_objects[i]->c = d->c[i];
		ns_objects[i]->a = d->a[i];
	}

	// on fait le tri en utilisant qsort
    qsort(ns_objects, d->n, sizeof(ns_object*), &TP1_compare_ns);

	// on re-organise les objets en fonction des résultats du tri
    for (int i = 0; i < d->n; i++) {
        d->c[i] = (int)ns_objects[i]->c;
        d->a[i] = (int)ns_objects[i]->a;
    }

    return 0;
}

// Knapsack - Linear Relaxation
int TP2_linear_relaxation(dataSet* dsptr) {
	TP1_sort(dsptr);

	int n = dsptr->n;
	int b = dsptr->b;
	int* a = dsptr->a;
	dsptr->x_lin = (float*)malloc(sizeof(float)*n);
	float* x = dsptr->x_lin;

	// Keep track of the total weight of objects added to the knapsack
	int total_weight = 0;
	for (int i = 0; i < n; i++) {
		// If the knapsack is full, return the solution
		if(total_weight + a[i] <= b){
			x[i] = 1;
			total_weight += a[i];
		}else{
			// Calculate the remaining capacity of the knapsack and add a fraction of the object that fits
			float remaining_capacity = b - total_weight;
			x[i] = remaining_capacity / a[i];
			break;
		}
	}
	
	// Print the results
	fprintf(stderr,"), x* linear relax. = (");
	for (int i = 0; i < n; i++) {
		fprintf(stderr,"%f", x[i]);
		if (i < n-1)
			fprintf(stderr,",");
	}
	fprintf(stderr,")\n");

	// Return 0 indicating success
	return 0;
}

// Knapsack - Greedy
int TP2_greedy(dataSet* d) {
	d->x_greedy = (int*)malloc(sizeof(int)*d->b);
	int i;
	int res = 0;
	// Sort the items by decreasing utility
	TP1_sort(d);

	// Initialize the vector
	for (i = 0; i< d -> n; i++) {
		d -> x_greedy[i] = 0;
	}

	// Initialize remaining capacity
	int remaining_capacity = d -> b;

	// Loop through each object descending
	for (i = 0; i < d-> n; i++) {
		// If the knapsack is full, stop
		if (remaining_capacity == 0) {
			break;
		}
		// If the object fits, add it to the solution vector and subtract its weight from the remaining capacity
		if (remaining_capacity >= d -> a[i]) {
			d->x_greedy[i] = 1;
			remaining_capacity -= d -> a[i];
			res += d -> c[i];
		}
	}

	// Print the results
	fprintf(stderr,"), x* greedy = (");
	for (int i = 0; i < d->n; i++) {
		fprintf(stderr,"%d", d->x[i]);
		if (i < d->n-1)
			fprintf(stderr,",");
	}
	fprintf(stderr,")\n");
	return 0;
}