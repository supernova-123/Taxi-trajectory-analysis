#include <iostream>
#include "taxi_header_use.h"
using namespace std;

//序列化函数定义,只需要将所有类的成员信息保存到ar&中,ar的类型是模版,具体根据实际类成员类型决定
//template<class Archive>
//void Point::serialize(Archive& ar, const unsigned int version) {
//	ar& taxiname;
//	ar& times;
//	ar& Longitude;
//	ar& Latitude;
//	}

//Point的无参构造函数:
Point::Point(): taxiname(0), times(), Longitude(0), Latitude(0) {}

//Point的初始化函数,此只用于序列化时对数据集分析结果的处理,以后直接反序列化即可,不会再用:
void Point::Point_init(int i, string& s1, string& s2, string& s3)
{
	taxiname = i;
	times[0] = (s1[9] - '0');
	times[1] = stoi(s1.substr(11, 2));
	times[2] = stoi(s1.substr(14, 2));
	times[3] = stoi(s1.substr(17, 2));
	Longitude = stod(s2);
	Latitude = stod(s3);
}

//下面为Point成员信息的返回函数
int Point::gettaxiname() { return taxiname; }
double Point::getLongitude() { return Longitude; }
double Point::getLatitude() { return Latitude; }
int Point::getday() { return times[0]; }
int Point::gethour() { return times[1]; }
int Point::getminute() { return times[2]; }
int Point::getsecond() { return times[3]; }
string Point::gettimes()
{
	return to_string(times[0]) + ":" + to_string(times[1]) + ":" + to_string(times[2]) + ":" + to_string(times[3]);
}

//Point信息输出
void Point::printPoint()
{
	cout << "Point:" << endl;
	cout << taxiname << " " << Longitude << " " << Latitude << " " << gettimes() << endl;
}

//出租车轨迹点的比较,利用时间和出租车序号名唯一确定一个点,直接比较即可
bool Point::operator== (const Point& p) const
{
	if (p.taxiname == taxiname and p.times[0] == times[0] and p.times[1] == times[1] and p.times[2] == times[2] and p.times[3] == times[3])
		return true;
	else
		return false;
}