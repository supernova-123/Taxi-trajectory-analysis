#include <iostream>
#include <cmath>
#include <fstream>
#include <unordered_map>
#include "taxi_header_use.h"

//pathNode

//构造函数,将结点的对应经纬度初始化(精确到0.001度)
pathNode::pathNode(double d1, double d2) :LongitudeNode(d1), LatitudeNode(d2) {}

//返回结点经度
double pathNode::getLongitudeNode()
{
	return LongitudeNode;
}

//返回结点纬度
double pathNode::getLatitudeNode()
{
	return LatitudeNode;
}


//pathEdge

//无参构造函数,用于反序列化使用
pathEdge::pathEdge() :len(0), shortesttime(0), p1(NULL), p2(NULL), count(0) {}


//构造函数,根据传入的两个轨迹点和其所在的结点进行构造(此处传入值已经经过预处理,判断出不是同一结点或者轨迹点)
pathEdge::pathEdge(Point a, Point b, pathNode* po1, pathNode* po2) :count(1), p1(po1), p2(po2)
{
	//floor向下取整,将精确度更高的点表示所有在该0.001精确度下
	double LongitudeNode1 = floor(a.getLongitude() * 1000) / 1000;
	double LongitudeNode2 = floor(b.getLongitude() * 1000) / 1000;
	double LatitudeNode1 = floor(a.getLatitude() * 1000) / 1000;
	double LatitudeNode2 = floor(b.getLatitude() * 1000) / 1000;

	//计算传入两点经过时间,因只有这一对点所以默认其为最短时间
	shortesttime = gettime(a.getday(), a.gethour(), a.getminute(), a.getsecond(), b.getday(), b.gethour(), b.getminute(), b.getsecond());

	//计算两点的物理距离
	len = 111194.9266 * sqrt(pow(LongitudeNode2 - LongitudeNode1, 2) + pow((LatitudeNode2 - LatitudeNode1) * lodividela, 2));
	
	//保存传入两点作为最短时间的起点和终点
	shortesttime_taxistart = a;
	shortesttime_taxiend = b;
}

//边更新函数,根据传入的两个轨迹点进行经过次数的更新和最短时间的更新和最短时间所用结点的更新
void pathEdge::pathEdgeupdate(Point a, Point b)
{
	//计算传入两点的经过时间
	int time = gettime(a.getday(), a.gethour(), a.getminute(), a.getsecond(), b.getday(), b.gethour(), b.getminute(), b.getsecond());
	
	//边经过次数+1
	count++;

	//比较传入的时间和原有的最短时间,如果前者更短就更新最短时间和对应起点、终点
	if (shortesttime > time)
	{
		shortesttime = time;
		shortesttime_taxistart = a;
		shortesttime_taxiend = b;
	}
	return;
}

//返回边的一个结点
pathNode* pathEdge::getpathNode1()
{
	return p1;
}

//返回边的另一个结点
pathNode* pathEdge::getpathNode2()
{
	return p2;
}

//返回边被经过的次数
int pathEdge::getpathcount()
{
	return count;
}

//返回经过该边的最短时间
int pathEdge::getpathshortesttime()
{
	return shortesttime;
}

//返回该边的长度
double pathEdge::getpathlen()
{
	return len;
}

//输出该边的全部信息
void pathEdge::printpath_information()
{
	cout << "path:" << endl;
	cout << "Node1Lo:" << p1->getLongitudeNode() << endl;
	cout << "Node1La" << p1->getLatitudeNode() << endl;
	cout << "Node2Lo:" << p2->getLongitudeNode() << endl;
	cout << "Node2La:" << p2->getLatitudeNode() << endl;
	cout << "path count:" << count << endl;
	cout << "path len:" << len << endl;
	cout << "path shortesttime:" << this->getpathshortesttime() << endl << endl;
	return;
}

//函数功能5专用的输出该边的信息
void pathEdge::printpath_information2(double d1,double d2,double d3,double d4)
{
	cout << "path:" << endl;
	cout << "Node1Lo:" << d1 << endl;
	cout << "Node1La" << d2 << endl;
	cout << "Node2Lo:" << d3 << endl;
	cout << "Node2La:" << d4 << endl;
	cout << "path count:" << count << endl;
	cout << "path len:" << len << endl << endl;
}


//pathgraph

//无参构造函数,不进行任何操作
pathgraph::pathgraph() {}

