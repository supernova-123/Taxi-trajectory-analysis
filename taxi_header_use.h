#ifndef TAXI_HEADER_H
#define TAXI_HEADER_H

//包含了下方定义的类所使用的头文件
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/string.hpp> 
using namespace std;

//声明各种变量：
extern const double lodividela;    //北京纬度和经度的比值,用于计算实际距离时作为参数因子使用
extern const int taxisize;         //需要考虑的出租车数量,本数据集对应10357辆出租车
extern const double inBeijing_Longitude_min; //北京范围的最小经度;
extern const double inBeijing_Longitude_max; //北京范围的最大经度;
extern const double inBeijing_Latitude_min;  //北京范围的最小纬度;
extern const double inBeijing_Latitude_max;  //北京范围的最大纬度;
extern const int changetime;       //对功能函数划分r*r大小格子时考虑这个格子——
								   //——所容纳的出租车对应的时间间隔

extern const double r_for_distance; //r的基本单位距离
/*对各种变量名称的说明:
*   Point:
	taxiname/i:出租车的序号名
	times/s1:时间数组,分别保存了日、时、分和秒,s1是字符串形式,times是int数组类型
	Longitude/s2:经度,前者double,后者string
	Latitude/s3:纬度,前者double,后者string

*   taxi:
	name:出租车的序号名
	size:存储的Point的数量
	pv:指针,指向有效Point数组
	vs:一个出租车的全部点信息经有效分解后的结果
	i/pos:出租车在数组中的位置

*   CMP：无说明

*	time_distribution:
	timemap：哈希表,存储键-值对,键信息为出租车序号名称,值信息为该时段内全部点信息
	vt:用于初始化的出租车
	i1、i2、i3:分别对应该对象需要存储的日、时、分、秒
	n:被查询的出租车序号名
	day、hour、minute:被查询的时间值
	p:double数组,大小为4,第一个为查询范围的左上角经度、第二个为查询范围的右下角经度、第三个为查询范围的左上角纬度、第四个为查询范围的右下角纬度
	countnum:统计多个类对象中出现出租车总数量信息
	taxi_haveuse:bool [taxisize]数组,在对应的位置标注了出租车是否被统计过

*	position_distribution_inBeiJing:
	posmap：哈希表,存储键-值对,键信息为出租车序号名称,值信息为该空间范围内全部点信息
	init_Longitude:该类对象统计的空间起始经度
	init_Latitude:该类对象统计的空间起始纬度
	vt:用于初始化的出租车
	d1,d2:被初始化的空间范围的起始经度和纬度
	n:被查询的出租车序号名
	day、hour、minute:被查询的时间范围的起始值
	p:double数组,大小为4,第一个为查询范围的左上角经度、第二个为查询范围的右下角经度、第三个为查询范围的左上角纬度、第四个为查询范围的右下角纬度
	countnum:统计多个类对象中出现出租车总数量信息
	time_span:所需要扫描的最大时间范围(即使和上面的起始值相加超过结束时间也可以,已经进行了限制)
	taxi_haveuse:bool [taxisize]数组,在对应的位置标注了出租车是否被统计过

*	position_distribution_outBeiJing:
	vt:用于初始化的出租车
	n:被查询的出租车序号名
*/

//出租车轨迹点类,包括时空信息和车名
class Point 
{
	//侵入式序列化和反序列化,声明序列化库内类的友元,授予其访问权限进行序列化
	friend class boost::serialization::access;

private:
	
	//Point成员包括出租车的序号名,出现的轨迹点的日、时、分、秒,及经度和纬度
	int taxiname;
	int times[4];
	double Longitude;
	double Latitude;

	//声明序列化和反序列化的执行函数
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& taxiname;
		ar& times;
		ar& Longitude;
		ar& Latitude;
	}
