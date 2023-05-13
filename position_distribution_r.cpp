#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include "taxi_header_use.h"

using namespace std;
int position_distribution_r::r = 0;
int position_distribution_r::vsize = 0;
//构造函数,确定两个静态变量r和vsize
position_distribution_r::position_distribution_r(int i, int j) 
{
	r = i, vsize = j;
}

//析构函数将所有容器全部清空腾出内存用于后续其他分析
position_distribution_r::~position_distribution_r()
{
	taxijudge.clear();
	taxijudge.shrink_to_fit();
	information_regional_density.clear();
	information_regional_density.shrink_to_fit();
}

//返回所有时段出租车出现次数之和
int position_distribution_r::getallcount()
{
	int temp = 0;
	//taxijudge.size()表示有效时段
	for (int i = 0; i < taxijudge.size(); i++)
	{
		temp += taxijudge[i].size();
	}
	return temp;
}

//返回该类内共有多少个时段
int position_distribution_r::getvsize()
{
	return vsize;
}

//返回该时段内某个时段该区域的出租车序号名集合,时段次序由i指定
vector<int> position_distribution_r::gettaxijudge(int i)
{
	//限定了i范围
	if (i < vsize and i < taxijudge.size())
	{
		return taxijudge[i];
	}
	else return {};
}

//返回该时段内某个时段该区域的出租车数量,时段次序由i指定
int position_distribution_r::getcountsize(int i) 
{
	if (i < taxijudge.size() and i < vsize) {
		return taxijudge[i].size();
	}
	else return 0;
}


//返回该时段内某个时段该区域的全部出租车信息,时段次序由i指定
set<Point, CMP> position_distribution_r::getallPoint(int i)
{
	//限定了i范围
	if (i < vsize and i< information_regional_density.size())
		return information_regional_density[i];
	else return {};
}

//向该类对应时段内插入符合要求的轨迹点,对应时段由i指定
void position_distribution_r::insert(Point p, int i)
{
	//先判断容器大小是否满足要求,不满足则扩容(为了省内存)
	if (information_regional_density.size() <= i)
	{
		information_regional_density.resize(i + 1, {});
		//将点信息直接插入对应位置
		information_regional_density[i].insert(p);
	}
	//满足直接插入
	else 
	{
		information_regional_density[i].insert(p);
	}

	//先判断容器大小是否满足要求,不满足则扩容(为了省内存)
	if (taxijudge.size()<=i)
	{
		taxijudge.resize(i + 1, {});
		//判断此出租车是否出现过,如果出现过不执行任何操作,出现过则将车名加入对应容器中,计数+1
		if (find(taxijudge[i].begin(), taxijudge[i].end(), p.gettaxiname()) == taxijudge[i].end())
		{
			taxijudge[i].push_back(p.gettaxiname());
		}
	}
	//满足跳到下一个判断
	else 
	{
		if (find(taxijudge[i].begin(), taxijudge[i].end(), p.gettaxiname()) == taxijudge[i].end())
		{
			taxijudge[i].push_back(p.gettaxiname());
		}
	}
	return;
}