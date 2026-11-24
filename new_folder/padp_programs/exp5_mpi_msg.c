#include <stdio.h>
#include <string.h>
#include <mpi.h>
#define BUFFER_SIZE 32
int main(int argc, char *argv[]) {
    int rank, size, tag = 0;
    char msg[BUFFER_SIZE];
    char *words[] = {"", "HELLO", "CSE", "RVCE"};
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (rank != 0) {
        strcpy(msg, words[rank]);
        MPI_Send(msg, BUFFER_SIZE, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
    } else {
        for (int src = 1; src < size; src++) {
            MPI_Recv(msg, BUFFER_SIZE, MPI_CHAR, src, tag, MPI_COMM_WORLD, &status);
            printf("Rank 0 received \"%s\" from rank %d\n", msg, status.MPI_SOURCE);
        }
    }
    MPI_Finalize();
    return 0;
}