public:

	//Point的无参构造函数:
	Point();
	
	//Point的有参构造函数:
	Point(int i1, int i2, int i3, int i4, int i5, double d1, double d2);
	
	//Point的初始化函数,此只用于序列化时对数据集分析结果的处理,以后直接反序列化即可,不会再用:
	void Point_init(int i, string& s1, string& s2, string& s3);
	//另一个初始化函数,此用于文件读写含有点信息时,使用频繁
	void Point_init2(int i1, int i2, int i3, int i4, int i5, double d1, double d2);

	//获得出租车轨迹点序号名函数
	int gettaxiname();

	//获得出租车轨迹点经度函数
	double getLongitude();
	
	//获得出租车轨迹点纬度函数
	double getLatitude();
	
	//获得出租车轨迹点的日期、小时、分钟、秒信息
	int getday();
	int gethour();
	int getminute();
	int getsecond();

	//以字符串形式,返回出租车轨迹点时间信息,用于输出时间便于用户理解
	string gettimes();

	//出租车轨迹点全信息输出:
	void printPoint();

	//出租车轨迹点的比较,用于判断是否是同一个点
	bool operator== (const Point& p) const;
};


//出租车类,以点为单位组合成为一辆出租车
class taxi
{
	friend class boost::serialization::access;

private:
	
	//出租车成员包括出租车序号名,出租车内的有效点的数量和具体的点信息
	int name;
	int size;
	Point* pv;
	
	//序列化函数定义, 注意taxi中的pv是动态数组, 序列化和反序列化也需要动态进行, 并逐个进行
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& name;
		ar& size;
		if (pv == 0)
		{
			pv = new Point[size];
		}
		for (int i = 0; i < size; ++i) {
			ar& pv[i];
		}
	}

public:
	
	//taxi的无参构造函数
	taxi();

	//taxi的拷贝构造函数
	taxi(const taxi& t);

	//taxi的operator=运算符
	taxi operator=(const taxi& t);

	//taxi的析构
	~taxi();

	//taxi初始化函数,第一个是序列化和文件写入使用,第二个是文件读取使用(反序列化不需要函数)
	void taxi_init(vector<vector<string>>& vs);
	void taxi_init2(vector<vector<string>>& vs);

	//taxi的各类信息返回函数,依次为点数量,车序号名,某点的经度,某点的纬度,某个具体点
	int getsize();
	int getname();
	double getLongitude2(int i);
	double getLatitude2(int i);
	Point& getPoint(int pos);

	//taxi的全部信息输出
	void printtaxi_track();

};


//特殊的比较函数,用于对出租车轨迹点排序
class CMP {
public:
	bool operator() (Point a, Point b)const;
};


//时间分布类,以分钟为单位将所有出租车信息分时整理
class time_distribution
{
	friend class boost::serialization::access;

private:
	
	//用一个无序哈希表进行对某个时刻的点信息存储
	//键值为出租车序号名,值为该出租车对应时间的全部点信息
	unordered_map<int, vector<Point>> timemap;
	
	//序列化+反序列化
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& timemap;
	}

public:
	
	//无参构造函数
	time_distribution();
	
	//析构:
	~time_distribution();

	//时间分布类的初始化函数,因不断传入出租车更新信息,取名为update
	void time_distribution_update(taxi& vt, int i1, int i2, int i3);
	
	//时间分布类的文件读取初始化函数
	void time_distribution_init(vector<vector<string>>& vs);
	
	//输出时间分布类该时刻的全部信息:
	void print_time_distribution();
	
	//获得某个出租车在该时刻的全部信息
	pair<int, vector<Point>>& get_onetime_onetaxi(int n);

	//返回该时间段内的全部出租车的全部信息
	unordered_map<int, vector<Point>> getallpoint();

	//以时间为划分依据,查询在某个范围内的出租车轨迹点信息,countnum和taxi_haveuse用于保存出现的总数量和哪些出租车出现
	set<Point, CMP> counttaxi(int day, int hour, int minute, double* p, int& countnum, bool* taxi_haveuse);
};


//北京的空间分布类,以0.01*0.01经纬度变化划为一个类
class position_distribution_inBeiJing
{

	friend class boost::serialization::access;

private:

