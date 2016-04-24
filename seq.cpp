#include <iostream>
#include <cstdlib>
#include "Block.h"
#include "Edge.h"
#include "test.h"

struct RowNO // not owning row class
{
	double * v;
	int sz;
	int row_no;
};

std::ostream& operator<<(std::ostream& os, RowNO& r)
{
	os << r.row_no << ':';
	for (int i=0; i < r.sz; ++i)
		os << r.v[i] << ' ';
	return os;
}

void elim_col(Edge& e, RowNO& r)
{
	int elimd_col = r.row_no;
	int block_no = r.row_no / e.block_sz;
	int block_last_col = e.block_sz*(block_no+1);
	
	for(int i = 0; i < e.rows; ++i)
	{
		double f = -e(i, elimd_col) / r.v[0];
		for (int j = elimd_col; j != block_last_col; ++j)
			e(i,j) = e(i,j) + r.v[j-elimd_col]*f;
			
		int rj = r.sz-1;
		int ej = e.cols-1;
		int n=0;
		for (; n < e.edge_sz+1; rj--, ej--, n++)
			e(i,ej) = e(i,ej) + f*r.v[rj];
	}
}

int main(int argc, char**argv)
{
	int block_sz = atoi(argv[1]);
	int blocks_nr = atoi(argv[2]);
	int edge_sz = atoi(argv[3]);
	
	std::vector<Block> bs(blocks_nr, Block(block_sz,edge_sz) );
	Edge e(edge_sz,blocks_nr,block_sz); 
	
	for (int i=0; i < bs.size(); ++i)
		bs[i].fill();
	e.fill();

//    for (int i=0; i < bs.size(); ++i)
//		std::cout << bs[i] << '\n';
//	std::cout << e << '\n';

    std::vector<Block> bs0 = bs;
	Edge e0 = e; 

	for (int i=0; i < bs.size(); ++i)
		bs[i].fwd();

	for (int b=0; b < bs.size(); ++b)
	{
		for (int br=0; br < bs[b].rows; ++br)
		{
		    double& diag = bs[b](br,br);
			RowNO r;
			r.v = &diag;
			r.sz = bs[b].cols-br;
			r.row_no = bs[b].rows*b + br;
			
            elim_col(e,r);
		}
	}
	e.fwd();
	e.bwd();
    
    for (int i=0; i < bs.size(); ++i){
		bs[i].bwd(e.edge_soln);
//		for (int k=0; k < bs[i].sol.size(); ++k)
//		    std::cout << bs[i].sol[k] << '\n';
//		std::cout << '\n';
	}
//    std::cout << e << '\n'; 
    
    std::vector<double> soln(bs[0].rows*bs.size() + edge_sz);

	for (int i=0; i < bs.size(); ++i)
		for (int k=0; k < bs[i].sol.size(); ++k)
		    soln[i*block_sz+k] = bs[i].sol[k];

	for (int k=0; k < e.edge_soln.size(); ++k)
	    soln[blocks_nr*block_sz+k] = e.edge_soln[k];
	
//	for (int k=0; k < soln.size(); ++k)
//	    std::cout << soln[k] << '\n';
	
	std::vector<double> Rhs;
	
	for(int i = 0; i < bs0.size(); ++i)
	{
		for(int r=0; r < bs0[i].rows; ++r)
			Rhs.push_back(bs0[i](r,bs0[i].cols-1));
	}
	
	for(int i = 0; i < e0.rows; ++i)
	{
		Rhs.push_back(e0(i,e0.cols-1));
	}
	
	std::vector<double> rhs = mult(bs0,e0,soln);
	
	
	double residue=0.0;
	for (int i=0; i < rhs.size(); ++i)
		residue += std::abs(rhs[i] - Rhs[i]);
	std::cout << "\n\nres = " << residue << '\n';
}


















