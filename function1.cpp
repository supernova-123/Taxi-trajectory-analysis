#include <iostream>
#include <set>
#include <unordered_map>
#include <cmath>
#include "taxi_header_use.h"
#include "taxi_init.h"
using namespace std;


//函数功能1实现
//set<Point, CMP>& s为引用,是为了将区域内的点信息保存到其中,以此来方便后续其他功能需要划定矩形区域时能够获得相关信息
//返回矩形区域内的出租车的数目
int function_findtaxinum_somewhere(set<Point, CMP>& s, double* pos, int day, int hour, int minute, int time_span , bool output)
{

	//初始化该矩形区域出租车数量
	int counttaxi = 0;

	//初始化判断该区域出租车序号名是否出现数组
	bool taxiflag[10375] = { 0 };

	//判断传入矩形的两个点是否均在北京市内
	if (pos[0] >= inBeijing_Longitude_min and pos[1] <= inBeijing_Longitude_max
		and pos[2] >= inBeijing_Latitude_min and pos[3] <= inBeijing_Latitude_max)
	{
		//如果在,根据空间分部类进行信息初始化

		//初始化给定的两个点对应的空间分部类的最大和最小经纬度
		double startLongitude = floor(pos[0] * 100) / 100;
		double endLongitude = ceil(pos[1] * 100) / 100;
		double startLatitude = floor(pos[2] * 100) / 100;
		double endLatitude = ceil(pos[3] * 100) / 100;

		//计算需要进行初始化的空间分部类的矩阵行列数量信息
		int row = round(endLongitude * 100 - startLongitude * 100);
		int col = round(endLatitude * 100 - startLatitude * 100);

		//进行对对应数量的空间分部类的初始化
		position_distribution_inBeiJing** newpdi = new position_distribution_inBeiJing * [row];
		for (int i = 0; i < row; i++)
		{
			newpdi[i] = new position_distribution_inBeiJing[col];
		}

		//根据给定的坐标信息进行全部在范围内的空间分布对象的初始化
		for (double d1 = startLongitude; d1 < endLongitude; d1 += 0.01)
		{
			for (double d2 = startLatitude; d2 < endLatitude; d2 += 0.01)
			{
				//序列化空间分布类对象
				newpdi[(int)((d1 - startLongitude) * 100)][(int)((d2 - startLatitude) * 100)] = boost_iarchive_position_in(d1, d2);
			}
		}

		//判断给定的初始开始分析时间是否符合实际数据集要求
		if (day >= 2 and day <= 8 and hour >= 0 and hour <= 59 and minute >= 0 and minute <= 59)
		{
			//符合要求,遍历全部反序列化的信息点
			for (int i = 0; i < row; i++)
			{
				for (int j = 0; j < col; j++) {

					//将各种参数传入分析函数,符合要求就更新counttaxi和taxiflag和temp
					set<Point, CMP> temp = newpdi[i][j].counttaxi(day, hour, minute, pos, counttaxi, time_span, taxiflag);

					//向s插入temp存储的所有点信息
					s.insert(temp.begin(), temp.end());
				}
			}

			//功能使用处,根据s内信息和counttaxi进行分析
			if (output) {
				cout << "in here and in this time , taxinum =" << counttaxi << endl;

				for (auto i : s)
				{
					cout << i.gettaxiname() << " " << i.getday() << " " << i.gethour() << " " << i.getminute() << " " << i.getsecond() << " " << i.getLongitude() << "  " << i.getLatitude() << endl;
				}
			}
		}

		//使用功能完毕,删除反序列化信息
		for (int i = 0; i < row; i++)
		{
			delete[] newpdi[i];
		}
		delete[]newpdi;
	}

	//如果两点不都在北京内,根据时间分部类进行信息初始化
	else
	{

		//判断给定的初始开始分析时间是否符合实际数据集要求
		if (day >= 2 and day <= 8 and hour >= 0 and hour <= 59 and minute >= 0 and minute <= 59)
		{
			//建立时间分部类数组
			time_distribution* newtd;

			//统计时间分布类数组大小
			int countnum;

			//计算需要进行初始化的时间分部类的数组元素个数信息
			//如果初始时间+时间长度<=数据集最后一天最后一分钟,数组元素个数=数据集最后时间-初始时间(以分钟为单位)
			if (gettime(day, hour, minute, 0, 8, 23, 59, 59) + 1 >= 60 * time_span)
			{
				newtd = new time_distribution[time_span];
				countnum = time_span;
			}

			//如果初始时间+时间长度>数据集最后一天最后一分钟,数组元素个数=时间长度(以分钟为单位)
			else
			{
				countnum = (8 - day) * 24 * 60 + (23 - hour) * 60 + 59 - minute;
				newtd = new time_distribution[countnum];
			}

			//从初始时间开始遍历每一分钟到上面给定的最后时间或者数据集结束时间
			int dayreal = day, hourreal = hour, minutereal = minute;
			for (int i = 0; i < countnum; i++) {

				//序列化时间分布类对象
				newtd[i] = boost_iarchive_time(dayreal, hourreal, minutereal);
				minutereal++;
				//计算下一个时间是否要跨小时/天
				if (minutereal > 59)
				{
					minutereal %= 60;
					hourreal++;
				}
				if (hourreal > 23)
				{
					hourreal %= 24;
					dayreal++;
				}
			}

			//遍历全部反序列化的信息点
			for (int i = 0; i < countnum; i++)
			{
				//将各种参数传入分析函数, 符合要求就更新counttaxi和taxiflag和temp
				set<Point, CMP> temp = newtd[i].counttaxi(day, hour, minute, pos, counttaxi, taxiflag);
				s.insert(temp.begin(), temp.end());
			}

			//函数功能使用处
			if (output) {
				cout << "in here and in this time , taxinum =" << counttaxi << endl;
				for (auto i : s)
				{
					cout << i.gettaxiname() << " " << i.getday() << " " << i.gethour() << " " << i.getminute() << " " << i.getsecond() << " " << i.getLongitude() << "  " << i.getLatitude() << endl;
				}
			}

			//使用结束,删除反序列化信息
			delete[]newtd;
		}
	}
	//返回矩形区域内的出租车的数目
	return counttaxi;
}


