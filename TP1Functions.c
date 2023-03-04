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
	dsptr->z = (int*)malloc(sizeof(int)*b+1);
	dsptr->d = (int*)malloc(sizeof(int)*b+1);
	dsptr->z1 = (int*)malloc(sizeof(int)*b+1);
	dsptr->x = (int*)malloc(sizeof(int)*n+1);

	int* z = dsptr->z;
	int* d = dsptr->d;
	int* z1 = dsptr->z1;
	int* x = dsptr->x;

	// start: code d'algo 1 de la fiche de TP1
	for (int y = 0; y <= b; y++) {
		z[y] = 0;
		d[y] = 0;
	}

	for (int k = 0; k <= n; k++) {
		for (int y = 0; y <= b; y++) {
			z1[y] = z[y];
		}

		for (int y = a[k + 1]; y <= b; y++) {
			if (z1[y - a[k + 1]] + c[k + 1] > z1[y]) {
				d[y] = k + 1;
				z[y] = max(z1[y], z1[y - a[k + 1]] + c[k + 1]);
			}
		}
	} 

	for (int j = 1; j <= n; j++) {
		x[j] = 0;
	}

	int y = b;

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
	for (int i = 1; i <= n; i++) {
		fprintf(stderr,"%d", x[i]);
		if (i < n)
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