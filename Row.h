#ifndef ROW_H
#define ROW_H

#include <vector>
#include <iostream>

struct Row {
	mutable double* v;
	mutable int sz;
	mutable int row_no;
	
	Row();
	Row(int cols, int _row_no);
	Row(Row const&); //copy ctor
	Row& operator=(Row const&); //copy assignment
	~Row();
};


bool operator<(Row const& lhs, Row const& rhs);
std::ostream& operator<< (std::ostream& s, Row const& r);

#endif // ROW_H
