#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include "taxi_header_use.h"

using namespace std;

//构造函数,逻辑同r类
position_distribution_outr::position_distribution_outr(int i) :vsize(i)
{
	information_regional_density.resize(vsize);
	count.resize(vsize);
	for (int i = 0; i < vsize; i++)
	{
		count[i] = 0;
	}
}

//析构函数,逻辑同r类
position_distribution_outr::~position_distribution_outr()
{
	count.clear();
	count.shrink_to_fit();
	information_regional_density.clear();
	information_regional_density.shrink_to_fit();
}

//返回所有时段出租车出现次数之和
int position_distribution_outr::getallvcount()
{
	int temp = 0;
	for (int i = 0; i < vsize; i++)
	{
		temp += count[i];
	}
	return temp;
}

//返回该类内共有多少个时段
int position_distribution_outr::getvsize()
{
	return vsize;
}

//返回该时段内某个时段北京外的出租车数量
int position_distribution_outr::getcountsize(int i) 
{
	//限定了i范围
	if (i < vsize)
	{
		return count[i];
	}
	else return -1;
}

//返回该时段内某个时段北京外的全部出租车信息
set<Point, CMP> position_distribution_outr::getallPoint(int i)
{
	//限定了i范围
	if (i < vsize)
		return information_regional_density[i];
	else return {};
}

//向该类对应时段内插入符合要求的轨迹点
void position_distribution_outr::insert(Point p, int i)
{
	//先将点信息直接插入对应位置
	information_regional_density[i].insert(p);

	//判断此出租车是否出现过,如果出现过不执行任何操作,出现过则将车名对应位置置为true,计数+1
	if (!taxi_haveuse[p.gettaxiname()])
	{
		count[i]++;
		taxi_haveuse[p.gettaxiname()] = true;
	}
	return;
}