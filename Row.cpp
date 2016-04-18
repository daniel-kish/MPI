#include "Row.h"

Row::Row(double* rowdata, int _row_no, int cols)
		: v(rowdata), row_no(_row_no), sz(cols-row_no)
{	}

Row::~Row()
{
	delete [] v;
}

bool operator<(Row const& lhs, Row const& rhs)
{
	return lhs.row_no > rhs.row_no;
}

std::ostream& operator<< (std::ostream& s, Row const& r)
{
	s << r.row_no << ": ";
	
	for(int i=0; i < r.sz; ++i)
		s << r.v[i] << ' ';
		
	return s;
}
