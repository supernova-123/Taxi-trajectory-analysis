#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include "taxi_header_use.h"

using namespace std;

//构造函数,初始为空,经纬度为0
position_distribution_inBeiJing::position_distribution_inBeiJing() :posmap({}), init_Longitude(0), init_Latitude(0) {}

//析构函数,需要将其内部的vector的容量变为0,以此腾出内存空间,也需要将unordered_map进行和空map的交换,以此腾出内存空间,否则大数据量下信息统计容易出现不足的情况
position_distribution_inBeiJing::~position_distribution_inBeiJing() {
	for (auto i : posmap)
	{
		//清空元素
		i.second.clear();
		//容量适应元素数量
		i.second.shrink_to_fit();
	}
	unordered_map<int, vector<Point>> uu;
	posmap.swap(uu);
}

//position_distribution_inBeiJing类的初始化,以0.01*0.01经纬度的变化为单位,每次输入一个出租车,如果该空间范围有有效信息就就行相应的信息保存
//d1,d2对应了需要更新的空间的经纬度信息
void position_distribution_inBeiJing::position_distribution_inBeiJing_update(taxi& vt, double d1, double d2)
{
	init_Longitude = d1;
	init_Latitude = d2;
	int name = vt.getname();
	if (name != 0) {
		for (int i = 0; i < vt.getsize(); i++) {
			Point p = vt.getPoint(i);
			double d11 = p.getLongitude();
			double d22 = p.getLatitude();
			//轨迹点空间符合要求就保存
			if (d11 < d1 + 0.01 and d22 < d2 + 0.01 and d11 >= d1 and d22 >= d2)
			{
				//先查询该出租车是否保存过某个点,没有则创建新的键值对插入保存
				auto temp = posmap.find(name);
				if (temp == posmap.end())
				{
					vector<Point> vtemp;
					vtemp.push_back(p);
					posmap.insert({ name , vtemp });
				}
				//有保存过对值的容器插入点信息即可
				else
				{
					temp->second.push_back(p);
				}
			}
			//轨迹点空间不符号要求就继续(因为出租车轨迹整体需要假定为完全无序的)
			else continue;
		}
	}
	return;
}
//position_distribution_inBeiJing类的初始化,以0.01*0.01经纬度的变化为单位,每次输入一个点信息进行保存
//d1,d2对应了需要更新的空间的经纬度信息
void position_distribution_inBeiJing::position_distribution_inBeiJing_update2(Point& t, double d1, double d2)
{
	init_Longitude = d1;
	init_Latitude = d2;
	auto i = posmap.find(t.gettaxiname());
	if (i == posmap.end())
	{
		vector<Point>temp;
		temp.push_back(t);
		posmap.insert({ t.gettaxiname(),temp });

	}
	else 
	{
		i->second.push_back(t);
	}
	return;
}

//空间分布类的文件读取初始化函数
void position_distribution_inBeiJing::position_distribution_inBeiJing_init(vector<vector<string>>& vs)
{
	//-1和0对应的是标记是否是第一次行信息
	int name = -1;
	for (auto i : vs)
	{
		//是第一行
		if(name==-1)
		{
			//存储的对应经纬度进行初始化
			init_Longitude = stod(i[0]);
			init_Latitude = stod(i[1]);
			name = 0;
			continue;
		}

		//不是第一行且如果是某个车的总信息一行信息
		if (i.size() == 2)
		{
			//插入对应序号名和空容器
			name = stoi(i[0]);
			vector<Point> temp;
			posmap.insert({ name, temp });
		}

		//如果是其中的某个点
		else
		{
			Point temp;

			//点的初始化
			temp.Point_init2(name, stoi(i[2]), stoi(i[3]), stoi(i[4]), stoi(i[5]), stod(i[0]), stod(i[1]));

			//点放入车的对应容器中
			auto itemp = posmap.find(name);


			itemp->second.push_back(temp);
		}
	}
	return;
}

//输出该空间范围内的全部信息
void position_distribution_inBeiJing::print_position_distribution_inBeiJing()
{
	for (auto i : posmap)
	{
		cout << i.first << ":" << endl;
		for (auto j = 0; j < i.second.size(); j++) {
			cout << i.second[j].getLongitude() << "," << i.second[j].getLatitude() << "," << i.second[j].gettimes() << endl;
		}
	}
	return;
}

//获得对应经度
double position_distribution_inBeiJing::getinit_Longitude() { return init_Longitude; }

//获得对应纬度
double position_distribution_inBeiJing::getinit_Latitude() { return init_Latitude; }

//返回该空间内的全部出租车的全部信息
unordered_map<int, vector<Point>> position_distribution_inBeiJing::getallpoint() 
{ return posmap; }

//返回对出租车序号名的查询信息,查询到了就将该车该空间范围的所有信息返回,否则返回序号名为0的无效信息对
pair<int, vector<Point>>& position_distribution_inBeiJing::get_onetime_onetaxi(int n)
{
	if (posmap.find(n) != posmap.end())
	{
		pair<int, vector<Point>> temp = *posmap.find(n);
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

//以空间为划分依据,查询在某个范围内,某段时间内的出租车轨迹点信息,countnum和taxi_haveuse用于保存出现的总数量和哪些出现
set<Point, CMP> position_distribution_inBeiJing::counttaxi(int day, int hour, int minute, double* p, int& countnum, int time_span, bool* taxi_haveuse)
{
	//创建返回结果对象
	set<Point, CMP> taxinum;

	//遍历查询信息
	for (auto i : posmap)
	{
		for (auto j = 0; j < i.second.size(); j++) {

			//需要计算查询时间和所查点的时间差判断计算范围
			int temptime = gettime(day, hour, minute, 0, i.second[j].getday(), i.second[j].gethour(), i.second[j].getminute(), i.second[j].getsecond());
			
			//一旦有点在查询范围内且在时段内,加入到对应结果中
			//temptime <= time_span*60 和 temptime >= 0是充要限制,只要满足一定在该时间段内
			if (temptime <= time_span * 60 and temptime >= 0 and i.second[j].getLongitude() >= p[0] and i.second[j].getLongitude() <= p[1] and i.second[j].getLatitude() >= p[2] and i.second[j].getLatitude() <= p[3])
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