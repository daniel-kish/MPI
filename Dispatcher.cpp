#include "Dispatcher.h"
#include "Block.h"
#include "Edge.h"
#include "Row.h"
#include "test.h"
#include <string>
#include <sstream>
#include <queue>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>


Dispatcher::Dispatcher(int _block_sz, int _blocks_nr, int _edge_sz)
	: block_sz(_block_sz), blocks_nr(_blocks_nr), edge_sz(_edge_sz),
	  edge(_edge_sz, _blocks_nr, block_sz), edgeInit(edge)
{
	MPI_Comm_size(MPI_COMM_WORLD, &world_sz);
	if (world_sz-1 != blocks_nr) // not enough or too many processes
		throw DispatcherError("not enough processes");
	edge.fill();
	edgeInit = edge;
}

void Dispatcher::elim_row(Row & r, int& col_tobe_elimd)
{
	clock_t st = clock();
	edge.eliminate_col(r);
	col_tobe_elimd++;
	clock_t fin = clock();
	time += double((fin - st)) / CLOCKS_PER_SEC;
}

void Dispatcher::work()
{
	dispatchBlocks();
	std::cout << "dispatched blocks\n";
	std::priority_queue<Row> q;
	
	int rows_nr = blocks_nr * block_sz;
	int col_tobe_elimd = 0;
	int total_recved = 0;

	time = 0.0;
	clock_t start, end;

	while(col_tobe_elimd < rows_nr) // elimn is not over
	{
		while(!q.empty() && q.top().row_no == col_tobe_elimd) {  // needed row is in queue
			Row r = q.top(); q.pop();
			elim_row(r,col_tobe_elimd);
		}
		if (total_recved < rows_nr)	// not all rows are recved yet
		{
			Row r = recv_row();
			total_recved++;
			if (r.row_no == col_tobe_elimd) { // immediate usage
				elim_row(r,col_tobe_elimd);
			}
			else	// keep for future
				q.push(r);
		}
	}


	// just in case
	if (!q.empty()) {
		std::cerr << "fatal error: q not empty\n";
		std::terminate();
	}
	std::cout << "recved rows\n";

	start = clock();
	edge.fwd();
	edge.bwd();
	end = clock();
	time += double((end - start)) / CLOCKS_PER_SEC;

	
	std::cout << "solved edge\n";

	send_edge_sol();
	
	std::cout << "sent edge sol\n";
	
	start = clock();
	recv_block_sols();
	end = clock();
	time += double((end - start)) / CLOCKS_PER_SEC;
	
	std::cout << "elapsed " << time << '\n';
	//std::cout << "recved block solns\n";
		
	std::cout << "\n\n";
	std::vector<double> rhs = mult(bs, edgeInit, soln);
	
	double residue=0.0;
	for (int i=0; i < rhs.size(); ++i)
		residue += fabs(rhs[i] - Rhs[i]);
	std::cout << residue << '\n';
}

