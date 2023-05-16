#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include "taxi_header_use.h"
#include "taxi_init.h"

//函数功能5:频繁路径分析1。一条路径的频繁度可以定义为这条路径上通行的汽车的总数。根据用户给定的参数k，距离参数x，统计在整个城市中，长度超过x的最为频繁的前k个路径
//传入参数x表示长度,k表示数量
//返回结点经纬度信息数组,每个容器的元素为一个容器,存储四个元素,分别对应了结点1的经度、纬度,结点2的经度和纬度
vector<vector<string>> function_path_frequent_analysis1(double x,int k, bool output)
{
	//判断k和x是否合理
	if (k >= 0 and k < 10000 and x >= 0) {
		
		//如果合理打开已经保存好的file文件(存储着pathEdge的对象)和class文件(存储相关信息)
		ifstream ifs1("D:\\pathdata\\pathsortedge_forcount_file", ios::in | ios::binary);
		ifstream ifs2("D:\\pathdata\\pathsortedge_forcount_class.txt", ios::in);

		//结果容器保存
		vector<pathEdge> result;

		//先将结果容器建立为k大小
		for (int i = 0; i < k; i++)
		{
			pathEdge temp;
			result.push_back(temp);
		}

		int i = 0;
		
		//保存满足要求的边对应的结点经纬度信息(目前无法写入读取对象的指针对象)
		vector<vector<string>> vstemp;

		//遍历全部pathEdge对象
		while (ifs1.read((char*)(&result[i]), sizeof(pathEdge)))
		{
			//获得当前边对应的结点经纬度和其他信息
			string stemp;
			getline(ifs2, stemp, '\n');

			//因其已经按照频繁度排序,依次向下,判断其长度是否要求
			if (result[i].getpathlen() >= x)
			{
				//符合要求时将每行结点经纬度信息保存到vstemp中
				vstemp.push_back(Split(stemp, ','));

				//++i表示已经存储了相关信息进行下一个
				i++;

			}
			//如果得到了k个符合要求的路径可以结束
			if (i == k) 
			{ break; }
		}
		if (output) {
			ofstream ofs("D:\\result\\function_path_frequent_analysis1.txt",ios::trunc);
			ofs << i << '\n';
			//遍历结果容器中的有效路径对象
			for (int j = 0; j < i; j++)
			{
				//输出结果容器中的有效信息和上面保存的结点经纬度信息
				ofs << setiosflags(ios::fixed) << setprecision(5)
					<< vstemp[j][0] << ","
					<< vstemp[j][1] << ","
					<< vstemp[j][2] << ","
					<< vstemp[j][3] << ","
					<< result[j].getpathcount() << ","
					<< result[j].getpathlen() << '\n';
			}
			ofs.close();
		}
		return vstemp;
	}

	//k和x输入有误则输出错误信息
	else 
	{
		ofstream ofs("D:\\result\\function_path_frequent_analysis1.txt", ios::trunc);
		ofs << -1 << '\n';
		ofs.close();
		vector<vector<string>> vstemp;

		//返回无效信息
		return vstemp;
	}
	
}


