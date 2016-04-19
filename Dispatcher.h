#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <mpi.h>
#include "Block.h"
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


	Dispatcher(int block_sz, int blocks_nr, int edge_sz);
	void work();
	void dispatchBlocks();
	void send_block(Block const&,int);
	Row recv_row();
};

#endif //DISPATCHER_H
