#include <iostream>
#include <vector>
#include "taxi_header_use.h"
using namespace std;

//构造函数:
taxi::taxi() :name(0), size(0), pv(NULL) {}

//拷贝构造函数,防止值传递时发生错误
taxi::taxi(const taxi& t) 
{
	name=t.name;
	size=t.size;
	pv = new Point[size];
	for(int i=0;i<size;i++)
	{
		pv[i] = t.pv[i];
	}
}

//赋值运算符防止值传递时发生错误
taxi taxi::operator=(const taxi& t)
{
	name = t.name;
	size = t.size;
	if (pv != NULL)
		delete[]pv;
	pv = new Point[size];
	for (int i = 0; i < size; i++)
	{
		pv[i] = t.pv[i];
	}
	return *this;
}

//taxi的析构,pv一定是空或者指向堆的指针,可以放心delete
taxi::~taxi()
{
	if (pv != NULL)
	{
		delete[]pv;
		pv = NULL;
	}
}

//taxi初始化函数,只在原数据处理时使用,以后用反序列化代替此函数(这个速度太慢了)
//传入参数为处理好的出租车信息二维数组,纵为点数,横为信息数(名字,时间的日时分秒,空间的经纬度)
void taxi::taxi_init(vector<vector<string>>& vs)
{
	//从其中获得其序号名
	name = stoi(vs[0][0]);

	//获得全部点的数量
	int tempsize = int(vs.size());
	
	//创建暂态轨迹点指针,指向全部点信息
	Point* temppv = new Point[vs.size()];
	size = 0;
	int j = 0;
	int i;
	
	//清洗数据,对点信息进行处理,将无效点去除
	
	for (i = 0; i < tempsize; i++)
	{
		//如果这个点的经度小于100或者纬度小于20直接认定为无效点(车不能开到海里去吧)
		if (stoi(vs[j][2]) < 100 or stoi(vs[j][3]) <= 20) {
			i--; tempsize--;
			j++; continue;
		}
		//如果不是这样的点就正常初始化点信息
		(*(temppv + i)).Point_init(name, vs[j][1], vs[j][2], vs[j][3]);
		
		//i==0是第一个点,作为参考点,
		if (i != 0) {

			//获得当前点和前一个点的经度差:
			double d1 = (*(temppv + i)).getLongitude() - (*(temppv + i - 1)).getLongitude();

			//获得当前点和前一个点的纬度差:
			double d2 = (*(temppv + i)).getLatitude() - (*(temppv + i - 1)).getLatitude();
			
			//获得两点的经纬度距离差并计算出实际的距离差
			double distance = sqrt(d1 * d1 * 100000 * 100000 + d2 * d2 * 111320 * 111320);

			//计算两点相差的时间,以秒为单位
			double time = gettime(
				(*(temppv + i - 1)).getday(), (*(temppv + i - 1)).gethour(),
				(*(temppv + i - 1)).getminute(), (*(temppv + i - 1)).getsecond(),
				(*(temppv + i)).getday(), (*(temppv + i)).gethour(),
				(*(temppv + i)).getminute(), (*(temppv + i)).getsecond()
			);

			//如果两点之间的速度超过了67m/s,说明不是车的速度这么快,而是数据测量的错误,视为无效点
			if (distance / (time + 0.00001) > 67)
			{
				cout << "name:" << name << endl;
				cout << "第" << j + 1 << "条 is error" << endl;
				i--;
				tempsize--;
			}
		}
		j++;
	}

	//size保存有效点数量
	size = tempsize;
	
	//pv保存有效点信息
	pv = new Point[size];
	for (i = 0; i < size; i++)
	{
		pv[i] = temppv[i];
	}

	//节省内存空间
	delete[]temppv;
	return;
}

//返回taxi的各类有效信息
int taxi::getsize() { return size; }
int taxi::getname() { return name; }
double taxi::getLongitude2(int i) 
{
	//如果有索引需要考虑是否越界
	if (pv != NULL and i < size and i >= 0)
		return pv[i].getLongitude();
	else return 0;
}
double taxi::getLatitude2(int i) 
{
	if (pv != NULL and i < size and i >= 0)
		return pv[i].getLatitude();
	else return 0;
}
Point& taxi::getPoint(int pos)
{
	Point temp;
	if (pv != NULL and pos < size and pos >= 0)
		return pv[pos];
	else return temp;
}


//taxi的全部信息输出
void taxi::printtaxi_track()
{
	cout << "name:" << name << endl;
	if (pv == NULL)
	{
		cout << "No data!" << endl;
		return;
	}
	for (int i = 0; i < size; i++)
	{
		cout << "Point:" << endl;
		cout << "time:" << pv[i].gettimes() << endl;
		cout << "Longitude:" << setprecision(8) << pv[i].getLongitude() << endl;
		cout << "Latitude:" << setprecision(8) << pv[i].getLatitude() << endl;
		//注意点经纬度信息最多保存了8位有效数字
	}
	return;
}
