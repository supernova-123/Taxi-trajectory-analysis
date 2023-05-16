#include <iostream>
#include "taxi_header_use.h"
using namespace std;

//���л���������,ֻ��Ҫ��������ĳ�Ա��Ϣ���浽ar&��,ar��������ģ��,�������ʵ�����Ա���;���
//template<class Archive>
//void Point::serialize(Archive& ar, const unsigned int version) {
//	ar& taxiname;
//	ar& times;
//	ar& Longitude;
//	ar& Latitude;
//	}

//Point���޲ι��캯��:
Point::Point(): taxiname(0), times(), Longitude(0), Latitude(0) {}

//�вι��캯��:
//�������Ϊ���⳵�����,��,ʱ,��,��,����,γ��
Point::Point(int i1,int i2,int i3,int i4,int i5,double d1,double d2):
	taxiname(i1),Longitude(d1),Latitude(d2)
{
	times[0] = i2;
	times[1] = i3;
	times[2] = i4;
	times[3] = i5;
}

//Point�ĳ�ʼ������,��ֻ�������л�ʱ�����ݼ���������Ĵ���,�Ժ�ֱ�ӷ����л�����,��������:
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

//Point����һ����ʼ������, �������ļ���д���е���Ϣʱ, ʹ��Ƶ��
//�������Ϊ���⳵�����,��,ʱ,��,��,����,γ��
void Point::Point_init2(int i1, int i2, int i3, int i4, int i5, double d1, double d2)
{
	taxiname = i1;
	times[0] = i2;
	times[1] = i3;
	times[2] = i4;
	times[3] = i5;
	Longitude = d1;
	Latitude = d2;
	return;
}

//����ΪPoint��Ա��Ϣ�ķ��غ���
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

//Point��Ϣ���
void Point::printPoint()
{
	cout << "Point:" << endl;
	cout << taxiname << " " << Longitude << " " << Latitude << " " << gettimes() << endl;
}

//���⳵�켣��ıȽ�,����ʱ��ͳ��⳵�����Ψһȷ��һ����,ֱ�ӱȽϼ���
bool Point::operator== (const Point& p) const
{
	if (p.taxiname == taxiname and p.times[0] == times[0] and p.times[1] == times[1] and p.times[2] == times[2] and p.times[3] == times[3])
		return true;
	else
		return false;
}