#ifndef BLOCKWORKER_H
#define BLOCKWORKER_H


#include <mpi.h>
#include "Block.h"

struct BlockWorkerError{
	std::string msg;
	BlockWorkerError(std::string s) : msg(s)
	{}
	std::string what() {return msg;}
};


struct BlockWorker
{
	int block_sz;
	int blocks_nr;
	int edge_sz;
	int myrank;
	int world_sz;

	double time;

	BlockWorker(int block_sz, int blocks_nr, int edge_sz);
	void work();
	Block recv_block();
	void send_row(Block &,int/*,MPI_Request**/);
	std::vector<double> recv_edge_sol();
	void send_block_sol(Block&);
};

#endif //BLOCKWORKER
