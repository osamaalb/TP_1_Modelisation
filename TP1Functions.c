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

