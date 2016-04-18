#include <vector>
#include <iostream>

struct Row {
	double* v;
	int row_no;
	int sz;
	Row(double* rowdata, int _row_no, int cols);
	~Row();
};


bool operator<(Row const& lhs, Row const& rhs);
std::ostream& operator<< (std::ostream& s, Row const& r);