//函数功能6:频繁路径分析2。给定两个矩形区域A和B，分析从A到B的最为频繁的前k个通行路径。
//传入参数pos1表示矩形1,pos2表示矩形2,k表示数量
//返回结点经纬度信息数组,每个容器的元素为一个容器,存储四个元素,分别对应了结点1的经度、纬度,结点2的经度和纬度
vector<vector<double>> function_path_frequent_analysis2(double* pos1, double* pos2, int k, bool output)
{
	//将结果的前k个频繁路径收集
	vector<pathEdge> k_for;

	//用于返回的结点经纬度信息数组
	vector<vector<double>> result = {};

	//利用函数功能3，获得两个矩形的车流交换信息
	set<Point, CMP> information_rect_exchange = function_region_association_analysis_one(pos1, pos2, 2, 0, 0, 60 * 24 * 7, false);
	
	//存储车流交换的出租车名称
	set<int> taxi_exchange_name;

	//获得车流交换的出租车名称
	for (auto i : information_rect_exchange) 
	{
		taxi_exchange_name.insert(i.gettaxiname());
	}

	//建立出租车指针
	taxi* taxi_use = new taxi[taxi_exchange_name.size()];
	int index = 0;
	//对车流交换的出租车进行反序列化
	for (auto i : taxi_exchange_name) 
	{
		taxi_use[index++] = boost_iarchive_taxi2(i);
	}

	//建立路径的图,用于分析其他数据
	pathgraph p;

	//循环处理出租车
	for (int i = 0; i < taxi_exchange_name.size(); i++) {

		//标记是否有点在information_rect_exchange中出现过
		int flag = 0;

		//记录开始出现在交流矩阵的点位置,最终结束出现在交流矩阵的点位置,和每个出现点在出租车的对应位置
		pair<pair<int,int>, vector<int>> record = {};

		//循环出租车的每个点
		for(int j=0;j<taxi_use[i].getsize();j++)
		{
			Point temp = taxi_use[i].getPoint(j);

			//如果没出现过交流矩阵的点就舍弃
			if (!flag and information_rect_exchange.find(temp) == information_rect_exchange.end())
			{
				continue;
			}
			//第一次出现保留位置和信息,置标记为1
			else if (!flag and information_rect_exchange.find(temp) != information_rect_exchange.end())
			{
				flag = 1;
				record.first.first=j;
				record.second.push_back(j);
			}
			//之后每次出现都更新位置和信息,这样最后一次保存的就是最后一个出现点的位置了
			else if (flag and information_rect_exchange.find(temp) != information_rect_exchange.end())
			{
				record.first.second = j;
				record.second.push_back(j);
			}
			//否则什么也不做
			else continue;
		}
		//初始化每个车第一次在矩形中出现的点
		Point temp1 = taxi_use[i].getPoint(record.second[0]);
		//做这个车从开始到结束在矩形出现的点的全部信息
		for (int k = record.first.first; k <= record.first.second; k++)
		{
			//更新到图中
			Point temp2 = taxi_use[i].getPoint(k);
			p.pathgreaphupdate2(temp1, temp2);
		}
	}

	//获得图的全部边
	unordered_map<string, pathEdge*> maptemp = p.getpathEdgemap();

	//存储图的全部边及边的频繁度,并进行排序(用拉姆达表达式排序)
	vector<pair<int, pathEdge>> sortpathEdge;
	for (auto i : maptemp) {
		sortpathEdge.push_back({ (i.second)->getpathcount(),*(i.second) });
	}
	sort(sortpathEdge.begin(), sortpathEdge.end(), [](const pair<int, pathEdge>& a, const pair<int, pathEdge>& b) { return a.first > b.first; });
	
	//从频繁度最高的开始遍历
	int x = 0;
	for(auto i: sortpathEdge)
	{
		//判断两结点是否均在矩形内,如果在则将该边放入结果容器内
		double iLo1 = i.second.getpathNode1()->getLongitudeNode();
		double iLa1 = i.second.getpathNode1()->getLatitudeNode();
		double iLo2 = i.second.getpathNode2()->getLongitudeNode();
		double iLa2 = i.second.getpathNode2()->getLatitudeNode();
		if ((iLo1 >= pos1[0] and iLa1 >= pos1[2] and iLo1 <= pos1[1] and iLa1 <= pos1[3] and iLo2 >= pos2[0] and iLa2 >= pos2[2] and iLo2 <= pos2[1] and iLa2 <= pos2[3]) or (iLo2 >= pos1[0] and iLa2 >= pos1[2] and iLo2 <= pos1[1] and iLa2 <= pos1[3] and iLo1 >= pos2[0] and iLa1 >= pos2[2] and iLo1 <= pos2[1] and iLa1 <= pos2[3]))
		{
			k_for.push_back(i.second);
			result.push_back({ iLo1 ,iLa1,iLo2,iLa2 });
			x++;
			//存了k个就停止,否则全部遍历一遍
			if (x == k) { break; }
		}
	}

	//输出前k条路径的相关信息
	if (output) {
		ofstream ofs("D:\\result\\function_path_frequent_analysis2.txt", ios::trunc);
		ofs << x << '\n';
		for (auto i : k_for)
		{
			ofs << setiosflags(ios::fixed) << setprecision(5)
				<< i.getpathNode1()->getLongitudeNode() << ","
				<< i.getpathNode1()->getLatitudeNode() << ","
				<< i.getpathNode2()->getLongitudeNode() << ","
				<< i.getpathNode2()->getLatitudeNode() << ","
				<< i.getpathcount() << ","
				<< i.getpathlen() << '\n';

		}
		ofs.close();
	}

	//返回路径的结点数组
	return result;
}

