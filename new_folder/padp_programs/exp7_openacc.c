#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
double now() { struct timeval t; gettimeofday(&t, NULL); return t.tv_sec + t.tv_usec / 1e6; }
int main(int argc, char *argv[]) {
    int n = (argc > 1) ? atoi(argv[1]) : 1000;
    double *restrict a = malloc((size_t)n*n*sizeof(double));
    double *restrict b = malloc((size_t)n*n*sizeof(double));
    double *restrict c = malloc((size_t)n*n*sizeof(double));
    double *restrict d = malloc((size_t)n*n*sizeof(double));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            a[i*n+j] = i + j;
            b[i*n+j] = i - j;
        }
    double t1 = now();
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            double s = 0;
            for (int k = 0; k < n; k++) s += a[i*n+k] * b[k*n+j];
            d[i*n+j] = s;
        }
    double tcpu = now() - t1;
    t1 = now();
    #pragma acc data copyin(a[0:n*n], b[0:n*n]) copyout(c[0:n*n])
    {
        #pragma acc parallel loop collapse(2)
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++) {
                double s = 0;
                #pragma acc loop reduction(+:s)
                for (int k = 0; k < n; k++) s += a[i*n+k] * b[k*n+j];
                c[i*n+j] = s;
            }
    }
    double tacc = now() - t1;
    for (int i = 0; i < n*n; i++)
        if (c[i] != d[i]) { printf("Error at %d: %f vs %f\n", i, c[i], d[i]); return 1; }
    printf("N = %d\n", n);
    printf("Serial CPU : %.6f s\n", tcpu);
    printf("OpenACC : %.6f s\n", tacc);
    printf("OpenACC matrix multiplication test was successful!\n");
    free(a); free(b); free(c); free(d);
    return 0;
}
