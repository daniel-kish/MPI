#ifndef EDGE_H
#define EDGE_H

#include <vector>
#include <iostream>
#include <iomanip>
#include "Row.h"

struct Edge
{
	std::vector<double> v;
	int rows;
	int cols;
	std::vector<double> edgeSoln;
	int block_sz;
	int edge_sz;
	
	Edge(int edgesz, int blocks_num, int blocksz);
	
	// access
	double& operator()(int i, int j);
	double operator()(int i, int j) const;
	double& e(int i, int j);
	
	// soln
	void eliminate_col(Row& r);
	bool elimDone();
	void solveEdgeEqs();
};

std::ostream& operator<<(std::ostream& s, Edge const& b);

#endif // EDGE_H
