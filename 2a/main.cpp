#include <iostream>
#include <omp.h>
using namespace std;

int main() {
    int n = 1000;
    int *A = new int[n * n];
    int *B = new int[n * n];
    int *C = new int[n * n]();

    for (int i = 0; i < n * n; i++) A[i] = B[i] = 1;

    double t = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            for (int k = 0; k < n; k++)
                C[n * i + k] += A[n * i + j] * B[n * j + k];

    cout << "Time: " << omp_get_wtime() - t << endl;
    cout << "C[0]: " << C[0] << " (Expected: " << n << ")" << endl;

    delete[] A; delete[] B; delete[] C;
}
