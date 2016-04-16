#include <iostream>
#include "block.h"

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
