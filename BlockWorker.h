#include <mpi.h>
#include "Block.h"


struct BlockWorker
{
	int block_sz;
	int blocks_nr;
	int edge_sz;
	int myrank;
	int world_sz;


	BlockWorker(int block_sz, int blocks_nr, int edge_sz);
	void work();
	Block recv_block();
};
