#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include "taxi_header_use.h"

using namespace std;

//构造函数,初始为空
position_distribution_outBeiJing::position_distribution_outBeiJing() :posmap({}) {}

//析构函数
position_distribution_outBeiJing::~position_distribution_outBeiJing() {
	for (auto i : posmap)
	{
		i.second.clear();
		i.second.shrink_to_fit();
	}
	unordered_map<int, vector<Point>> uu;
	posmap.swap(uu);
}

//position_distribution_outBeiJing类的初始化,每次输入一个出租车,如果该空间范围有有效信息就就行相应的信息保存
void position_distribution_outBeiJing::position_distribution_outBeiJing_update(taxi& vt)
{
	int name = vt.getname();
	if (name != 0) {
		for (int i = 0; i < vt.getsize(); i++) {
			Point p = vt.getPoint(i);
			double d11 = p.getLongitude();
			double d22 = p.getLatitude();

			//不在北京范围内就进行保存
			if (d11 < 115.40 or d11 >= 117.60 or d22 < 39.40 or d22>41.10)
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
			else continue;
		}
	}
	return;
}

//空间分布类的文件读取初始化函数
void position_distribution_outBeiJing::position_distribution_outBeiJing_init(vector<vector<string>>& vs)
{
	int name =0;
	for (auto i : vs)
	{

		//如果是某个车的总信息一行信息
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
void position_distribution_outBeiJing::print_position_distribution_outBeiJing()
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
	
unordered_map<int, vector<Point>> position_distribution_outBeiJing::getallpoint() 
{
	return posmap;
}

//返回对出租车序号名的查询信息,查询到了就将该车该空间范围的所有信息返回,否则返回序号名为0的无效信息对
pair<int, vector<Point>>& position_distribution_outBeiJing::get_onetime_onetaxi(int n)
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