	//用一个无序哈希表进行对某个空间范围的点信息存储
	//键值为出租车序号名,值为该出租车对应空间范围的全部点信息
	unordered_map<int, vector<Point>> posmap;

	//该空间范围的起始经度和纬度
	double init_Longitude;
	double init_Latitude;

	//序列化+反序列化
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& posmap;
		ar& init_Longitude;
		ar& init_Latitude;
	}

public:

	//无参构造函数
	position_distribution_inBeiJing();

	//析构函数
	~position_distribution_inBeiJing();

	//空间分布类的初始化函数,因不断传入出租车更新信息,取名为update
	void position_distribution_inBeiJing_update(taxi& vt, double d1, double d2);

	//空间分布类的另一个初始化函数,因不断传入点的更新信息,取名为update2
	void position_distribution_inBeiJing_update2(Point& t, double d1, double d2);

	//空间分布类的文件读取初始化函数
	void position_distribution_inBeiJing_init(vector<vector<string>>& vs);

	//输出空间分布类该空间范围内的全部信息:
	void print_position_distribution_inBeiJing();

	//获得对应经度
	double getinit_Longitude();

	//获得对应纬度
	double getinit_Latitude();

	//返回该空间内的全部出租车的全部信息
	unordered_map<int, vector<Point>> getallpoint();

	//获得某个出租车在该空间范围内的全部信息
	pair<int, vector<Point>>& get_onetime_onetaxi(int n);

	//以空间为划分依据,查询在某个范围内,某段时间内的出租车轨迹点信息,countnum和taxi_haveuse用于保存出现的总数量和哪些出租车出现
	set<Point, CMP> counttaxi(int day, int hour, int minute, double* p, int& countnum, int time_span, bool* taxi_haveuse);

};


//北京外的空间分布类,整体信息少,作为一个整体使用,是对上一个类的补充
class position_distribution_outBeiJing
{

	friend class boost::serialization::access;

private:

	//只有一个无序哈希表进行对某个空间范围的点信息存储
	//键值为出租车序号名,值为该出租车对应空间范围的全部点信息
	unordered_map<int, vector<Point>> posmap;

	//序列化+反序列化
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& posmap;
	}
	
public:

	//无参构造函数
	position_distribution_outBeiJing();

	//析构函数
	~position_distribution_outBeiJing();
	
	//空间分布类的初始化函数,因不断传入出租车更新信息,取名为update
	void position_distribution_outBeiJing_update(taxi& vt);

	//空间分布类的文件读取初始化函数
	void position_distribution_outBeiJing_init(vector<vector<string>>& vs);

	//输出空间分布类该空间范围内的全部信息:
	void print_position_distribution_outBeiJing();

	//获得该空间范围内全部出租车的全部点信息:
	unordered_map<int, vector<Point>> getallpoint();
	
	//获得某个出租车在该空间范围内的全部信息
	pair<int, vector<Point>>& get_onetime_onetaxi(int n);

};


//路径结点类,用于存储对应路径的大致被划分到空间位置信息
class pathNode
{

	friend class boost::serialization::access;

private:
	
	//结点存储路径的对应大概经纬度(精确到0.001度)
	double LongitudeNode;
	double LatitudeNode;
	
	//序列化+反序列化
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& LongitudeNode;
		ar& LatitudeNode;
	}
public:
	
	//结点构造函数,存储其所在的经纬度
	pathNode(double d1 = 0, double d2 = 0);

	//获得结点经度
	double getLongitudeNode();

	//获得结点纬度
	double getLatitudeNode();

};


//路径边类,用于存储相邻路径的空间信息
class pathEdge
{

	friend class boost::serialization::access;

private:

	//p1、p2分别为边所用的两个结点(无向)
	pathNode* p1;
	pathNode* p2;

	//该边多次经过时所花费的最短时间
	int shortesttime;

	//该边的权值,即为该边被经过的次数,对应了频繁度
	int count = 0;

	//该边的第二权值,即为该边对应的物理长度
	double len;
	
	//获得最短时间时所用的起始点和终点(有向)
	Point shortesttime_taxistart;
	Point shortesttime_taxiend;

