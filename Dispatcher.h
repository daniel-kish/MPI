#include <mpi.h>
#include "Block.h"
enum tag { block_tag };

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
};
