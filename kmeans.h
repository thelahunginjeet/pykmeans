#ifndef KMEANS_H_INCLUDED
#define KMEANS_H_INCLUDED 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "pcg_basic.h"

void choice_floyd(pcg32_random_t *rng, int *values, int M, int N);

int rand_cdf_index(pcg32_random_t *rng, double *cdf, int N);

/* x, y are both arrays of length N */
double euclidean(double *x, double *y, int N);

double cosine(double *x, double *y, int N);

double manhattan(double *x, double *y, int N);

double correlation(double *x, double *y, int N);

/* X is the N*p row-ordered matrix of feature data
   +N and p are the size parameters
   +metric controls the distance function
      0: euclidean distance
      1: cosine distance
      2: correlation distance
      3: manhattan distance
   +clusterids will store the cluster ids upon exit
   +K is the desired number of clusters
   +init controls initialization
      0: use K random samples (p-length vectors) as initial centers
      1: initialize using the kmeans++ method
   +once sum-squared difference in centers < tol, algorithm terminates
*/
void kmeans(double *X, int N, int p, int metric, int *clusterids, int K, int init, double tol, int nruns);

#endif // KMEANS_H_INCLUDED
