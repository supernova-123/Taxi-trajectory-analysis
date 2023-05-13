#include<iostream>
#include"taxi_header_use.h"
#include"taxi_init.h"
using namespace std;
int main()
{
	double pos1[4] = { 116.6133,116.7321,40.234,40.444 };
	double pos2[4] = { 116.4612,116.671,40.034,40.144 };
	//函数功能1测试
	//set<Point, CMP> s1;
	//set<Point, CMP> s2;
	//int counttaxi1 = function_findtaxinum_somewhere(s1, pos1, 4, 0, 0, 24 * 60);
	//int counttaxi2 = function_findtaxinum_somewhere(s2, pos2, 5, 0, 0, 24 * 60);
	//函数功能2测试
	//function_regional_density_analysis(5, 5, 12, 0, 6 * 60);
	//function_regional_density_analysis(20, 6, 12, 0, 24 * 60);
	//函数功能3测试
	//function_region_association_analysis_one(pos1, pos2, 5, 0, 0, 24 * 60);
	
	//函数功能4测试
	//function_region_association_analysis_two(pos1, 3, 12, 0, 2 * 60);

	//函数功能5测试
	//function_path_frequent_analysis1(2000, 10);
	
	//函数功能6测试
	//function_path_frequent_analysis2(pos1,pos2,10);
	//cout << "-------------------------------------" << endl;	
	
	//函数功能6相邻点版本测试
	//function_path_frequent_analysis2_neighbor(pos1, pos2, 10);
	
	//函数功能7测试
	//vector<Point> result =function_pathuse_shortesttime(pos1,pos2,2,0,0,1*24*60);
	//for (auto i : result) 
	//{
	//	i.printPoint();
	//}
	return 0;	
}