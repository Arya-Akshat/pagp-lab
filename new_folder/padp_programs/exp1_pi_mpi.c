#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    long niter = (argc > 1) ? atol(argv[1]) : 10000000;
    long local = 0, total = 0;
    unsigned int seed = 1234 + rank;
    double t = MPI_Wtime();
    for (long i = rank; i < niter; i += size) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if (x*x + y*y <= 1.0) local++;
    }
    MPI_Reduce(&local, &total, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);
    t = MPI_Wtime() - t;
    if (rank == 0)
        printf("# of trials = %ld, processes = %d, pi = %.6f, time = %f s\n", niter, size, 4.0 * total / niter, t);
    MPI_Finalize();
    return 0;
}
