#include <vector>
#include <iostream>

struct Row {
	std::vector<double> v;
	int row_no;
	Row(std::vector<double>&& rowdata, int _row_no);
};


bool operator<(Row const& lhs, Row const& rhs);
std::ostream& operator<< (std::ostream& s, Row const& r);
