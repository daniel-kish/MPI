#include "test.h"
//#include <random>
#include <algorithm>

std::vector<double> mult(std::vector<Block> const& bs,
						 Edge const& e, std::vector<double> x)
{
	int b_rows = bs[0].rows;
	int e_rows = e.rows;
	int bs_nr = bs.size();
	
	std::vector<double> res(x.size());
	
	for (int b=0; b < bs_nr; ++b)
	{
		int b_beg = b*b_rows;
		int b_end = (b+1)*b_rows;
		
		for (int r=0; r < b_rows; ++r)
		{
			double s=0.0;
			for (int gc = b_beg; gc != b_end; ++gc)
				s += bs[b](r, gc-b_beg) * x[gc];
		
			int e_end = bs[b].cols-2;
			int times = e.rows;

			for(int i = e_end, j=x.size()-1; times > 0; times--,i--,j--)
				s += bs[b](r,i) * x[j];
			res[r + b_rows*b] = s;
		}
	}
	
	int i = res.size() - e.rows;
	for (int r = 0; r < e.rows; ++r)
	{
		double s=0.0;
		for (int c = 0; c < x.size(); ++c)
		{
			s += e(r,c) * x[c];
		}
		res[i] = s;
		i++;
	}
	
	
	return res;
}

double rand_d()
{
  return ((double)rand()/(double)RAND_MAX);  
}

void fill(std::vector<double>& v)
{
	std::generate(v.begin(), v.end(), rand_d);
}
