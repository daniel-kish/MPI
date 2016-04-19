#include "Row.h"
#include <algorithm>


Row::Row()
		: v(0), sz(0), row_no(-1)
{	}


Row::Row(int _sz, int _row_no)
		: v(new double[_sz]), sz(_sz), row_no(_row_no)
{	}

// move only type - destructive read XXX
Row::Row(Row const& rhs) : v(rhs.v), sz(rhs.sz), row_no(rhs.row_no)
{
	rhs.v = 0;
	rhs.sz = 0;
	rhs.row_no = -1;
}

Row& Row::operator=(Row const& rhs) // move only type - destructive read XXX
{
	std::swap(v,rhs.v);
	std::swap(sz,rhs.sz);
	std::swap(row_no,rhs.row_no);
}

Row::~Row() 
{
	row_no = -1;
	sz = 0;
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
