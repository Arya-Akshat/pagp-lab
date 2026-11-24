#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
int main() {
    int n, nt, i, j, k;
    printf("Enter matrix size N and number of threads: ");
    scanf("%d %d", &n, &nt);
    omp_set_num_threads(nt);
    int *A = malloc(n*n*sizeof(int)), *B = malloc(n*n*sizeof(int));
    int *C = malloc(n*n*sizeof(int)), *S = malloc(n*n*sizeof(int)), *W = malloc(n*n*sizeof(int));
    for (i = 0; i < n*n; i++) { A[i] = rand() % 10; B[i] = rand() % 10; }
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            int sum = 0;
            for (k = 0; k < n; k++) sum += A[i*n+k] * B[k*n+j];
            S[i*n+j] = sum;
        }
    double t = omp_get_wtime();
    #pragma omp parallel for private(i, j, k) shared(A, B, C, n)
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            int sum = 0;
            for (k = 0; k < n; k++) sum += A[i*n+k] * B[k*n+j];
            C[i*n+j] = sum;
        }
    t = omp_get_wtime() - t;
    int sum = 0;
    #pragma omp parallel for
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            for (k = 0; k < n; k++) sum = sum + A[i*n+k] * B[k*n+j];
            W[i*n+j] = sum;
            sum = 0;
        }
    int okC = 1, errW = 0;
    for (i = 0; i < n*n; i++) { if (C[i] != S[i]) okC = 0; if (W[i] != S[i]) errW++; }
    if (n <= 4) {
        printf("A:\n"); for (i=0;i<n;i++){ for(j=0;j<n;j++) printf("%4d",A[i*n+j]); printf("\n"); }
        printf("B:\n"); for (i=0;i<n;i++){ for(j=0;j<n;j++) printf("%4d",B[i*n+j]); printf("\n"); }
        printf("A x B:\n"); for (i=0;i<n;i++){ for(j=0;j<n;j++) printf("%5d",C[i*n+j]); printf("\n"); }
    }
    printf("N = %d, threads = %d, time (with private) = %f s\n", n, nt, t);
    printf("With private(i,j,k): %s\n", okC ? "CORRECT" : "WRONG");
    printf("Without scope : %d of %d elements wrong\n", errW, n*n);
    free(A); free(B); free(C); free(S); free(W);
    return 0;
}