//析构函数,将两个map的元素清空以腾出内存继续其他分析
pathgraph::~pathgraph()
{
	unordered_map<string, pathNode*>temp1;
	pathNodemap.swap(temp1);
	unordered_map<string, pathEdge*>temp2;
	pathEdgemap.swap(temp2);
}

//更新函数,根据传入出租车进行对图的更新
void pathgraph::pathgraphupdate(taxi& t)
{
	//根据出租车相邻时间轨迹点逐个分析
	for (int i = 0; i < t.getsize() - 1; i++)
	{
		//获得相邻两点
		Point a = t.getPoint(i);
		Point b = t.getPoint(i + 1);

		//计算相邻两点各自应该所在的结点
		double LongitudeNode1 = floor(a.getLongitude() * 1000) / 1000;
		double LongitudeNode2 = floor(b.getLongitude() * 1000) / 1000;
		double LatitudeNode1 = floor(a.getLatitude() * 1000) / 1000;
		double LatitudeNode2 = floor(b.getLatitude() * 1000) / 1000;
		
		//判断是否在同一结点上,如果在就继续判断下一组轨迹点
		if (LongitudeNode1 == LongitudeNode2 and LatitudeNode1 == LatitudeNode2)
		{
			continue;
		}

		//否则为这两个轨迹点连接结点
		pathNode* p1;
		pathNode* p2;
		
		//创建结点名称
		string flagNode1 = to_string(LongitudeNode1) + "-" + to_string(LatitudeNode1);
		string flagNode2 = to_string(LongitudeNode2) + "-" + to_string(LatitudeNode2);
		
		//查询结点表是否存在对应结点,如存在则指向已有结点,如不存在则创建新结点并插入到结点表中
		auto j1 = pathNodemap.find(flagNode1);
		auto j2 = pathNodemap.find(flagNode2);
		if (j1 == pathNodemap.end())
		{
			p1 = new pathNode(LongitudeNode1, LatitudeNode1);
			pathNodemap.insert({ flagNode1 ,p1 });
		}
		else
		{
			p1 = (*j1).second;
		}
		if (j2 == pathNodemap.end())
		{
			p2 = new pathNode(LongitudeNode2, LatitudeNode2);
		}
		else
		{
			p2 = (*j2).second;
			pathNodemap.insert({ flagNode2 ,p2 });
		}

		//建立边名称,查询边的信息表是否已存在这样的边(由无向边,两个方向都要查),如果存在直接将两个轨迹点和结点传入存在的边进行信息更新,不存在则新建一个边插入到边的信息表中,利用传入的结点和轨迹点进行初始化
		string flagpath1 = to_string(LongitudeNode1) + "-" + to_string(LatitudeNode1) + "-" + to_string(LongitudeNode2) + "-" + to_string(LatitudeNode2);
		string flagpath2 = to_string(LongitudeNode2) + "-" + to_string(LatitudeNode2) + "-" + to_string(LongitudeNode1) + "-" + to_string(LatitudeNode1);
		auto j3 = pathEdgemap.find(flagpath1);
		if (j3 == pathEdgemap.end())
		{
			auto k = pathEdgemap.find(flagpath2);
			if (k == pathEdgemap.end()) {
				pathEdge* p = new pathEdge(a, b, p1, p2);
				pathEdgemap.insert({ flagpath1,p });
			}
			else
			{
				k->second->pathEdgeupdate(a, b);
			}
		}
		else
		{
			j3->second->pathEdgeupdate(a, b);
		}
	}
	return;
}
//函数功能6的辅助函数,根据传入的点信息进行对图信息的更新
void pathgraph::pathgreaphupdate2(Point a, Point b)
{
	//计算相邻两点各自应该所在的结点
	double LongitudeNode1 = floor(a.getLongitude() * 1000) / 1000;
	double LongitudeNode2 = floor(b.getLongitude() * 1000) / 1000;
	double LatitudeNode1 = floor(a.getLatitude() * 1000) / 1000;
	double LatitudeNode2 = floor(b.getLatitude() * 1000) / 1000;

	//判断是否在同一结点上,如果在就结束更新
	if (LongitudeNode1 == LongitudeNode2 and LatitudeNode1 == LatitudeNode2)
	{
		return;
	}

	//否则为这两个轨迹点连接结点
	pathNode* p1;
	pathNode* p2;

	//创建结点名称
	string flagNode1 = to_string(LongitudeNode1) + "-" + to_string(LatitudeNode1);
	string flagNode2 = to_string(LongitudeNode2) + "-" + to_string(LatitudeNode2);

	//查询结点表是否存在对应结点,如存在则指向已有结点,如不存在则创建新结点并插入到结点表中
	auto j1 = pathNodemap.find(flagNode1);
	auto j2 = pathNodemap.find(flagNode2);
	if (j1 == pathNodemap.end())
	{
		p1 = new pathNode(LongitudeNode1, LatitudeNode1);
		pathNodemap.insert({ flagNode1 ,p1 });
	}
	else
	{
		p1 = (*j1).second;
	}
	if (j2 == pathNodemap.end())
	{
		p2 = new pathNode(LongitudeNode2, LatitudeNode2);
	}
	else
	{
		p2 = (*j2).second;
		pathNodemap.insert({ flagNode2 ,p2 });
	}

	//建立边名称,查询边的信息表是否已存在这样的边(由无向边,两个方向都要查),如果存在直接将两个轨迹点和结点传入存在的边进行信息更新,不存在则新建一个边插入到边的信息表中,利用传入的结点和轨迹点进行初始化
	string flagpath1 = to_string(LongitudeNode1) + "-" + to_string(LatitudeNode1) + "-" + to_string(LongitudeNode2) + "-" + to_string(LatitudeNode2);
	string flagpath2 = to_string(LongitudeNode2) + "-" + to_string(LatitudeNode2) + "-" + to_string(LongitudeNode1) + "-" + to_string(LatitudeNode1);
	auto j3 = pathEdgemap.find(flagpath1);
	if (j3 == pathEdgemap.end())
	{
		auto k = pathEdgemap.find(flagpath2);
		if (k == pathEdgemap.end()) {
			pathEdge* p = new pathEdge(a, b, p1, p2);
			pathEdgemap.insert({ flagpath1,p });
		}
		else
		{
			k->second->pathEdgeupdate(a, b);
		}
	}
	else
	{
		j3->second->pathEdgeupdate(a, b);
	}
	return;
}

