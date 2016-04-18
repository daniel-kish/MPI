#ifndef BLOCK_H
#define BLOCK_H
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <assert.h>

typedef std::vector<double>::iterator iter;
struct Block
{
	int rows;
	int cols;
	std::vector<double> v;

	Block(int blocksz, int edgesz);
	double& operator()(int i, int j);
	double operator()(int i, int j) const;
	double& e(int i, int j);
	void fwd(int);
	void bwd(std::vector<double> & x);

	void eliminateEdge(iter xb, iter xe);
	void add_rows(int i, int j, double f);
	void fill();
};

std::ostream& operator<<(std::ostream& s, Block const& b);


#endif //BLOCK_H
