#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include "block.h"

enum tag { block_tag, block_row_tag, edge_sol_tag, block_sol_tag };

int main(int argc, char** argv)
{
	MPI_Init(&argc,&argv);
		
	int myrank;
	MPI_Comm_rank (MPI_COMM_WORLD, &myrank);
	
	
	if(myrank == 0){
		
		std::vector<double> v{ 1,2,9,8,3,9,5,2, 3,3,5,4,3,7,5,5, 9,1,1,3,3,5,4,8, 7,2,3,1,8,1,3,1 };
		block b(4,3);
		b.v = v;
		assert(v.size() == b.v.size());

		MPI_Send (b.v.data(), b.v.size(), MPI_DOUBLE, 1, block_tag, MPI_COMM_WORLD);
	}
	else {
		MPI_Status status;
		std::size_t count;
		MPI_Probe(0, block_tag, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_DOUBLE, &count);
	
		
		block b(4,3);
		std::vector<double> v(count);
		MPI_Recv(v.data(), count, MPI_DOUBLE, 0, block_tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		b.v = v;
		std::cout << b << '\n';
	}
	
	
	MPI_Finalize();
}
