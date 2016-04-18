#include <vector>
#include <iostream>
#include <iomanip>
#include "Row.h"

struct Edge
{
	int rows;
	int cols;
	std::vector<double> v;
	std::vector<double> edgeSoln;
	
	Edge(int edgesz, int blocks_num, int blocksz);
	
	// access
	double& operator()(int i, int j);
	double operator()(int i, int j) const;
	double& e(int i, int j);
	
	// soln
	void eliminateCol(Row& r);
	bool elimDone();
	void solveEdgeEqs();
};
