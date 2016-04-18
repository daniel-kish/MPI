#include <mpi.h>
#include <iostream>
#include <vector>
#include "block.h"
#include "Row.h"
#include <queue>

int get_row_no(int block_no, int block_sz, int cols, int row_len)
{
	return block_no*block_sz + cols - row_len;
}


enum tag { block_tag, block_row_tag, edge_sol_tag, block_sol_tag };

int main(int argc, char** argv)
{
	MPI_Init(&argc,&argv);
		
	int myrank;
	MPI_Comm_rank (MPI_COMM_WORLD, &myrank);
	
	
	if(myrank == 0){
		
		// sending block
		std::vector<double> v{ 1,2,9,8,3,9,5,2, 3,3,5,4,3,7,5,5, 9,1,1,3,3,5,4,8, 7,2,3,1,8,1,3,1 };
		Block b(4,3);
		b.v = v;
		assert(v.size() == b.v.size());

		MPI_Send (b.v.data(), b.v.size(), MPI_DOUBLE, 1, block_tag, MPI_COMM_WORLD);
		
		
		std::priority_queue<Row> q;
		
		int row_no;
		// receiving first row
		MPI_Status status;
		int row_len;
		MPI_Probe(1, block_row_tag, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_DOUBLE, &row_len);
	
		std::vector<double> row(row_len, -1.0);
		MPI_Recv(row.data(), row_len, MPI_DOUBLE, 1, block_row_tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		row_no = get_row_no(0, 4, b.cols, row_len);
		
		q.push( Row(std::move(row), row_no) );
		
		std::cout << "got row# " << q.top().row_no << '\n';
		for(int i=0; i < q.top().data.size(); ++i)
			std::cout << q.top().data[i] << ' ';
		std::cout << '\n';
		q.pop();
		
		//receiving second row
		MPI_Probe(1, block_row_tag, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_DOUBLE, &row_len);
		row.resize(row_len);
		MPI_Recv(row.data(), row_len, MPI_DOUBLE, 1, block_row_tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		row_no = get_row_no(0, 4, b.cols, row_len);
		q.push( Row(std::move(row), row_no) );
		std::cout << "got row# " << q.top().row_no << '\n';
		for(int i=0; i < q.top().data.size(); ++i)
			std::cout << q.top().data[i] << ' ';
		std::cout << '\n';
	}
	else {
		MPI_Status status;
		int count;
		MPI_Probe(0, block_tag, MPI_COMM_WORLD, &status);
		MPI_Get_count(&status, MPI_DOUBLE, &count);
	
		
		Block b(4,3);
		std::vector<double> v(count);
		MPI_Recv(v.data(), count, MPI_DOUBLE, 0, block_tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		b.v = v;
		MPI_Request req;
		
		MPI_Isend (b.v.data(), b.cols, MPI_DOUBLE, 0, block_row_tag, MPI_COMM_WORLD,
			&req);		
		b.fwd(0);
		//std::cout << "proc #" << myrank << '\n' << b << '\n';


		MPI_Isend (b.v.data()+b.cols+1, b.cols-1, MPI_DOUBLE, 0, block_row_tag, MPI_COMM_WORLD, &req);		
		b.fwd(1);
		//std::cout << "proc #" << myrank << '\n' << b << '\n';
		
		MPI_Wait(&req,MPI_STATUS_IGNORE);
	}

	MPI_Finalize();
}
