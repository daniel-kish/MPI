#include <mpi.h>
#ifndef DISPATCHER_H
#define DISPATCHER_H

#include "Block.h"
#include "Edge.h"
#include "Row.h"

struct DispatcherError{
	std::string msg;
	DispatcherError(std::string s) : msg(s)
	{}
	std::string what() {return msg;}
};


struct Dispatcher
{
	int block_sz;
	int blocks_nr;
	int edge_sz;
	int world_sz;
	Edge edge;
	Edge edgeInit;
	std::vector<Block> bs;
	std::vector<double> soln;
	std::vector<double> Rhs;
	double time;

	Dispatcher(int block_sz, int blocks_nr, int edge_sz);
	void work();
	void dispatchBlocks();
	void send_block(Block &,int);
	void elim_row(Row&, int&);
	void send_edge_sol();
	void recv_block_sols();
	Row recv_row();
};

#endif //DISPATCHER_H
