#include "Row.h"

Row::Row(std::vector<double>&& rowdata, int _row_no)
		: v(std::move(rowdata)), row_no(_row_no)
{	}


bool operator<(Row const& lhs, Row const& rhs)
{
	return lhs.row_no > rhs.row_no;
}

std::ostream& operator<< (std::ostream& s, Row const& r)
{
	s << r.row_no << ": ";
	
	for(const auto& e : r.v)
		s << e << ' ';
		
	return s;
}
