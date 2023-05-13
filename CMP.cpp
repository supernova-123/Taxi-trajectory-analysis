#include <iostream>
#include "taxi_header_use.h"

//排序依据:车名权重最大, 其次考虑时间
bool CMP::operator() (Point a, Point b)const
{
	if (a.gettaxiname() != b.gettaxiname())
		return bool(a.gettaxiname() < b.gettaxiname());
	else {
		int i1 = a.getsecond() + a.getminute() * 60 + a.gethour() * 60 * 60 + a.getday() * 24 * 60 * 60;
		int i2 = b.getsecond() + b.getminute() * 60 + b.gethour() * 60 * 60 + b.getday() * 24 * 60 * 60;
		return bool(i1 < i2);
	}
}