#include <stdio.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const char *m[] = {"", "Hello", "RVCE", "CSE"};
    char msg[32];

    if (rank != 0) {
        strcpy(msg, m[rank]);
        MPI_Send(msg, 32, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    } else {
        for (int i = 1; i < size; i++) {
            MPI_Recv(msg, 32, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Received %s in process %d from process %d\n", msg, rank, i);
        }
    }

    MPI_Finalize();
}
