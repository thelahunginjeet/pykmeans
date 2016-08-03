#include "kmeans.h"

/* remove this after debugging is finished */
void printarray(double *X, int rows, int cols) {
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
        printf("%le ",X[i*cols + j]);
    }
    printf("\n");
  }
  return;
}

void choice_floyd(pcg32_random_t *rng, int *values, int M, int N) {
  /*
  Floyd's algorithm for returning a list of M random numbers in the range
  0,...,N-1.  Preferable to Knuth's method when M << N (though Knuth is still
  viable).  Uses Melissa O'Neil's PCG generator.
  */
  int in;
  int im = 0;
  unsigned char *is_used = malloc(N*sizeof(unsigned char));
  for (int i = 0; i < N; i++) {
    is_used[i] = 0;
  }

  for(in = N - M; in < N && im < M; ++in) {
    int r = pcg32_boundedrand_r(rng,N-1);

    if (is_used[r]) {
      r = in;
    }

    values[im++] = r;
    is_used[r] = 1;
  }

  free(is_used);
}


double euclidean(double *x, double *y, int N) {
  /* euclidean (sum squared) distance */
  int i;
  double d = 0.0;
  for (i = 0; i < N; i++){
    d += (x[i] - y[i])*(x[i] - y[i]);
  }
  return d;
}

void kmeans(double *X, int N, int p, int *clusterids, int K, int init, double tol, int nruns) {
    /*
    kmeans algorithm, currently featuring:
      +euclidean distance only
      +initialization with random centers (init = 0)
      +returns best clustering result from nruns tries
      +printf for errors instead of error code return.
    function returns void but stores the best cluster assignments in clusterids
    */

    double centerchg,odist,ndist,ssqd;

    /* number of exemplars of each class */
    double *nSamples = malloc(K*sizeof(double));
    /* indices of starting centers (rows of X) */
    int *centerloc = malloc(K*sizeof(int));
    /* cluster centers */
    double *oldCenters = malloc(K*p*sizeof(double));
    double *newCenters = malloc(K*p*sizeof(double));
    /* temporary labels */
    int *labels = malloc(N*sizeof(int));

    // RNG stuff
    pcg32_random_t rng;
    pcg32_srandom_r(&rng, time(NULL), (intptr_t)&rng);

    double minssqd = INFINITY;

    for (int irun = 0; irun < nruns; irun++) {
        centerchg = 1.0+tol;

        /* initialize cluster centers */
        if(init == 0) {
            choice_floyd(&rng,centerloc,K,N);
        } else {
            printf("ERROR! Unknown initialization method.");
        }
        for (int i = 0; i < K; i ++) {
            for (int j = 0; j < p; j++) {
                oldCenters[i*p + j] = X[centerloc[i]*p + j];
            }
        }

        /* run until convergence */
        for (int r = 0; r < 1000; r++) {

            /* find closest center */
            for (int i = 0; i < N; i++) {
                odist = euclidean(&X[i*p],&oldCenters[0],p);
                labels[i] = 0;
                for (int k = 0; k < K; k++) {
                    ndist = euclidean(&X[i*p],&oldCenters[k*p],p);
                    if (ndist < odist) {
                        odist = ndist;
                        labels[i] = k;
                    }
                }
            }

            /* recompute new centers */
            for (int k = 0; k < K; k++) {
                nSamples[k] = 0.0;
                for(int j = 0; j < p; j++) {
                    newCenters[k*p + j] = 0.0;
                }
            }
            for (int i = 0; i < N; i++) {
                nSamples[labels[i]] += 1.0;
            }

            for (int j = 0; j < N ; j++) {
                for (int l = 0; l < p; l++) {
                    newCenters[labels[j]*p + l] += X[j*p + l]/nSamples[labels[j]];
                }
            }

            /* check to see if centers have changed enough to continue */
            centerchg = 0.0;
            for (int k = 0; k < K; k++) {
                centerchg += euclidean(&newCenters[k*p],&oldCenters[k*p],p);
            }
            if (centerchg < tol) {
                break;
            }

            /* copy newCenters into oldCenters */
            for (int i = 0; i < K; i++) {
                for (int j = 0; j < p; j++) {
                    oldCenters[i*p + j] = newCenters[i*p + j];
                }
            }

        }
    /* compute ssd and copy into clusterids in case this is the best run */
        ssqd = 0.0;
        for (int i = 0; i < N; i++) {
            ssqd += euclidean(&X[i*p],&newCenters[labels[i]*p],p);
        }
        if (ssqd < minssqd) {
            minssqd = ssqd;
            memcpy(clusterids,labels,N*sizeof(*clusterids));
        }
    }


    /* free memory */
    free(nSamples);
    free(labels);
    free(centerloc);
    free(oldCenters);
    free(newCenters);

    return;
}
