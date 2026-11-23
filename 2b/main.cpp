#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int n = 500;
    int *A = new int[n * n], *B = new int[n * n];
    int *C_with = new int[n * n](), *C_without = new int[n * n]();

    for (int idx = 0; idx < n * n; idx++) A[idx] = B[idx] = 1;

    int i, j, k, sum;

    // 1. With explicit scoping (Correct)
    #pragma omp parallel for private(i, j, k, sum) shared(A, B, C_with)
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            sum = 0;
            for (k = 0; k < n; k++) sum += A[n * i + k] * B[n * k + j];
            C_with[n * i + j] = sum;
        }

    // 2. Without scoping (Race condition on j, k, sum)
    #pragma omp parallel for
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++) {
            sum = 0;
            for (k = 0; k < n; k++) sum += A[n * i + k] * B[n * k + j];
            C_without[n * i + j] = sum;
        }

    int errors = 0;
    for (int idx = 0; idx < n * n; idx++)
        if (C_with[idx] != C_without[idx]) errors++;

    cout << "Errors without scope: " << errors << " / " << n * n << endl;

    delete[] A; delete[] B; delete[] C_with; delete[] C_without;
}
