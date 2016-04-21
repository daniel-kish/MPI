#include <iostream>
#include "Block.h"
#include "Edge.h"
#include "test.h"

int main(int argc, char** argv)
{
	int block_sz = atoi(argv[1]);
	int blocks_nr = atoi(argv[2]);
	int edge_sz = atoi(argv[3]);
	
/*	std::vector<double> b1{ 1,2,9,8,3,9,5,2, 3,3,5,4,3,7,5,5, 9,1,1,3,3,5,4,8, 7,2,3,1,8,1,3,1 };
	std::vector<double> b2{ 3,8,2,2,1,2,3,7, 1,1,5,6,1,8,4,2, 7,1,8,2,6,2,4,2, 9,2,3,9,3,6,2,7 };
	std::vector<double> b3{ 1,2,7,5,7,2,7,2, 2,3,1,4,1,3,0,5, 2,1,8,3,2,4,6,2, 1,3,2,3,1,7,3,6 };
	std::vector<double> edata {
		 1, 2, 2, 4, 6, 7, 1, 4, 8, 4, 8, 5, 3, 6, 8, 4,
		 2, 3, 8, 2, 6, 1, 6, 9, 4, 4, 7, 3, 9, 7, 0, 3,
		 4, 3, 0, 7, 0, 5, 4, 7, 4, 8, 7, 5, 8, 7, 1, 7  };
*/
	std::vector<Block> blocks(blocks_nr, Block(block_sz,edge_sz));
//	blocks[0].v=b1; blocks[1].v=b2; blocks[2].v=b3;
	
	for(int i=0; i < blocks.size(); ++i)
		blocks[i].fill();
	
	Edge e(edge_sz,blocks_nr,block_sz);
//	e.v = edata;
	e.fill();
	
	std::vector<double> x(block_sz*blocks_nr+edge_sz); 
	//= {0.91664053, 1.3855379, -0.25781433, -0.039548852, 0.96574954,
					         //0.74537883, 0.14520541, -0.47564557, -2.0941189, -0.60557768,
						     //0.26717363, 2.3517415, -0.78157613, 0.70413586, -0.60869818};
	fill(x);
//	for(int i=0; i < blocks.size(); ++i)
//		std::cout << blocks[i] << '\n';

//	std::cout << e; 

	std::vector<double> v = mult(blocks,e,x);
	for(int i=0; i < v.size(); ++i)
		std::cout << v[i] << '\n';
}


















