#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include "taxi_header_use.h"
using namespace std;

//构造函数,初始为空
time_distribution::time_distribution() :timemap({}) {}

//析构函数,需要将其内部的vector的容量变为0,以此腾出内存空间,也需要将unordered_map进行和空map的交换,以此腾出内存空间,否则大数据量下信息统计容易出现不足的情况
time_distribution::~time_distribution()
{
	for (auto i : timemap)
	{
		//清空元素
		i.second.clear();
		//容量适应元素数量
		i.second.shrink_to_fit();
	}
	unordered_map<int, vector<Point>> uu;
	timemap.swap(uu);
}

//time_distribution类的初始化,以分钟为单位,每次输入一个出租车,如果该时间有有效信息就就行相应的信息保存
//i1,i2,i3对应了需要更新的时间的日时分信息
void time_distribution::time_distribution_update(taxi& vt, int i1, int i2, int i3)
{
	int name = vt.getname();
	if (name != 0) {
		for (int i = 0; i < vt.getsize(); i++) {
			Point p = vt.getPoint(i);
			int i11 = p.getday();
			int i22 = p.gethour();
			int i33 = p.getminute();

			//轨迹点时间符合要求就保存
			if (i11 == i1 and i22 == i2 and i33 == i3)
			{
				//先查询该出租车是否保存过某个点,没有则创建新的键值对插入保存
				auto temp = timemap.find(name);
				if (temp == timemap.end())
				{
					vector<Point> vtemp;
					vtemp.push_back(p);
					timemap.insert({ name , vtemp });
				}
				//有保存过对值的容器插入点信息即可
				else
				{
					temp->second.push_back(p);
				}
			}

			//轨迹点时间符合超出要求就结束(出租车内点信息按时间排序了)
			else if (i11 > i1 or (i11 == i1 and i22 > i2) or
				(i11 == i1 and i22 == i2 and i33 > i3))
			{
				break;
			}

			//轨迹点时间不到要求就继续
			else continue;

		}
	}
	return;
}

//输出该时间段内的全部信息
void time_distribution::print_time_distribution()
{
	for (auto i : timemap)
	{
		cout << i.first << ":" << endl;
		for (auto j = 0; j < i.second.size(); j++) {
			cout << i.second[j].getLongitude() << "," << i.second[j].getLatitude() << "," << i.second[j].gettimes() << endl;
		}
	}
	return;
}

//返回对出租车序号名的查询信息,查询到了就将该车该时段所有信息返回,否则返回序号名为0的无效信息对
pair<int, vector<Point>>& time_distribution::get_onetime_onetaxi(int n)
{
	if (timemap.find(n) != timemap.end())
	{
		pair<int, vector<Point>> temp = *timemap.find(n);
		return temp;
	}
	else
	{
		Point defaultpoint;
		vector<Point> p;
		p.push_back(defaultpoint);
		pair<int, vector<Point>> temp = { 0,p };
		return temp;
	}
}

//返回全部该时段的出租车轨迹点信息
unordered_map<int, vector<Point>> time_distribution::getallpoint()
{
	return timemap;
}

//以时间为划分依据,查询在某个范围内的出租车轨迹点信息,countnum和taxi_haveuse用于保存出现的总数量和哪些出现
set<Point, CMP> time_distribution::counttaxi(int day, int hour, int minute, double* p, int& countnum, bool* taxi_haveuse)
{
	//创建返回结果对象
	set<Point, CMP> taxinum;
	
	//遍历查询信息
	for (auto i : timemap)
	{
		for (auto j = 0; j < i.second.size(); j++) {

			//一旦有点在查询范围内,加入到对应结果中
			if (i.second[j].getLongitude() >= p[0] and i.second[j].getLongitude() <= p[1]
				and i.second[j].getLatitude() >= p[2] and i.second[j].getLatitude() <= p[3])
			{
				taxinum.insert(i.second[j]);

				//更新对应的出租车数量信息和序号名信息,出现则跳过,未出现就++count并标记此序号名数组位置为true
				if (!taxi_haveuse[i.second[j].gettaxiname()])
				{
					countnum++;
					taxi_haveuse[i.second[j].gettaxiname()] = true;
				}
			}
		}
	}

	//返回结果
	return taxinum;
}