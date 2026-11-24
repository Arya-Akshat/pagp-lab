#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>
long unfriendly(long n) {
    char *comp = calloc(n + 1, 1);
    long count = 0;
    for (long i = 2; i * i <= n; i++)
        if (!comp[i])
            for (long j = i * i; j <= n; j += i) comp[j] = 1;
    for (long i = 2; i <= n; i++) if (!comp[i]) count++;
    free(comp);
    return count;
}
long friendly(long n) {
    long m = (long)sqrt((double)n);
    char *comp = calloc(n + 1, 1);
    long *primes = malloc((m + 1) * sizeof(long)), np = 0, count = 0;
    for (long i = 2; i <= m; i++)
        if (!comp[i]) {
            primes[np++] = i;
            for (long j = i * i; j <= m; j += i) comp[j] = 1;
        }
    count = np;
    for (long lo = m + 1; lo <= n; lo += m) {
        long hi = lo + m - 1 < n ? lo + m - 1 : n;
        for (long k = 0; k < np; k++) {
            long p = primes[k];
            long start = ((lo + p - 1) / p) * p;
            for (long j = start; j <= hi; j += p) comp[j] = 1;
        }
        for (long i = lo; i <= hi; i++) if (!comp[i]) count++;
    }
    free(comp); free(primes);
    return count;
}
long parallel(long n) {
    long m = (long)sqrt((double)n);
    char *comp = calloc(n + 1, 1);
    long *primes = malloc((m + 1) * sizeof(long)), np = 0, count = 0;
    for (long i = 2; i <= m; i++)
        if (!comp[i]) {
            primes[np++] = i;
            for (long j = i * i; j <= m; j += i) comp[j] = 1;
        }
    count = np;
    #pragma omp parallel for schedule(dynamic) reduction(+:count)
    for (long lo = m + 1; lo <= n; lo += m) {
        long hi = lo + m - 1 < n ? lo + m - 1 : n;
        for (long k = 0; k < np; k++) {
            long p = primes[k];
            long start = ((lo + p - 1) / p) * p;
            for (long j = start; j <= hi; j += p) comp[j] = 1;
        }
        for (long i = lo; i <= hi; i++) if (!comp[i]) count++;
    }
    free(comp); free(primes);
    return count;
}
int main(int argc, char *argv[]) {
    long n = (argc > 1) ? atol(argv[1]) : 100000000;
    double t; long c1, c2, c3;
    t = omp_get_wtime(); c1 = unfriendly(n); double t1 = omp_get_wtime() - t;
    t = omp_get_wtime(); c2 = friendly(n); double t2 = omp_get_wtime() - t;
    t = omp_get_wtime(); c3 = parallel(n); double t3 = omp_get_wtime() - t;
    printf("N = %ld\n", n);
    printf("Cache unfriendly : primes = %ld time = %f s\n", c1, t1);
    printf("Cache friendly : primes = %ld time = %f s\n", c2, t2);
    printf("Parallel (%d thr) : primes = %ld time = %f s\n", omp_get_max_threads(), c3, t3);
    printf("Correctness : %s\n", (c1 == c2 && c2 == c3) ? "all counts match" : "MISMATCH");
    return 0;
}