//将结点信息表返回
unordered_map<string, pathNode*> pathgraph::getpathNodemap() { return pathNodemap; }

//将边的信息表返回
unordered_map<string, pathEdge*> pathgraph::getpathEdgemap() { return pathEdgemap; }

//输出图的基本信息和一些边的基本信息
void pathgraph::printgetpathNodemap() 
{
	cout << pathNodemap.size() << endl;
	cout << pathEdgemap.size() << endl;
	for (auto i : pathEdgemap) 
	{
		if (i.second->getpathcount() >= 10)
		{
			cout << i.first << ": " << i.second->getpathcount() << " " << i.second->getpathlen() << " " << i.second->getpathshortesttime() << endl;
		}
	}
	return;
}


void pathgraph::function_path_frequent_analysis_init()
{
	//将图中的全部边信息存入vector容器中
	vector<pair<string, pathEdge*>> sortedge = {};
	for (auto i : pathEdgemap) {
		sortedge.push_back(i);
	}

	//对vector容器内的边按照频繁度进行排序,排序函数通过lambda表达式进行参数捕捉和分析
	sort(sortedge.begin(), sortedge.end(), [](pair<string, pathEdge*>& a, pair<string, pathEdge*>& b) {return a.second->getpathcount()> b.second->getpathcount(); });

	//打开两个输入文件对象,进行信息写入
	ofstream ofs1("E:\\pathdata\\pathsortedge_forcount_class.txt", ios::trunc);
	
	for (auto i: sortedge)
	{
		//ofs1负责写入结点经纬度和边的频繁度、长度和最短时间,分行输入
		ofs1 << (i.second->getpathNode1())->getLongitudeNode() << "," << (i.second->getpathNode1())->getLatitudeNode() << "," << (i.second->getpathNode2())->getLongitudeNode() << "," << (i.second->getpathNode2())->getLatitudeNode() << "," << i.second->getpathcount() << "," << i.second->getpathlen() << "," << i.second->getpathshortesttime() << "\n";
		
		
		
	}

	//先关闭ofs1,再进行ofs2,以免出现缓冲区冲突
	ofs1.close();

	//ofs2负责写入边对象的相关信息
	ofstream ofs2("E:\\pathdata\\pathsortedge_forcount_file", ios::out | ios::binary);
	for (auto i : sortedge) 
	{
		ofs2.write((char*)(i.second), sizeof(pathEdge));
	}
	//关闭ofs2
	ofs2.close();
	return;
}