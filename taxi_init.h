#ifndef TAXI_INIT_H
#define TAXI_INIT_H
#include<iostream>
#include<vector>
#include"taxi_header_use.h"
using namespace std;
//�ָ��,���ڽ����ݼ��ĳ��⳵��Ϣ�ָ�ɳ�����ʱ�����ʱ���롢�ռ侭γ��
vector<string> Split(const string& str, char split);








//���⳵�����л�����,���ڳ��⳵������ϴ�ͽ����ĳ��⳵����Ϣ���л�
//����������ٷ����л�������ĳ�ʼ����
void boost_archive_taxi(const int d);

//�ļ�����汾
void boost_archive_taxi2(const int d);


//�����л����⳵�ຯ��,���ڿ��ٽ�����ʹ����Ҫ�ĳ��⳵������Ϣ,���ض�Ӧ�ķ����л����⳵������Ϣ
taxi boost_iarchive_taxi(int i);

//�ļ�����汾
taxi boost_iarchive_taxi2(int i);

//taxi�����л���Ϣ���뵽�ļ��н���������ʾ,������������
void taxifile(taxi& t);






//���⳵ʱ��ֲ������л�����,���ڶԳ��⳵�켣��ʱ����Ϣ�ķ���ʹ���,�Է���Ϊ��λ
//����������ٷ����л�������ĳ�ʼ����
void boost_archive_time();

//�ļ��汾
void boost_archive_time2(int needday,int kkk);


//�����л����⳵ʱ��ֲ��ຯ��,���ڿ��ٽ�����ʹ����Ҫ��Ӧʱ��εĳ��⳵��Ϣ,���ض�Ӧ�ķ����л����⳵ʱ��ֲ��������Ϣ
time_distribution boost_iarchive_time(int i,int j,int k);

//�ļ��汾
time_distribution boost_iarchive_time2(int i, int j, int k);









//���⳵�ռ�ֲ������л�����,���ڶԳ��⳵�켣�㱱���ڿռ���Ϣ�ķ���ʹ���,��0.01��γ�ȱ仯Ϊ��λ
//����������ٷ����л�������ĳ�ʼ����
void boost_archive_position_in();

//�ļ��汾
void boost_archive_position_in2();



//���⳵�ռ�ֲ������л�����,���ڶԳ��⳵�켣�㱱����ռ���Ϣ�Ĵ���,��Ϊһ������ʹ��
//����������ٷ����л�������ĳ�ʼ����
void boost_archive_position_out();

void boost_archive_position_out2();






//�����л����⳵�����ڿռ�ֲ��ຯ��,���ڿ��ٽ�����ʹ����Ҫ��Ӧ�ռ�λ�õĳ��⳵��Ϣ,���ض�Ӧ�ķ����л����⳵�����ڿռ�ֲ��������Ϣ
position_distribution_inBeiJing boost_iarchive_position_in(double i,double j);

//�ļ��汾
position_distribution_inBeiJing boost_iarchive_position_in2(double i, double j);



//�����л����⳵������ռ�ֲ��ຯ��,���ڿ��ٽ�����ʹ����Ҫ��Ӧ�ռ�λ�õĳ��⳵��Ϣ,���ض�Ӧ�ķ����л����⳵������ռ�ֲ��������Ϣ
position_distribution_outBeiJing boost_iarchive_position_out();

//�ļ��汾
position_distribution_outBeiJing boost_iarchive_position_out2();





//�����л�·��ͼ��,���ڷ���·����Ϣ,��Ϊ�ö���̫�������÷�ʽ����
void boost_pathserialization_iarchive(pathgraph&p);

//�ļ��汾
void boost_pathserialization_iarchive2(pathgraph& p);





//�����Ǳ����õ�·������Ϣ,����Ϣ��̫�󲻹����ڴ������ж�����

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