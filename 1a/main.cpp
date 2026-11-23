#include <iostream>
#include <cstdlib>
#include <omp.h>
using namespace std;

int main() {
    int th, n = 10000, c = 0;
    cout << "Enter threads: ";
    cin >> th;

    double t = omp_get_wtime();

    #pragma omp parallel for num_threads(th) reduction(+:c)
    for (int i = 0; i < n; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1) c++;
    }

    t = omp_get_wtime() - t;

    cout << "Time: " << t << "\nPi: " << (double)c / n * 4 << endl;
}
