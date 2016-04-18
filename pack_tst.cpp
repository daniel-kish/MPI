#include "mpi.h"
#include <iostream>
#include "Row.h"
#define SIZE 4

int main(int argc, char *argv[])  
{
	int numtasks, rank;
	double a[SIZE][SIZE] =
	  {1.0,  2.0,  3.0,  4.0,
	   5.0,  6.0,  7.0,  8.0,
	   9.0,  10.0, 11.0, 12.0,
	   13.0, 14.0, 15.0, 16.0};

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	if (rank == 0)
	{
		int row_no = 1;
		
		MPI_Aint addr_row_no, addr_row_data, disp[2];
		
		MPI_Get_address(&row_no, &addr_row_no);
		MPI_Get_address(&a[1][1], &addr_row_data);
		disp[0] = (MPI_Aint) 0;
		disp[1] = addr_row_data - addr_row_no;
		
		MPI_Datatype row_type;
		int blocks[2] = {1,SIZE-row_no};
		MPI_Datatype types[2]={MPI_INT, MPI_DOUBLE};
		
		MPI_Type_create_struct(2, blocks, disp, types, &row_type);
		MPI_Type_commit(&row_type);
		
		MPI_Request req;
		MPI_Isend(&row_no, 1, row_type, 1, 0, MPI_COMM_WORLD, &req);
		
		std::cout << "sent and out\n";
		
		MPI_Type_free(&row_type);
	}
	else
	{		
		std::vector<char> buf;
		MPI_Status status;
		
		MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
		
		int elems;
		
		MPI_Get_elements(&status, MPI_PACKED, &elems); // size in bytes
		
		std::cout << "elems " << elems << '\n';
		buf.resize(elems);
		MPI_Recv(buf.data(), buf.size(), MPI_PACKED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		
		int pos=0;
		int row_no=-1;

		MPI_Unpack(buf.data(), buf.size(), &pos, &row_no, 1, MPI_INT, MPI_COMM_WORLD);
		
		std::vector<double> row_data; row_data.resize(SIZE-row_no);
		
		MPI_Unpack(buf.data(), buf.size(), &pos, row_data.data(), row_data.size(), MPI_DOUBLE, MPI_COMM_WORLD);
		

		Row r(std::move(row_data), row_no);
		std::cout << r << '\n';
	}
	
	MPI_Finalize();
}