	//序列化+反序列化
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& p1;
		ar& p2;
		ar& len;
		ar& shortesttime;
		ar& count;
		ar& shortesttime_taxistart;
		ar& shortesttime_taxiend;
	}

public:

	//无参构造函数,用于反序列化使用
	pathEdge();

	//构造函数,根据传入的两个轨迹点和其所在的结点进行构造(此处传入值已经判断不会是同一结点或者轨迹点)
	pathEdge(Point a, Point b, pathNode* po1, pathNode* po2);

	//更新函数,根据传入的两个轨迹点进行经过次数的更新和最短时间的更新和最短时间所用结点的更新
	void pathEdgeupdate(Point a, Point b);

	//初始化函数,根据文件的读取信息进行初始化
	void pathEdge_init(double d1,double d2,double d3,double d4,int i5,double d6,int i7,int  i8,double i9,double i10,int i11,int i12,int i13,int i14,int i15,double i16,double i17,int i18,int i19,int i20,int i21);


	//获得边所用的两个结点中的一个
	pathNode* getpathNode1();
	pathNode* getpathNode2();

	//获得开始最短时间点和结束最短时间点
	Point getshortesttime_taxistart();
	Point getshortesttime_taxiend();

	//获得经过该边的次数
	int getpathcount();

	//获得经过该边的最短时间
	int getpathshortesttime();

	//获得该边的物理长度
	double getpathlen();

	//输出该边的信息
	void printpath_information();

	//函数功能5专用的输出该边的信息
	void printpath_information2(double d1,double d2,double d3,double d4);
};


//路径图类,用于存储所有节点和边,构成一个图
class pathgraph
{

	friend class boost::serialization::access;

private:

	//用一个无序哈希表进行全部出租车的结点信息存储
	//键值为由结点位置生成的字符串(结点生成字符串唯一),值为该结点形成的类信息
	unordered_map<string, pathNode*> pathNodemap;

	//再用一个无序哈希表进行全部出租车的边信息存储
	//键值为由边生成的字符串(无向边生成字符串唯一),值为该边形成的类信息
	unordered_map<string, pathEdge*> pathEdgemap;

	//序列化+反序列化
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& pathNodemap;
		ar& pathEdgemap;
	}

public:

	//图的无参构造函数
	pathgraph();

	//析构函数
	~pathgraph();

	//根据传入的出租车进行对图信息的更新
	void pathgraphupdate(taxi& t);

	//图的文件读取初始化函数
	void pathgraph_init(vector<vector<string>>& vs);
	
	//函数功能6的辅助函数,根据传入的点信息进行对图信息的更新
	void pathgreaphupdate2(Point p1, Point p2);

	//返回全部结点信息
	unordered_map<string, pathNode*> getpathNodemap();
	
	//返回全部边信息
	unordered_map<string, pathEdge*> getpathEdgemap();

	void printgetpathNodemap();

	//对图进行序列化
	void pathserialization();

	//文件版本
	void pathserialization2();

	//函数功能5的辅助函数 
	void function_path_frequent_analysis_init();
};   


//r格子类,用于某个将r*r的格子的各段时间的信息存储到一起,r的基本单位是0.001经纬度
class position_distribution_r
{

private:
	
	//r的指定大小,如r=10则每格子大小为0.01*0.01
	static int r;

	//将时间段除以要分析的时间变化单位
	static int vsize;

	//将单个格子各时段出现的出租车序号名存入其对应的一个容器中,此容器为全分析时段格子容器的集合,且每个容器的size大小对应了单个格子每时段出现的出租车次数,用于统计车流密度
	vector<vector<int>> taxijudge;
	
	//和上面的逻辑类似,但是存储的是单个格子每时段出现的全部出租车轨迹点信息,用于分析车流情况
	vector<set<Point, CMP>> information_regional_density;

public:

	//构造函数,不是无参函数
	position_distribution_r(int i, int j);
	
	//析构函数
	~position_distribution_r();

