#include "Edge.h"

Edge::Edge(int _edge_sz, int _blocks_num, int _block_sz)
	: v( _edge_sz*(_blocks_num*_block_sz + _edge_sz + 1) ), 
	  rows(_edge_sz), cols(_blocks_num*_block_sz + _edge_sz + 1), 
	  block_sz(_block_sz), edge_sz(_edge_sz)
{	}

double& Edge::operator()(int i, int j)
{
	return v[i*cols + j];
}

double Edge::operator()(int i, int j) const
{
	return v[i*cols + j];
}

double& Edge::e(int i, int j)
{
	return v[i*cols + j];
}

void Edge::eliminate_col(Row& r)
{
	int elimd_col = r.row_no;
	int block_no = r.row_no / block_sz;
	int block_last_col = block_sz*(block_no+1);
	
	std::cout << "b_no " << block_no << " blc " << block_last_col << '\n';
	
	for(int i = 0; i < rows; ++i)
	{
		double f = -e(i, elimd_col) / r.v[0];
		for (int j = elimd_col; j != block_last_col; ++j)
			e(i,j) = e(i,j) + r.v[j-elimd_col]*f;
			
		int rj = r.sz-1;
		int ej = cols-1;
		int n=0;
		for (; n < edge_sz+1; rj--, ej--, n++)
			e(i,ej) = e(i,ej) + f*r.v[rj];
	}
}

std::ostream& operator<<(std::ostream& s, Edge const& e)
{
	const int w = 6;
	s << std::fixed << std::setprecision(2);
	for (int i = 0; i < e.rows; ++i)
	{
		for (int j = 0; j < e.cols; ++j)
			std::cout << std::setw(w) << e(i,j) << ' ';
		std::cout << '\n';
	}
	return s;
}