//此为只分析相邻点的路径情况
vector<vector<string>> function_path_frequent_analysis2_neighbor(double* pos1, double* pos2, int k, bool output)
{
	//如果合理打开已经保存好的file文件(存储着pathEdge的对象)和class文件(存储相关信息)
	ifstream ifs1("D:\\pathdata\\pathsortedge_forcount_file", ios::in | ios::binary);
	ifstream ifs2("D:\\pathdata\\pathsortedge_forcount_class.txt", ios::in);

	//结果容器保存
	vector<pathEdge> result;

	//先将结果容器建立为k大小
	for (int i = 0; i < k; i++)
	{
		pathEdge temp;
		result.push_back(temp);
	}

	int i = 0;

	//保存满足要求的边对应的结点经纬度信息(目前无法写入读取对象的指针对象)
	vector<vector<string>> vstemp;

	//遍历全部pathEdge对象
	while (ifs1.read((char*)(&result[i]), sizeof(pathEdge)))
	{
		//获得当前边对应的结点经纬度和其他信息
		string stemp;
		getline(ifs2, stemp, '\n');

		//分割这些信息,用于判断是否在矩形内,信息存储于容器内
		vector<string> vvtemp = Split(stemp, ',');

		double iLo1 = stod(vvtemp[0]);
		double iLa1 = stod(vvtemp[1]);
		double iLo2 = stod(vvtemp[2]);
		double iLa2 = stod(vvtemp[3]);
		//因其已经按照频繁度排序,依次向下,判断其长度是否要求
		if ((iLo1 >= pos1[0] and iLa1 >= pos1[2] and iLo1 <= pos1[1] and iLa1 <= pos1[3] and iLo2 >= pos2[0] and iLa2 >= pos2[2] and iLo2 <= pos2[1] and iLa2 <= pos2[3]) or (iLo2 >= pos1[0] and iLa2 >= pos1[2] and iLo2 <= pos1[1] and iLa2 <= pos1[3] and iLo1 >= pos2[0] and iLa1 >= pos2[2] and iLo1 <= pos2[1] and iLa1 <= pos2[3]))
		{

			//符合要求时将每行结点经纬度信息保存到vstemp中
			vstemp.push_back(vvtemp);
			
			//++i表示已经存储了相关信息进行下一个,此时result[i]的结果固定了
			i++;

		}
		//如果得到了k个符合要求的路径可以结束
		if (i == k)
		{
			break;
		}
	}
	if (output) {
		ofstream ofs("D:\\result\\function_path_frequent_analysis2_neighbor.txt", ios::trunc);
		ofs << i << '\n';
		//遍历结果容器中的有效路径对象
		for (int j = 0; j < i; j++)
		{
			//输出结果容器中的有效信息和上面保存的结点经纬度信息
			ofs << setiosflags(ios::fixed) << setprecision(5)
				<< vstemp[j][0] << ","
				<< vstemp[j][1] << ","
				<< vstemp[j][2] << ","
				<< vstemp[j][3] << ","
				<< result[j].getpathcount() << ","
				<< result[j].getpathlen() << '\n';
		}
		ofs.close();
	}
	return vstemp;
}


