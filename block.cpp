#include "block.h"


Block::Block(int blocksz, int edgesz)
		: rows(blocksz), cols(blocksz + edgesz + 1), v(rows*cols)
{}

double& Block::operator()(int i, int j)
{
	return v[i*cols + j];
}

double Block::operator()(int i, int j) const
{
	return v[i*cols + j];
}

double& Block::e(int i, int j)
{
	return v[i*cols + j];
}

void Block::fwd(int i)
{
	assert(i >= 0 && i < rows);
	for (int j = i + 1; j < rows; ++j)
	{
		double f = -e(j, i) / e(i, i);
		add_rows(i, j, f);
	}
}

void Block::bwd(std::vector<double> & x)
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

void Block::eliminateEdge(iter xb, iter xe)
{
	for (int i = rows-1; i >= 0; --i) {
		auto cb = begin(v) + i*cols + rows;
		double scp = std::inner_product(xb, xe, cb, 0.0);
		e(i, cols - 1) -= scp;
	}
}

void Block::add_rows(int i, int j, double f)
{
	for (int m = 0; m < cols; ++m)
	{
		e(j, m) = e(i, m)*f + e(j, m);
	}
}

void Block::fill()
{
	std::mt19937 g{ 5 };
	std::uniform_real_distribution<double> d(1, 5);
	std::generate(begin(v), end(v), [&]() { return d(g); });
}

std::ostream& operator<<(std::ostream& s, Block const& b)
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
