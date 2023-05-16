#ifndef TAXI_INIT_H
#define TAXI_INIT_H
#include<iostream>
#include<vector>
#include"taxi_header_use.h"
using namespace std;
//分割函数,用于将数据集的出租车信息分割成车名、时间的日时分秒、空间经纬度
vector<string> Split(const string& str, char split);








//出租车类序列化函数,用于出租车数据清洗和建立的出租车类信息序列化
//方便后续快速反序列化进行类的初始化。
void boost_archive_taxi(const int d);

//文件处理版本
void boost_archive_taxi2(const int d);


//反序列化出租车类函数,用于快速建立和使用需要的出租车对象信息,返回对应的反序列化出租车对象信息
taxi boost_iarchive_taxi(int i);

//文件处理版本
taxi boost_iarchive_taxi2(int i);

//taxi反序列化信息输入到文件中进行正常表示,方便其他操作
void taxifile(taxi& t);






//出租车时间分布类序列化函数,用于对出租车轨迹点时间信息的分类和处理,以分钟为单位
//方便后续快速反序列化进行类的初始化。
void boost_archive_time();

//文件版本
void boost_archive_time2(int needday,int kkk);


//反序列化出租车时间分布类函数,用于快速建立和使用需要对应时间段的出租车信息,返回对应的反序列化出租车时间分布类对象信息
time_distribution boost_iarchive_time(int i,int j,int k);

//文件版本
time_distribution boost_iarchive_time2(int i, int j, int k);









//出租车空间分布类序列化函数,用于对出租车轨迹点北京内空间信息的分类和处理,以0.01经纬度变化为单位
//方便后续快速反序列化进行类的初始化。
void boost_archive_position_in();

//文件版本
void boost_archive_position_in2();



//出租车空间分布类序列化函数,用于对出租车轨迹点北京外空间信息的处理,作为一个整体使用
//方便后续快速反序列化进行类的初始化。
void boost_archive_position_out();

void boost_archive_position_out2();






//反序列化出租车北京内空间分布类函数,用于快速建立和使用需要对应空间位置的出租车信息,返回对应的反序列化出租车北京内空间分布类对象信息
position_distribution_inBeiJing boost_iarchive_position_in(double i,double j);

//文件版本
position_distribution_inBeiJing boost_iarchive_position_in2(double i, double j);



//反序列化出租车北京外空间分布类函数,用于快速建立和使用需要对应空间位置的出租车信息,返回对应的反序列化出租车北京外空间分布类对象信息
position_distribution_outBeiJing boost_iarchive_position_out();

//文件版本
position_distribution_outBeiJing boost_iarchive_position_out2();





//反序列化路径图类,用于分析路径信息,因为该对象太大以引用方式传递
void boost_pathserialization_iarchive(pathgraph&p);

//文件版本
void boost_pathserialization_iarchive2(pathgraph& p);





//下面是被废置的路径及信息,因信息量太大不够在内存中运行而废置

//class path
//{
//	friend class boost::serialization::access;
//private:
//	pathNode* pstart;
//	pathNode* pend;
//	int count;
//	vector<int> times;
//	double len;
//	template<class Archive>
//	void serialize(Archive& ar, const unsigned int version)
//	{
//		ar& pstart;
//		ar& pend;
//		ar& count;
//		ar& times;
//		ar& len;
//	}
//public:
//	path() :count(0), len(0), pstart(NULL), pend(NULL), times({}) {}
//	~path() 
//	{
//		if (pstart != NULL and pend != NULL)
//		{
//			delete pstart;
//			delete pend;
//		}
//	}
//	path(const path& p) 
//	{
//		count = p.count;
//		len = p.len;
//		times = p.times;
//		pstart = new pathNode(*p.pstart);
//		pend = new pathNode(*p.pend);
//	}
//	void insertpath(Point a, Point b, double LongitudeNode1, double LatitudeNode1, double LongitudeNode2, double LatitudeNode2)
//	{
//		if (pstart == NULL and pend == NULL) {
//			pstart = new pathNode(LongitudeNode1, LatitudeNode1);
//			pend = new pathNode(LongitudeNode2, LatitudeNode2);
//			len = 111194.9266 * sqrt(pow(LongitudeNode2 - LongitudeNode1, 2) + pow((LatitudeNode2 - LatitudeNode1) * lodividela, 2));
//		}
//		pstart->insertpoint(a);
//		pend->insertpoint(b);
//		times.push_back(gettime(a.getday(), a.gethour(), a.getminute(), a.getsecond(), b.getday(), b.gethour(), b.getminute(), b.getsecond()));
//		count++;
//	}
//	pathNode* getpathstrat() 
//	{
//		return pstart;
//	}
//	pathNode* getpathend()
//	{
//		return pend;
//	}
//	int getpathcount() 
//	{
//		return count;
//	}
//	int getpathshortesttime() 
//	{
//		if (!times.empty()) {
//			sort(times.rbegin(), times.rend());
//			return times[0];
//		}
//		else return 0;
//	}
//	double getpathlen() 
//	{
//		return len;
//	}
//	void printpath_information() 
//	{
//		cout << "path:" << endl;
//		cout << "startLo:" << pstart->getLongitudeNode() << endl;
//		cout << "startLa:" << pstart->getLatitudeNode() << endl;
//		cout << "endLo:" << pend->getLongitudeNode() << endl;
//		cout << "endLa:" << pend->getLatitudeNode() << endl;
//		cout << "path count:" << count << endl;
//		cout << "path len:" << len << endl;
//		cout << "path shortesttime:" << this->getpathshortesttime() << endl << endl;
//		cout << "path startNode:"; pstart->printpathNode();
//		cout << "path endNode:"; pend->printpathNode();
//		return;
//	}
//};

