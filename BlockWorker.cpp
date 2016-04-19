#include "BlockWorker.h"
#include <string>
#include <sstream>

BlockWorker::BlockWorker(int _block_sz, int _blocks_nr, int _edge_sz)
	: block_sz(_block_sz), blocks_nr(_blocks_nr), edge_sz(_edge_sz)
{
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_sz);
	
/*	if (world_sz-1 != blocks_nr) // not enough or too many processes
		throw BlockWorkerError("not enough processes");
		*/
}

void BlockWorker::work()
{
	Block b = recv_block();

	MPI_Request* reqs = new MPI_Request[b.rows];

	for(int i = 0; i < b.rows; ++i)
	{
		send_row(b,i,reqs+i);
		b.fwd(i);
	}
	
	MPI_Waitall(b.rows, reqs, MPI_STATUS_IGNORE);
}

Block BlockWorker::recv_block()
{
/*	MPI_Datatype block_type;
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
	
*/
	Block b(block_sz, edge_sz);
	MPI_Recv(b.v.data(), b.v.size(), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

	return b;
}

void BlockWorker::send_row(Block & b, int row_no, MPI_Request* req)
{
	int block_no = myrank-1;
	int row_data_sz = b.cols - row_no;
//	int g_row_no = block_no*block_sz + row_no; // first determine the global row_no
	MPI_Datatype row_type;
	const int fields_nr = 2;


	MPI_Datatype field_types[fields_nr] = { MPI_INT, MPI_DOUBLE }; // define datatype for row
	int field_len[fields_nr] = { 1, row_data_sz };
	MPI_Aint disp[fields_nr], addr_g_row_no, addr_row_data;

	MPI_Get_address(&row_no, &addr_g_row_no);
	double& v=b(row_no,row_no);
	MPI_Get_address(&v, &addr_row_data);

	disp[0] = 0; 								// g_row_no
	disp[1] = addr_row_data - addr_g_row_no;	// data [row_data_sz] doubles

	MPI_Type_create_struct(fields_nr, field_len, disp, field_types, &row_type);
	MPI_Type_commit(&row_type);

	MPI_Isend(&row_no, 1, row_type, 0, 1, MPI_COMM_WORLD, req);

	MPI_Type_free(&row_type);
//	std::cout << myrank << " sent " << g_row_no << ':' << row_data_sz << '\n';
}




