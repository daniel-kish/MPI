#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <assert.h>

struct block
{
	int rows;
	int cols;
	std::vector<double> v;

	block(int blocksz, int edgesz)
		: rows(blocksz), cols(blocksz + edgesz + 1), v(rows*cols)
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
		for (int i = 0; i < rows; ++i) {
			for (int j = i + 1; j < rows; ++j)
			{
				double f = -e(j, i) / e(i, i);
				add_rows(i, j, f);
			}
		}
	}
	void bwd(std::vector<double> & x)
	{
		eliminateEdge(begin(x), end(x));

		for (int i = rows - 1; i >= 0; --i) // every row
		{
			double s = 0.0;
			for (int j = rows-1; j != i; --j) // cols from end to diagonal
			{
				s += e(i, j)*e(j, cols - 1);
			}
			e(i, cols - 1) -= s;
			e(i, cols - 1) /= e(i, i);
		}
	}
	template <class Iter>
	void eliminateEdge(Iter xb, Iter xe)
	{
		for (int i = rows-1; i >= 0; --i) {
			auto cb = begin(v) + i*cols + rows;
			double scp = std::inner_product(xb, xe, cb, 0.0);
			e(i, cols - 1) -= scp;
		}
	}
	void add_rows(int i, int j, double f)
	{
		for (int m = 0; m < cols; ++m)
		{
			e(j, m) = e(i, m)*f + e(j, m);
		}
	}
	void fill()
	{
		std::mt19937 g{ 5 };
		std::uniform_real_distribution<double> d(1, 5);
		std::generate(begin(v), end(v), [&]() { return d(g); });
	}
};

std::ostream& operator<<(std::ostream& s, block const& b)
{
	int blocksz = b.rows;
	int edgesz = b.cols - b.rows - 1;
	const int w = 8;
	s << std::fixed << std::setprecision(4);
	for (int i = 0; i < b.rows; ++i)
	{
		int j = 0;
		for (; j < blocksz; ++j)
			s << std::setw(w) << b(i, j) << ' ';
		s << std::setw(w) << '|';
		for (; j < b.cols - 1; ++j)
			s << std::setw(w) << b(i, j) << ' ';
		s << std::setw(w) << '|';
		s << std::setw(w) << b(i, j);
		s << std::setw(w) << '\n';
	}
	return s;
}

int main()
{
	std::vector<double> v{ 1,2,9,8,3,9,5,2, 3,3,5,4,3,7,5,5, 9,1,1,3,3,5,4,8, 7,2,3,1,8,1,3,1 };
	block b(4, 3);
	assert(v.size() == b.v.size());
	b.v = v;
	std::cout << b << '\n';
	b.fwd();
	std::cout << b << '\n';

	std::vector<double> x{ -0.781576, 0.704136, -0.608698 };

	b.bwd(x);

	std::cout << b << '\n';
}
