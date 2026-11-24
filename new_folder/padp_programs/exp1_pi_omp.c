#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
int main() {
    long niter, count = 0;
    printf("Enter the number of iterations used to estimate pi: ");
    scanf("%ld", &niter);
    double t = omp_get_wtime();
    #pragma omp parallel reduction(+:count)
    {
        unsigned int seed = 1234 + omp_get_thread_num();
        #pragma omp for
        for (long i = 0; i < niter; i++) {
            double x = (double)rand_r(&seed) / RAND_MAX;
            double y = (double)rand_r(&seed) / RAND_MAX;
            if (x*x + y*y <= 1.0) count++;
        }
    }
    t = omp_get_wtime() - t;
    double pi = 4.0 * count / niter;
    printf("# of trials = %ld, threads = %d, estimate of pi = %.6f\n", niter, omp_get_max_threads(), pi);
    printf("Time taken = %f s\n", t);
    return 0;
}
