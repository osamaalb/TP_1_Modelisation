#include <ctype.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "TP1Functions.h"


int main(int argc, char **argv)
{
	int rval =0;	
	//File instance name
	//-F option
        char instance_file[1024];
        snprintf(       instance_file,
                        1024,
                        "%s",
                        "instance1.csv");
	
	/*
	char c;
        while ((c=getopt (argc, argv,"F:h")) != EOF)
	{
		switch(c)
		{
                        case 'F':
				snprintf(       instance_file,
						1024,
						"%s",
						optarg);
				break;

			case 'h':
				fprintf(stderr,"Usage: ./TP1 [options]\nOptions:\n\n");
				fprintf(stderr,"******** INSTANCE DATA ********\n");
				fprintf(stderr,"\t-F Instance file name to load............................(default %s).\n",instance_file);
				break;
			default:
				exit(0);
		}
	}
	*/

	dataSet data;

	//Open the instance file
	FILE* fin = fopen(instance_file,"r");
	read_TP1_instance(fin,&data);
	fclose(fin);

	//Generate a random instance
	generate_TP1_instance(&data, 10, 50, 15, 15);

	//Print the generated data
	for( int i = 0 ; i < data.n ; i++)
		fprintf(stderr,"%d,%d\n",data.c[i],data.a[i]);

	//Sort the data by the value/weight ratio
	TP1_sort(&data);

	fprintf(stderr, "\n\n");

	//Print the sorted data
	for( int i = 0 ; i < data.n ; i++)
		fprintf(stderr,"%d,%d\n",data.c[i],data.a[i]);

	//execute your solution methods on the instance you just read
	//Exact solution using dynamic programming
	TP1_solve_exact(&data);

	return rval;
}

