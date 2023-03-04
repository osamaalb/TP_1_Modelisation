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

int generate_TP1_instance(dataSet* dsptr, int n, int b, int max_c, int max_a) {
	dsptr->n = n;
	dsptr->b = b;
	dsptr->c = (int*)malloc(sizeof(int)*n);
	dsptr->a = (int*)malloc(sizeof(int)*n);

	max_c++;
	max_a++;

	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		dsptr->c[i] = rand() % max_c;
		dsptr->a[i] = rand() % max_a;
	}

	return 0;
}

int TP1_solve_exact(dataSet* dsptr)
{
	int b = dsptr->b;
	int n = dsptr->n;
	int* c = dsptr->c;
	int* a = dsptr->a;
	dsptr->z = (int*)malloc(sizeof(int)*b+1);
	dsptr->d = (int*)malloc(sizeof(int)*b+1);
	dsptr->z1 = (int*)malloc(sizeof(int)*b+1);
	dsptr->x = (int*)malloc(sizeof(int)*n);

	int* z = dsptr->z;
	int* d = dsptr->d;
	int* z1 = dsptr->z1;
	int* x = dsptr->x;

	for (int y = 0; y <= b; y++) {
		z[y] = 0;
		d[y] = 0;
	}

	for (int k = 0; k < n; k++) {
		for (int y = 0; y <= b; y++) {
			z1[y] = 0;
		}

		for (int y = a[k + 1]; y <= b; y++) {
			if (z1[y - a[k + 1]] + c[k + 1] > z1[y]) {
				d[y] = k + 1;
				z[y] = max(z1[y], z1[y - a[k + 1]] + c[k + 1]);
			}
		}
	} 

	for (int j = 1; j < n; j++) {
		x[j] = 0;
	}

	int y = b;

	while(y > 0) {
		while(z[y] > z[y - 1]) {
			y = y - 1;
		}

		x[d[y]] = 1;

		y = y - a[d[y]];
	}

	fprintf(stderr,"Result: %d\n", z[b]);

	return 0;
}

int TP1_compare_ns(const void* a, const void* b)
{
    const ns_object* arg1 = a;
    const ns_object* arg2 = b;
 
    if (arg1->c/arg1->a < arg2->c/arg2->a) return -1;
    if (arg1->c/arg1->a > arg2->c/arg2->a) return 1;
    return 0;
}

int TP1_sort(dataSet* d){
	ns_object** ns_objects = malloc(sizeof(struct ns_object *) * d->n);
 
	for(int i=0;i<d->n;i++){
		ns_objects[i] = malloc(sizeof(struct ns_object *));
		ns_objects[i]->c = d->c[i];
		ns_objects[i]->a = d->a[i];
	}

    qsort(ns_objects, d->n, sizeof(ns_object*), &TP1_compare_ns);

    for (int i = 0; i < d->n; i++) {
        d->c[i] = (int)ns_objects[i]->c;
        d->a[i] = (int)ns_objects[i]->a;
    }
 
    return 0;
}

/*
int tp1_compare_ns(const void* a, const void* b)
{
    const ns_object* arg1 = a;
    const ns_object* arg2 = b;
 
    if (arg1->u < arg2->u) return -1;
    if (arg1->u > arg2->u) return 1;
    return 0;
}

int tp1_sort(dataSet* d){
	ns_object* ns_objects[1000000000];

	dataSet d1;
	memcpy(&d1, d, sizeof(d));
 
	for(int i=0;i<d->n;i++){
		ns_objects[i] = malloc(sizeof(struct ns_object *));
		ns_objects[i]->i = i;
		ns_objects[i]->u = (float)d->c[i] / (float)d->a[i];
	}
    qsort(ns_objects, d->n, sizeof(ns_object), tp1_compare_ns);

    for (int i = 0; i < d->n; i++) {
        d->c[i] = d1.c[ns_objects[i]->i];
        d->a[i] = d1.a[ns_objects[i]->i];
    }
 
    return 0;
}
*/