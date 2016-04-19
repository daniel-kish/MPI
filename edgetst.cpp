#include <iostream>
#include "Edge.h"
#include "Row.h"

void fill_row(Row & r)
{
	std::vector<double> data{ 1,2,9,8,3,9,5,2 };
//	std::vector<double> data{ 8,2,6,2,4 };
	for(int i=0; i < data.size(); ++i)
		r.v[i] = data[i];
}

int main()
{
	std::vector<double> edata {
		 1, 2, 2, 4, 6, 7, 1, 4, 8, 4, 8, 5, 3, 6, 8, 4,
		 2, 3, 8, 2, 6, 1, 6, 9, 4, 4, 7, 3, 9, 7, 0, 3,
		 4, 3, 0, 7, 0, 5, 4, 7, 4, 8, 7, 5, 8, 7, 1, 7  };
	Edge e(3,3,4);
	e.v = edata;
	
	Row r(8,0);
	fill_row(r);

	e.eliminate_col(r);
	
	std::cout << r << '\n';
	std::cout << e << '\n';
}
