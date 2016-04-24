#include <mpi.h>
#include <iostream>

int main(int argc, char** argv)
{
	MPI_Init(&argc,&argv);
	int rank,world_sz;
	MPI_Comm_rank (MPI_COMM_WORLD, &myrank);
	MPI_Comm_size (MPI_COMM_WORLD, &world_sz);
	
	
	MPI_Finalize();
}