void Dispatcher::recv_block_sols()
{
	soln.resize(block_sz*blocks_nr + edge_sz);

	for (int rank = 1; rank < world_sz; rank++)
	{
		MPI_Recv(soln.data()+(rank-1)*block_sz, block_sz, MPI_DOUBLE,
		 rank, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}
	
	int last = blocks_nr*block_sz;
	for (int i=last; i < soln.size(); ++i)
		soln[i] = edge.edge_soln[i-last];
}

void Dispatcher::dispatchBlocks()
{
	bs.resize(blocks_nr);
	fill(bs.begin(), bs.end(), Block(block_sz, edge_sz));
/*	std::vector< std::vector<double> > dataBlocks;
	dataBlocks.push_back(
	  std::vector<double>{ 1,2,9,8,3,9,5,2, 3,3,5,4,3,7,5,5, 9,1,1,3,3,5,4,8, 7,2,3,1,8,1,3,1 }
	);
	dataBlocks.push_back(
	  std::vector<double>{ 3,8,2,2,1,2,3,7, 1,1,5,6,1,8,4,2, 7,1,8,2,6,2,4,2, 9,2,3,9,3,6,2,7 }
	);
	dataBlocks.push_back(
	  std::vector<double>{ 1,2,7,5,7,2,7,2, 2,3,1,4,1,3,0,5, 2,1,8,3,2,4,6,2, 1,3,2,3,1,7,3,6 }
	);
*/
	for(int rank = 1; rank < world_sz; ++rank)
	{
		bs[rank-1].fill();
		send_block(bs[rank-1],rank);
	}
	
	for(int i = 0; i < bs.size(); ++i)
	{
		for(int r=0; r < bs[i].rows; ++r)
			Rhs.push_back(bs[i](r,bs[i].cols-1));
	}
	
	for(int i = 0; i < edge.rows; ++i)
	{
		Rhs.push_back(edge(i,edge.cols-1));
	}
}


void Dispatcher::send_block(Block & b, int dest_rank)
{
/*	MPI_Datatype block_type;
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
*/
	MPI_Send(b.v.data(), b.v.size(), MPI_DOUBLE, dest_rank, 0, MPI_COMM_WORLD);
}

// FIXME TODO: make it asynchronous
Row Dispatcher::recv_row()
{
	MPI_Status status;
	MPI_Probe(MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
	int pack_sz;
	int src_rank = status.MPI_SOURCE;
	
	MPI_Get_count(&status, MPI_PACKED, &pack_sz);
	
//	std::cout <<"probe: from "<< src_rank <<' '<< pack_sz << '\n';
	
	char * b = new char[pack_sz];
	
	if(b == 0)
	{
		throw DispatcherError("not enough memory\n");
	}
	
	MPI_Recv(b,pack_sz, MPI_PACKED, src_rank, 1, MPI_COMM_WORLD, &status);
	
//	std::cout <<"recved\n";
	
	int row_no;
	int pos = 0;
	MPI_Unpack(b, pack_sz, &pos, &row_no, 1, MPI_INT, MPI_COMM_WORLD);
	
//	std::cout << "got " << row_no << ':';
	
	int g_row_no = (src_rank-1)*block_sz + row_no;
	int row_data_sz = block_sz + edge_sz + 1 - row_no;
	Row r(row_data_sz, g_row_no);
	
	if (r.v == 0)
		throw DispatcherError("not enough memory for Row\n"); 
	
	MPI_Unpack(b, pack_sz, &pos, r.v, r.sz, MPI_DOUBLE, MPI_COMM_WORLD);
	
	delete [] b;
//	std::cout << r.sz << " from " << src_rank << '\n';
	return r;


/*	MPI_Status status;
	MPI_Probe(MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
	int pack_sz;
	int src_rank = status.MPI_SOURCE;
	MPI_Get_elements(&status, MPI_PACKED, &pack_sz);
	int row_data_sz = (pack_sz - sizeof(int)) / sizeof(double);

	std::cout << "got pack sz=" << row_data_sz <<" from " << src_rank << '\n';  
	Row r(row_data_sz,-1);
	int row_no;

	MPI_Datatype row_type;
	const int fields_nr = 2;
	MPI_Datatype field_types[fields_nr] = { MPI_INT, MPI_DOUBLE }; // define datatype for row
	int field_len[fields_nr] = { 1, row_data_sz };
	MPI_Aint disp[fields_nr], addr_g_row_no, addr_row_data;

	MPI_Get_address(&row_no, &addr_g_row_no);
	MPI_Get_address(r.v, &addr_row_data);

	disp[0] = 0; 								// row_no
	disp[1] = addr_row_data - addr_g_row_no;	// data [row_data_sz] doubles

	MPI_Type_create_struct(fields_nr, field_len, disp, field_types, &row_type);
	MPI_Type_commit(&row_type);

	int errcode = MPI_Recv(&row_no, 1, row_type, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);

	r.row_no = (src_rank-1)*block_sz + row_no;

	std::cout << "errcode " << errcode << '\n';

	MPI_Type_free(&row_type);
	
	return r;
*/
}

void Dispatcher::send_edge_sol()
{
	MPI_Bcast(edge.edge_soln.data(), edge.edge_soln.size(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
}









