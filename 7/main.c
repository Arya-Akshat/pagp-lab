#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX 1000
#define TOL 1e-6

double now() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

int jacobi(double *u, double *v, double *f, int n, int acc) {
    int nn = n * n, iter = 0;
    double diff;

    #pragma acc data copy(u[0:nn]) copyin(f[0:nn]) create(v[0:nn]) if(acc)
    do {
        diff = 0;

        #pragma acc parallel loop collapse(2) reduction(max:diff) if(acc)
        for (int i = 1; i < n - 1; i++)
            for (int j = 1; j < n - 1; j++) {
                int k = i * n + j;
                v[k] = (u[k - n] + u[k + n] + u[k - 1] + u[k + 1] - f[k]) / 4;
                double d = fabs(v[k] - u[k]);
                if (d > diff) diff = d;
            }

        #pragma acc parallel loop collapse(2) if(acc)
        for (int i = 1; i < n - 1; i++)
            for (int j = 1; j < n - 1; j++)
                u[i * n + j] = v[i * n + j];

    } while (++iter < MAX && diff > TOL);

    return iter;
}

int main(int argc, char **argv) {
    int n = argc > 1 ? atoi(argv[1]) : 1000, nn = n * n;
    double *a = calloc(nn, sizeof(double)), *b = calloc(nn, sizeof(double));
    double *v = calloc(nn, sizeof(double)), *f = malloc(nn * sizeof(double));
    for (int i = 0; i < nn; i++) f[i] = 1;

    double t = now();
    int s = jacobi(a, v, f, n, 0);
    double ts = now() - t;

    t = now();
    int p = jacobi(b, v, f, n, 1);
    double tp = now() - t;

    double err = 0;
    for (int i = 0; i < nn; i++)
        if (fabs(a[i] - b[i]) > err) err = fabs(a[i] - b[i]);

    printf("Grid: %d x %d\n", n, n);
    printf("Serial:  %f s (%d iter)\n", ts, s);
    printf("OpenACC: %f s (%d iter)\n", tp, p);
    printf("Speedup: %.2fx | Result: %s\n", ts / tp, err < 1e-9 ? "CORRECT" : "MISMATCH");

    free(a); free(b); free(v); free(f);
}
