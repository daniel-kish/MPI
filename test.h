#include <vector>
#include "Block.h"
#include "Edge.h"

std::vector<double> mult(std::vector<Block> const& bs,
						 Edge const& e, std::vector<double> x);
double rand_d();						 
void fill(std::vector<double>& v);