//函数功能7:通信时间分析。给定两个矩形区域A和B，分析在不同的时间段，出租车从A到B的通行时间最短的路径，以及相应的通行时间。
//传入参数pos1表示矩形1,pos2表示矩形2,day hour minute表示起始时间,time_span表示时间范围
//返回一系列的点, 从最短时间的开始点出发到结束点之间的全部点表示一条明确的路径
vector<Point> function_pathuse_shortesttime(double* pos1, double* pos2, int day, int hour, int minute, int time_span, bool output)
{
	//利用函数功能3，获得在要求时段内两个矩形的车流交换信息
	set<Point, CMP> information_rect_exchange = function_region_association_analysis_one(pos1, pos2, day, hour, minute, time_span,false);

	//定义最短时间对应的起点和终点
	Point a, b;

	//定义最短时间,开始时为int类型最大值
	int shortest_time = INT_MAX;

	//定义交换矩阵vector版本,主要是用于遍历
	vector<Point> information_rect_exchange_use = {};
	
	//定义结果保存容器
	vector<Point> result = {};

	//遍历交换矩阵,将信息保存到vector版本中
	for (const auto& i : information_rect_exchange)
	{
		information_rect_exchange_use.push_back(i);
	}

	//遍历vector版本的交换矩阵点信息(可以一次用两个)
	for (auto j = information_rect_exchange_use.begin(); j != information_rect_exchange_use.end();)
	{
		//j为后一个点,i为前一个点
		auto i = j++;

		//如果j没到结束出就进行分析
		if (j != information_rect_exchange_use.end()) {

			//两个点为同一个出租车的轨迹点
			if (i->gettaxiname() == j->gettaxiname())
			{
				//获得两个点的经纬度
				double iLo1 = i->getLongitude();
				double iLa1 = i->getLatitude();
				double iLo2 = j->getLongitude();
				double iLa2 = j->getLatitude();

				//判断两个点在这两个矩形内且不在同一矩形内
				if ((iLo1 >= pos1[0] and iLa1 >= pos1[2] and iLo1 <= pos1[1] and iLa1 <= pos1[3] and iLo2 >= pos2[0] and iLa2 >= pos2[2] and iLo2 <= pos2[1] and iLa2 <= pos2[3]) or (iLo2 >= pos1[0] and iLa2 >= pos1[2] and iLo2 <= pos1[1] and iLa2 <= pos1[3] and iLo1 >= pos2[0] and iLa1 >= pos2[2] and iLo1 <= pos2[1] and iLa1 <= pos2[3]))
				{
					//判断两点的时间差
					int temptime = gettime(i->getday(), i->gethour(), i->getminute(), i->getsecond(), j->getday(), j->gethour(), j->getminute(), j->getsecond());

					//更新最短时间和对应点
					if (shortest_time > temptime) {
						shortest_time = temptime;
						a = *i;
						b = *j;
					}
				}
			}
		}
	}

	//定义一个出租车反序列化,对应的是最短路径的轨迹点出租车序号名
	taxi t = boost_iarchive_taxi2(a.gettaxiname());

	//置标记为0
	int flag = 0;

	//遍历出租车轨迹点
	for (int i = 0; i < t.getsize(); i++) 
	{
		//如果没有发现起点就一直寻找起点
		if (!flag) 
		{
			//找到起点时
			if (a == t.getPoint(i))
			{
				//标记置1
				flag = 1;

				//起点放入结果容器
				result.push_back(a);
			}
		}

		//如果找到了起点且找到了终点
		else if (flag and b == t.getPoint(i))
		{
			//终点放入结果容器
			result.push_back(b);

			//停止分析
			break;
		}

		//如果只找到了起点不是终点
		else 
		{
			//将这些中间点也存入结果容器,形成一个完整路径
			result.push_back(t.getPoint(i));
		}
	}
	//输出最短时间信息
	if (output) {
		ofstream ofs("D:\\result\\function_pathuse_shortesttime.txt", ios::trunc);
		ofs  << shortest_time << '\n';
		for (auto i : result)
		{
			ofs << setiosflags(ios::fixed) << setprecision(5) << i.gettaxiname() << "," << i.getLongitude() << "," << i.getLatitude() << "," << i.getday() << "," << i.gethour() << "," << i.getminute() << "," << i.getsecond() << '\n';
		}
		ofs.close();
	}
	//返回结果容器
	return result;
}