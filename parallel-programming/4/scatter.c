#include "mpi.h"

#include <stdio.h>

int main(int argc, char **argv) {
	int size, rank, sum = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	MPI_Reduce(&rank, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	if (rank == 0) 
		printf("%d\n", sum);

}