//函数功能2实现
void function_regional_density_analysis(int r, int day, int hour, int minute, int time_span, bool output)
{
	//判断给定的初始开始分析时间是否符合实际数据集要求
	if (day >= 2 and day <= 8 and hour >= 0 and hour <= 59 and minute >= 0 and minute <= 59)
	{
		//建立时间分部类数组
		time_distribution* newtd;

		//统计时间分布类数组大小
		int countnum;

		//计算需要进行初始化的时间分部类的数组元素个数信息
		//如果初始时间+时间长度<=数据集最后一天最后一分钟,数组元素个数=数据集最后时间-初始时间(以分钟为单位)
		if (gettime(day, hour, minute, 0, 8, 23, 59, 59) + 12 >= 60 * time_span)
		{
			countnum = time_span;
			newtd = new time_distribution[countnum];
		}

		//如果初始时间+时间长度>数据集最后一天最后一分钟,数组元素个数=时间长度(以分钟为单位)
		else
		{
			countnum = (8 - day) * 24 * 60 + (24 - hour) * 60 + 59 - minute;
			newtd = new time_distribution[countnum];
		}

		//计算统计的时间段个数,基础时间间隔为10分钟,可在changetime处修改
		int changesize = ceil(countnum / changetime);

		//从初始时间开始遍历每一分钟到上面给定的最后时间或者数据集结束时间
		int dayreal = day, hourreal = hour, minutereal = minute;
		for (int i = 0; i < countnum; i++) {
			
			//序列化时间分布类对象
			newtd[i] = boost_iarchive_time(dayreal, hourreal, minutereal);

			//计算下一个时间是否要跨小时/天
			minutereal++;
			if (minutereal > 59)
			{
				minutereal %= 60;
				hourreal++;
			}
			if (hourreal > 23)
			{
				hourreal %= 24;
				dayreal++;
			}
		}

		//计算北京内按r而被分割成的经纬度各自对应的区域块数
		//加1对应边缘的点
		int countrnumLong = ceil(2200 / r) + 1;
		int countrnumLa = ceil(1700 / r) + 1;

		//建立对应数量的r区域对象数组,进行初始化
		position_distribution_r*** pdr = new position_distribution_r * *[countrnumLong];
		for (int i = 0; i < countrnumLong; i++)
		{
			pdr[i] = new position_distribution_r * [countrnumLa];
			for (int j = 0; j < countrnumLa; j++)
			{
				pdr[i][j] = new position_distribution_r(r, changesize);
			}
		}

		//建立北京外的区域对象,进行初始化
		position_distribution_outr* pdor = new position_distribution_outr(changesize);

		//利用dtemp1和dtemp2存储要分析的点的经纬度
		double dtemp1, dtemp2;

		//循环处理符合要求的所有点信息(在时间分部类内),将这些信息按r区域对象格式进行保存
		//按时间段分割对象
		for (int n = 0; n < changesize; n++)
		{
			//按单个时间段保存点信息
			for (int i = 0; i < changetime; i++)
			{
				int index = n * changetime + i;
				
				//如果超过范围就停止
				if (index >= countnum) { break; }

				//返回全部时间范围内的点信息
				unordered_map<int, vector<Point>> temp = newtd[index].getallpoint();
				int t = 0;
				
				//遍历点信息,按要求进行归类
				for (auto j : temp)
				{
					for (auto k : j.second)
					{
						dtemp1 = k.getLongitude();
						dtemp2 = k.getLatitude();
						
						//如果满足在北京内就保存到对应的r区域对象格子内
						if (dtemp1 >= 115.40 and dtemp1 <= 117.60 and dtemp2 >= 39.40 and dtemp2 <= 41.10)
						{
							pdr[(int)floor((dtemp1 - 115.40) / r * 1000)][(int)floor((dtemp2 - 39.40) / r * 1000)]->insert(k, n);
						}
						//不满足就保存到北京外
						else
						{
							pdor->insert(k, n);
						}
					}
				}
			}

			//根据区域保存信息进行用户的需求处理
			if (output) {
				cout << "n:" << n << endl;
				for (int i = 0; i < countrnumLong; i++)
				{
					for (int j = 0; j < countrnumLa; j++)
					{
						if (pdr[i][j]->getcountsize(n) >= 30)
						{
							cout << "pdr[" << i << "][" << j << "]" << endl;
							cout << pdr[i][j]->getcountsize(n) << endl;
						}
					}
				}
				cout << "podr:" << endl;
				cout << pdor->getcountsize(n) << endl;
			}
		}

		//功能执行完毕,删除全部临时反序列化信息和r区域信息
		for (int i = 0; i < countrnumLong; i++)
		{
			for (int j = 0; j < countrnumLa; j++)
			{
				delete pdr[i][j];
			}
			delete[]pdr[i];
		}
		delete[]pdr;
		delete pdor;
	}
}


