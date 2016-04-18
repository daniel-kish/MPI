#include "BlockWorker.h"
#include <string>
#include <sstream>

struct BlockWorkerError{
	std::string msg;
	BlockWorkerError(std::string s) : msg(s)
	{}
	std::string what() {return msg;}
};

BlockWorker::BlockWorker(int _block_sz, int _blocks_nr, int _edge_sz)
	: block_sz(_block_sz), blocks_nr(_blocks_nr), edge_sz(_edge_sz)
{
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_sz);
}

void BlockWorker::work()
{
	Block b = recv_block();
	
	std::ostringstream s;
	s << myrank << ":\n" << b << '\n';
	std::cout.write(s.str().c_str(),s.str().size());
}

Block BlockWorker::recv_block()
{
	MPI_Datatype block_type;
	const int fields_nr = 3;

	Block b(block_sz, edge_sz);
	MPI_Datatype field_types[fields_nr] = { MPI_INT, MPI_INT, MPI_DOUBLE }; // define datatype for block
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

	MPI_Recv(&b.rows, 1, block_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	MPI_Type_free(&block_type);
	
	return b;
}

/*void BlockWorker::send_row(Block const& b, int block_no, int row_no)
{
	assert(row_no >= && row_no < b.cols);
	
	int row_data_sz = b.cols - row_no;
	int g_row_no = block_no*block_sz + b.cols - row_data_sz; // first determine the global row_no
	
}*/