	//返回所有时段出租车出现次数之和
	int getallcount();

	//返回该类内共有多少个时段
	int getvsize();

	//返回该时段内某个时段该区域的出租车序号名集合
	vector<int> gettaxijudge(int i);

	//返回该时段内某个时段该区域的出租车数量
	int getcountsize(int i);
	
	//返回该时段内某个时段该区域的全部出租车信息
	set<Point, CMP> getallPoint(int i);

	//向该类对应时段内插入符合要求的轨迹点
	void insert(Point p, int i);

};


//上面的只考虑北京内,该类将北京外信息
//统合到一起作为整体考虑
class position_distribution_outr
{

private:

	//将时间段除以要分析的时间变化单位
	int vsize;
	
	//判断出租车是否被统计过
	bool taxi_haveuse[10357] = { 0 };
	
	//存储的是北京外每时段出现的出租车次数(单个车只统计一次),用于统计车流密度
	vector<int> count;

	//和上面的逻辑类似,但是存储的是北京外每时段出现的全部出租车轨迹点信息,用于分析车流情况
	vector<set<Point, CMP>> information_regional_density;

public:

	//构造函数,传入要分析的时段数
	position_distribution_outr(int i);

	//析构函数
	~position_distribution_outr();

	//返回所有时段出租车出现次数之和
	int getallvcount();

	//返回该类内共有多少个时段
	int getvsize();

	//返回该时段内某个时段北京外的出租车数量
	int getcountsize(int i);

	//返回该时段内某个时段北京外的全部出租车信息
	set<Point, CMP> getallPoint(int i);
	
	//向该类对应时段内插入符合要求的轨迹点
	void insert(Point p, int i);

};     


//计算两个时间差的函数,返回时间差以秒为单位。
int gettime(int day1, int hour1, int minute1, int second1, int day2, int hour2, int minute2, int second2);



/*函数功能1:区域范围查找。统计在某个特定的时间段，用户指定的矩形区域内的出租车的数目。这里的矩形区域可以通过给出矩形的左上角和右下角的经纬度坐标来确定。
传入参数:
	s为该区域内符合要求的所有点信息,先按照出租车序号名排序,再按照出租车轨迹点的时间排序
	pos数组为传入的矩形坐标,pos[0]对应左上角经度信息,pos[1]对应右下角经度信息,
	                      pos[2]对应左上角纬度信息,pos[3]对应右下角纬度信息
	day,hour,minute对应了查找的起始时间
	time_spac对应了查找的时间范围,注意time_span在未指定的情况下默认为10,单位为分钟,也就是分析特定时间段时一般假定这一段时间长度为10分钟
	output表示是否要函数输出相关信息,一般为true,表示要输出,如果是在其他功能内部调用就显式指定为false即可
*/
int function_findtaxinum_somewhere(set<Point, CMP>& s, double* pos, int day, int hour, int minute, int time_span = 10, bool output = true);


/*函数功能2: 区域车流密度分析。给定距离参数r,将整个地图划分成网格，每个格子的大小是r*r。统计分析在不同的时间段，经过所有网格区域内的车流密度的变化。
传入参数:
	r为0.001经纬度基本单位对应的次数值,如r=2则每个区域大小为0.002*0.002经纬度的区域	
	day,hour,minute对应了查找的起始时间
	time_spac对应了查找的时间范围,注意time_span在未指定的情况下默认为60,单位为分钟,也就是分析特定时间段时一般假定这一段时间长度为60分钟
	output表示是否要函数输出相关信息,一般为true,表示要输出,如果是在其他功能内部调用就显式指定为false即可
*/
void function_regional_density_analysis(int r, int day, int hour, int minute, int time_span = 60, bool output = true);