//函数功能3实现
set<Point, CMP> function_region_association_analysis_one(double* pos1, double* pos2, int day, int hour, int minute, int time_span, bool output)
{
	//新建矩形1、2和经过两者的车流的对应信息对象
	set<Point, CMP>* information_rect1 = new set<Point, CMP>;
	set<Point, CMP>* information_rect2 = new set<Point, CMP>;
	set<Point, CMP>* information_rect_exchange = new set<Point, CMP>;

	//利用函数功能1,获得矩形1、2内的全部相关信息
	function_findtaxinum_somewhere(*information_rect1, pos1, day, hour, minute, time_span,false);
	function_findtaxinum_somewhere(*information_rect2, pos2, day, hour, minute, time_span,false);

	//taxicmp用于保存在两个矩形中都出现的出租车序号名称
	set<int> taxicmp;

	//遍历1矩形
	for (auto i : *information_rect1)
	{
		//先插入1的全部出租车序号名
		taxicmp.insert(i.gettaxiname());
	}

	//再遍历taxicmp(此时对应保存了1的全部出租车序号名信息)
	for (set<int>::iterator iter = taxicmp.begin(); iter != taxicmp.end();)
	{

		//做个标记
		int flag = 1;

		//如果能够在2中发现相关信息
		for (auto j : *information_rect2)
		{
			if (j.gettaxiname() == *iter)
			{
				//标记置零
				flag = 0;
			}
		}

		//没发现说明没有进行车流交换,标记此时置一
		if (flag)
		{
			//把没出现的序号名删除
			iter = taxicmp.erase(iter);
		}
		else iter++;
	}

	//此时taxicmp保存了全部的矩形1和2的交流出租车序号名,遍历它们
	for (auto i : taxicmp)
	{
		//遍历1矩阵,将和出现的序号名相关的点信息进行保存
		for (auto j : *information_rect1)
		{
			if (j.gettaxiname() == i)
			{
				(*information_rect_exchange).insert(j);
			}
		}
		//遍历2矩阵,将和出现的序号名相关的点信息进行保存
		for (auto j : *information_rect2)
		{
			if (j.gettaxiname() == i)
			{
				(*information_rect_exchange).insert(j);
			}
		}
	}
	//获得了全部的车流出租车轨迹点信息,进行用户相关需求输出
	if (output) {
		cout << "taxi exchange number: " << taxicmp.size() << endl;
		for (auto i : *information_rect_exchange) {
			cout << "taxi information: " << i.gettaxiname() << " " << i.getLongitude() << " " << i.getLatitude() << " " << i.gettimes() << endl;
		}
	}

	//功能调用结束,清空矩阵信息,腾出内存为其他功能使用
	information_rect1->clear();
	information_rect2->clear();
	delete information_rect1;
	delete information_rect2;
	return *information_rect_exchange;
}


