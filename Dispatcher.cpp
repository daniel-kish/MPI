#include "Dispatcher.h"
#include "Block.h"
#include <string>
#include <sstream>



Dispatcher::Dispatcher(int _block_sz, int _blocks_nr, int _edge_sz)
	: block_sz(_block_sz), blocks_nr(_blocks_nr), edge_sz(_edge_sz)
{
	MPI_Comm_size(MPI_COMM_WORLD, &world_sz);
	
	if (world_sz-1 != blocks_nr) // not enough or too many processes
		throw DispatcherError("not enough processes");

}

void Dispatcher::work()
{
	dispatchBlocks();
}

void Dispatcher::dispatchBlocks()
{
	std::vector< std::vector<double> > dataBlocks;
	dataBlocks.push_back(
	  std::vector<double>{ 1,2,9,8,3,9,5,2, 3,3,5,4,3,7,5,5, 9,1,1,3,3,5,4,8, 7,2,3,1,8,1,3,1 }
	);
	dataBlocks.push_back(
	  std::vector<double>{ 3,8,2,2,1,2,3,7, 1,1,5,6,1,8,4,2, 7,1,8,2,6,2,4,2, 9,2,3,9,3,6,2,7 }
	);
	dataBlocks.push_back(
	  std::vector<double>{ 1,2,7,5,7,2,7,2, 2,3,1,4,1,3,0,5, 2,1,8,3,2,4,6,2, 1,3,2,3,1,7,3,6 }
	);

	for(int rank = 1; rank < world_sz; ++rank)
	{
		Block b(block_sz, edge_sz); b.v = dataBlocks[rank-1];
		send_block(b,rank);
	}
	
}


void Dispatcher::send_block(Block const& b, int dest_rank)
{
	MPI_Datatype block_type;
	const int fields_nr = 3;
	
	MPI_Datatype field_types[fields_nr] = { MPI_INT, MPI_INT, MPI_DOUBLE };
	int field_len[fields_nr] = { 1, 1, b.v.size() };
	MPI_Aint disp[fields_nr], addr_rows, addr_cols, addr_data;

	MPI_Get_address(&b.rows, &addr_rows);
	MPI_Get_address(&b.cols, &addr_cols);
	MPI_Get_address(b.v.data(), &addr_data);
	
	disp[0] = 0; 						// rows
	disp[1] = addr_cols - addr_rows;	// cols
	disp[2] = addr_data - addr_rows;	// data [rows x cols] doubles
	
	MPI_Type_create_struct(fields_nr, field_len, disp, field_types, &block_type);
	MPI_Type_commit(&block_type);
	
	MPI_Send(&b.rows, 1, block_type, dest_rank, 0, MPI_COMM_WORLD);
	
	MPI_Type_free(&block_type);
	std::cout << "sent to " << dest_rank << '\n';
}



