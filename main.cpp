#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>

struct block{
	int rows;
	int cols;
	std::vector<double> v;

	block(int blocksz, int edgesz)
		: rows(blocksz), cols(blocksz+edgesz+1), v(rows*cols)
	{}
	double& operator()(int i, int j)
	{
	  return v[i*cols + j];
	}
	double operator()(int i, int j) const
	{
	  return v[i*cols + j];
	}
	double& e(int i, int j)
	{
	  return v[i*cols + j];
	}
	void fwd()
	{
	  for(int i=0; i < rows; ++i){
	    for(int j=i+1; j < rows; ++j)
	    {
	      double f = -e(j,i)/ e(i,i);
	      add_rows(i,j,f);
	    }	  
	  }
	}
	void add_rows(int i, int j, double f)
	{
	  for (int m=0; m < cols; ++m)
	  {
	    e(j,m) = e(i,m)*f + e(j,m);
	  }
	}
	void fill()
	{
	  std::mt19937 g;
	  std::uniform_int_distribution<int> d(1,5);
	  std::generate(begin(v), end(v),[&](){ return d(g); });	  
	}
};

std::ostream& operator<<(std::ostream& s, block const& b)
{
  int blocksz = b.rows;
  int edgesz = b.cols - b.rows - 1;
  const int w = 9;
  for(int i=0; i < b.rows; ++i)
  {
    int j=0;
    for(; j < blocksz; ++j)
      s << std::setw(w) << b(i,j) << ' ';
    s << std::setw(w) << '|';
    for(; j < b.cols-1; ++j)
      s << std::setw(w) << b(i,j) << ' ';
    s << std::setw(w) << '|';
    s << std::setw(w) << b(i,j);
    s << std::setw(w) << '\n';
  }
  return s;
}

int main()
{
  block b(4,3);
  b.fill();
  std::cout << b << '\n';
  b.fwd();
  std::cout << b << '\n';
}