//函数功能4实现
void function_region_association_analysis_two(double* pos, int day, int hour, int minute, int time_span, bool output)
{
	//新建要判断的矩形和经过该矩形及外界的车流的对应信息对象
	set<Point, CMP>* information_rect = new set<Point, CMP>;
	set<Point, CMP>* information_rect_exchange = new set<Point, CMP>;

	//利用函数功能1,获得判断矩形内的全部相关信息
	function_findtaxinum_somewhere(*information_rect, pos, day, hour, minute, time_span,false);

	//taxicount内保存判断矩形的全部出租车序号名
	set<int> taxicount;

	//taxicount_exchange内保存在判断矩形内出现又在外界出现的全部出租车序号名
	set<int> taxicount_exchange;

	//遍历判断矩阵,将和出现的序号名相关的点信息进行保存
	for (auto i : *information_rect)
	{
		taxicount.insert(i.gettaxiname());
	}

	//taxisize不是外面的头文件声明,是局部变量,对应判断矩阵内出租车序号名数量
	int taxisize = taxicount.size();

	//新建taxi数组,大小为判断矩阵内出租车序号名数量
	taxi* newvtaxi = new taxi[taxisize];
	int j = 0;
	
	//遍历taxicount的出租车序号名,将对应的出租车文件反序列化,获得该出租车全部信息
	for (auto i : taxicount) {
		newvtaxi[j++] = boost_iarchive_taxi(i);
	}

	//遍历获得的全部出租车信息,判断分析的时间段内的所有轨迹点是否没有出现在矩形内
	for (int i = 0; i < taxisize; i++)
	{
		int flag = 1;
		//遍历某个车的全部轨迹点
		for (int j = 0; j < newvtaxi[i].getsize(); j++)
		{
			//暂存点
			Point tppot = newvtaxi[i].getPoint(j);
			int timetemp = gettime(day, hour, minute, 0, tppot.getday(), tppot.gethour(), tppot.getminute(), tppot.getsecond());
			//暂存点如果在时间范围内
			if (timetemp < 0)
			{
				continue;
			}
			else if (timetemp >= 0 and timetemp <= time_span * 60)
			{
				//暂存点不在矩阵内,说明出现在了外界
				if (find((*information_rect).begin(), (*information_rect).end(), tppot) == (*information_rect).end())
				{
					//向交流矩阵插入点信息
					(*information_rect_exchange).insert(tppot);
					flag = 0;
				}
			}
			//如果是时间范围全部考虑完成,停止继续运行
			else
			{
				break;
			}
		}

		//遍历交流矩阵点
		for (auto i : *information_rect_exchange)
		{
			//每个点去对应矩形内的点
			for (auto j : *information_rect)
			{
				//如果是一个车的就把矩阵内点信息也插入交流矩阵内,相当于有了二次检验
				if (i.gettaxiname() == j.gettaxiname())
					(*information_rect_exchange).insert(j);
			}
		}

		//如果这个出租车序号名是第一次被保存到交流矩阵内,出租车序号名插入到交流的集合内
		if (!flag)
		{
			taxicount_exchange.insert(newvtaxi[i].getname());
		}
	}

	//根据用户需要输出相关信息
	if (output) {
		cout << "taxicount_exchange: " << taxicount_exchange.size() << endl;
		for (auto i : *information_rect_exchange)
		{
			cout << "taxi information: " << i.gettaxiname() << " " << i.getLongitude() << " " << i.getLatitude() << " " << i.gettimes() << endl;
		}
	}
	//功能调用结束,清空矩阵信息,腾出内存为其他功能使用
	information_rect->clear();
	information_rect_exchange->clear();
	delete information_rect;
	delete information_rect_exchange;
	delete[]newvtaxi;
}
