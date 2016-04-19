#include "mpi.h"
#include <iostream>
#include "Row.h"
#include "Block.h"
#include "Dispatcher.h"
#include "BlockWorker.h"
#include <assert.h>
#include <sstream>
#define SIZE 4


// pars: <prog> block_sz blocks_nr edge_sz 
int main(int argc, char *argv[])  
try{
	if (argc != 4) {
		std::cerr << "No parameters\n";
		std::terminate();
	}

	const int block_sz  = atoi(argv[1]);
	const int blocks_nr = atoi(argv[2]);
	const int edge_sz   = atoi(argv[3]);
	
	MPI_Init(&argc, &argv);
	
	int myrank, world_sz;
	
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_sz);

	if (myrank == 0) {
		Dispatcher d(block_sz,blocks_nr,edge_sz);
		d.work();
		std::cout << "exiting " << myrank << '\n';
	}
	else {
		BlockWorker bw(block_sz,blocks_nr,edge_sz);
		bw.work();
		std::cout << "exiting " << myrank << '\n';
	}
	
	MPI_Finalize();
}
catch(DispatcherError & e)
{
	std::cerr << e.what() << '\n';
	MPI_Finalize();
}
catch(BlockWorkerError & e)
{
	std::cerr << e.what() << '\n';
	MPI_Finalize();
}
