/*函数功能3:区域关联分析1。用户指定两个矩形区域，统计在不同的时间段，往来这两个区域之间的车流量的变化。
传入参数:
	pos1数组为传入的第一个矩形坐标,pos1[0]对应左上角经度信息,pos1[1]对应右下角经度信息,
							     pos1[2]对应左上角纬度信息,pos1[3]对应右下角纬度信息
	pos2数组为传入的第一个矩形坐标,pos2[0]对应左上角经度信息,pos2[1]对应右下角经度信息,
								 pos2[2]对应左上角纬度信息,pos2[3]对应右下角纬度信息
	day,hour,minute对应了查找的起始时间
	time_spac对应了查找的时间范围,注意time_span在未指定的情况下默认为60,单位为分钟,也就是分析特定时间段时一般假定这一段时间长度为60分钟
	output表示是否要函数输出相关信息,一般为true,表示要输出,如果是在其他功能内部调用就显式指定为false即可
返回两个矩形区域相交时的出租车序号名集合,用于函数功能6的分析
*/
set<Point, CMP> function_region_association_analysis_one(double* pos1, double* pos2, int day, int hour, int minute, int time_span = 60, bool output = true);


/*函数功能4:区域关联分析2。用户指定一个矩形区域，统计往来该矩形区域和其他区域的车流量随时间的变化。
传入参数:
	pos数组为传入的矩形坐标,pos[0]对应左上角经度信息,pos[1]对应右下角经度信息,
	                      pos[2]对应左上角纬度信息,pos[3]对应右下角纬度信息
	day,hour,minute对应了查找的起始时间
	time_spac对应了查找的时间范围,注意time_span在未指定的情况下默认为60,单位为分钟,也就是分析特定时间段时一般假定这一段时间长度为60分钟
	output表示是否要函数输出相关信息,一般为true,表示要输出,如果是在其他功能内部调用就显式指定为false即可
*/
void function_region_association_analysis_two(double* pos, int day, int hour, int minute, int time_span = 60, bool output = true);


/*
函数功能5:频繁路径分析1。一条路径的频繁度可以定义为这条路径上通行的汽车的总数。根据用户给定的参数k，距离参数x，统计在整个城市中，长度超过x的最为频繁的前k个路径。\
传入参数x表示长度,k表示数量,output表示是否要函数输出相关信息
返回结点经纬度信息数组,每个容器的元素为一个容器,存储四个元素,分别对应了结点1的经度、纬度,结点2的经度和纬度
*/
vector<vector<string>> function_path_frequent_analysis1(double x, int k, bool output = true);

/*
函数功能6:频繁路径分析2。给定两个矩形区域A和B，分析从A到B的最为频繁的前k个通行路径。
传入参数pos1表示矩形1,pos2表示矩形2,k表示数量,output表示是否要函数输出相关信息
返回结点经纬度信息数组,每个容器的元素为一个容器,存储四个元素,分别对应了结点1的经度、纬度,结点2的经度和纬度
*/
vector<vector<double>> function_path_frequent_analysis2(double* pos1, double* pos2, int k, bool output = true);

//此为只分析相邻点的路径情况
vector<vector<string>> function_path_frequent_analysis2_neighbor(double* pos1, double* pos2, int k, bool output = true);


/*函数功能7:通信时间分析。给定两个矩形区域A和B，分析在不同的时间段，出租车从A到B的通行时间最短的路径，以及相应的通行时间。
	传入参数:
	pos1表示矩形1, pos2表示矩形2
	pos1数组为传入的矩形坐标,pos1[0]对应左上角经度信息,pos1[1]对应右下角经度信息,
						  pos1[2]对应左上角纬度信息,pos1[3]对应右下角纬度信息
	pos2数组为传入的矩形坐标,pos2[0]对应左上角经度信息,pos2[1]对应右下角经度信息,
						  pos2[2]对应左上角纬度信息,pos2[3]对应右下角纬度信息
	day,hour,minute对应了查找的起始时间
	time_spac对应了查找的时间范围,
	output表示是否要函数输出相关信息,注意time_span默认分析一天的时间,除非手动指定
	返回一系列的点,从最短时间的开始点出发到结束点之间的全部点表示一条明确的路径
*/
vector<Point> function_pathuse_shortesttime(double* pos1, double* pos2, int day, int hour, int minute, int time_span = 60 * 24, bool output = true);
#endif