//class CMPpath 
//{
//public:
//	bool operator() (string a, string b)const 
//	{
//		int i = 0;
//		while (a[i] == b[i] and i < a.size() and i<b.size())
//		{
//			i++;
//		}
//		return a[i] < b[i];
//	}
//};
//

//class CMPpath1 
//{
//public:
//	bool operator() (path a, path b)const 
//	{
//		return (a.getpathlen() * 1000 + a.getpathcount() > b.getpathlen() * 1000 + b.getpathcount());
//	}
//};
//

//class CMPpath2 
//{
//public:
//	bool operator() (path a, path b)const
//	{
//		return (a.getpathlen() + a.getpathcount()*1000000 > b.getpathlen() + b.getpathcount() * 1000000);
//	}
//};
//

//class CMPpath3
//{
//public:
//	bool operator() (path a, path b)const
//	{
//		double d1 = a.getpathstrat()->getLongitudeNode() * 1000 + a.getpathstrat()->getLatitudeNode();
//		double d2 = b.getpathstrat()->getLongitudeNode() * 1000 + b.getpathstrat()->getLatitudeNode();
//		return d1 < d2;
//	}
//};

//class pathset
//{
//	friend class boost::serialization::access;
//private:
//	map<string,path,CMPpath>* pathmap;
//	template<class Archive>
//	void serialize(Archive& ar, const unsigned int version)
//	{
//		ar& pathmap;
//	}
//public:
//	pathset() :pathmap(NULL) {}
//	~pathset() 
//	{
//		if (pathmap != NULL) 
//		{
//			delete pathmap;
//		}
//	}
//	map<string, path, CMPpath> getpathmap() 
//	{
//		return *pathmap;
//	}
//	int getpathmapsize() { return pathmap->size(); }
//	bool pathsetinsert(Point a, Point b) 
//	{
//		if (pathmap == NULL) 
//		{
//			pathmap = new map<string ,path, CMPpath>;
//		}
//		double LongitudeNode1 = floor(a.getLongitude() * 1000) / 1000;
//		double LongitudeNode2 = floor(b.getLongitude() * 1000) / 1000;
//		double LatitudeNode1= floor(a.getLatitude() * 1000) / 1000;
//		double LatitudeNode2 = floor(b.getLatitude() * 1000) / 1000;
//		if (LongitudeNode1 == LongitudeNode2 and LatitudeNode1 == LatitudeNode2) 
//		{
//			return false;
//		}
//		string flagpath1 = to_string(LongitudeNode1)+"," + to_string(LatitudeNode1)+"," + to_string(LongitudeNode2)+"," + to_string(LatitudeNode2);
//		string flagpath2 = to_string(LongitudeNode2) + "," + to_string(LatitudeNode2) + "," + to_string(LongitudeNode1) + "," + to_string(LatitudeNode1);
//		auto i = pathmap->find(flagpath1); 
//		if (i == pathmap->end())
//		{
//			auto j = pathmap->find(flagpath2);
//			if (j == pathmap->end()) {
//				path newpath;
//				newpath.insertpath(a, b, LongitudeNode1, LatitudeNode1, LongitudeNode2, LatitudeNode2);
//				pathmap->insert({ flagpath1,newpath });
//			}
//			else 
//			{
//				j->second.insertpath(a, b, LongitudeNode2, LatitudeNode2, LongitudeNode1, LatitudeNode1);
//			}
//		}
//		else 
//		{
//			i->second.insertpath(a, b, LongitudeNode1, LatitudeNode1, LongitudeNode2, LatitudeNode2);
//		}
//		return true;
//	}
//	void pathserialization() 
//	{
//		for (auto i : *pathmap) 
//		{
//			ofstream ofs;
//			ofs.open("E:\\pathdata\\pathdatafor"+i.first);
//			boost::archive::text_oarchive oa(ofs);
//			oa << i.second;
//			ofs.close();
//		}
//		ofstream ofs;
//		ofs.open("E:\\pathdata\\pathdataall");
//		boost::archive::text_oarchive oa(ofs);
//		oa << *this;
//		ofs.close();
//	}
//};

#endif