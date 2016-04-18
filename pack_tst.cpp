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
{
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
/*		MPI_Datatype block_type;
		const int fields_nr = 3;

		Block b(block_sz, edge_sz);
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

		MPI_Recv(&b.rows, 1, block_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

		MPI_Type_free(&block_type);
		std::ostringstream s;
		s << myrank << ":\n" << b << '\n';
		std::cout.write(s.str().c_str(),s.str().size());
*/	}
	
	MPI_Finalize();
}


















