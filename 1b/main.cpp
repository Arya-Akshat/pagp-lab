#include <iostream>
#include <cstdlib>
#include <mpi.h>
using namespace std;

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size, n = 10000, count = 0, red_count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    double t = MPI_Wtime();
    srand(3655942);

    for (int i = 0; i < n / size; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1.0) count++;
    }

    MPI_Reduce(&count, &red_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        cout << "Time: " << MPI_Wtime() - t << endl;
        cout << "Processes: " << size << "\nTrials: " << n << endl;
        cout << "Est Pi: " << (double)red_count / n * 4 << endl;
    }

    MPI_Finalize();
}
