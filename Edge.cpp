#include "Edge.h"

Edge::Edge(int _edge_sz, int _blocks_num, int _block_sz)
	: v( _edge_sz*(_blocks_num*_block_sz + _edge_sz + 1) ), 
	  rows(_edge_sz), cols(_blocks_num*_block_sz + _edge_sz + 1), 
	  block_sz(_block_sz), blocks_nr(_blocks_num), edge_sz(_edge_sz)
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

void Edge::fwd()
{
	int subcols = edge_sz+1;
	int j0 = block_sz*blocks_nr;
		
	for(int p = 0; p < rows-1; ++p)
	{
		for(int r = p+1; r < rows; ++r)
		{
			double f = -e(r,p+j0)/e(p,j0+p);
			for (int k=p; k < subcols; k++)
			{
				e(r,k+j0) = e(r, k+j0) + f*e(p,k+j0);
			}
		}
	}
}

void Edge::bwd()
{
	int j0 = block_sz*blocks_nr;
	edge_soln.resize(edge_sz);
	for (int i = rows - 1; i >= 0; --i) // every row
	{
		double s=0.0;
		for (int j = cols-2; j-j0!=i; --j)
			s += e(i,j)*e(j-j0,cols-1);
		e(i,cols-1) -= s;
		e(i,cols-1) /= e(i,i+j0);
		edge_soln[i] = e(i,cols-1);
	}
}

void Edge::solveEdgeEqs()
{
	fwd();
	bwd();
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




