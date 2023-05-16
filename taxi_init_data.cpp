#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/string.hpp> 
#include "taxi_init.h"
#include "taxi_header_use.h"
using namespace std;

//Split函数用于将初始数据集中的出租车每行的点信息拆解成有效信息
vector<string> Split(const string& str, char split) {
	
	//建立istringstream对象,用于读取str字符串内容 
	istringstream ss(str);

	//暂存的item字符串对象
	string item;
	
	//拆解后点信息组成的数据的存储容器
	vector<std::string> elems;
	
	//循环读取ss对象内的串流内容,每次读取的停止符以split为准（本数据集中为逗号）
	while (getline(ss, item, split)) {
		if (!item.empty()) {
			elems.push_back(item);
		}
	}

	//返回蕴含点的信息的对应容器
	return elems;
}








//出租车类序列化实现:
//每次序列化1000辆出租车(否则太久了),然后将序列化的结果保存在磁盘对应路径上
const int taxiinit_num = 1000;     //每次序列化出租车的数量
//传入序列化出租车的起始位置。
void boost_archive_taxi(const int taxiinit_pos) {
	
	//出租车序列化总结果写入的磁盘目录和文件名,例如此处可以是taxitotal0-10的一个文件名;
	ofstream ofs("E:\\taxitotal\\taxitotal" + to_string(taxiinit_pos / taxiinit_num));

	//文本的输出归档类，使用一个ofstream来构造
	boost::archive::text_oarchive oa(ofs);
	
	//新建taxi指针指向需要创建的taxi数组大小
	taxi* vtaxi = new taxi[taxiinit_num];

	//数据集文件目录
	string k = "F:\\vs 2019gongzuoqu\\bighomework\\bighomework\\taxi_log_2008_by_id\\";
	ifstream fs;

	//新建二维容器将打开数据集文件的文件处理结果放入其中
	vector<vector<string>> vs;

	//循环处理对应的出租车信息
	for (int i = taxiinit_pos; i < taxiinit_num + taxiinit_pos; i++) {

		//打开数据集的某个出租车文件
		fs.open(k + to_string(i + 1) + ".txt", ios::in);

		//如果能够打开
		if (fs.is_open())
		{
			int j = 0;
			string temp;

			//按行读取信息, 每行信息经过函数处理完毕后作为一个元素放入容器中
			while (getline(fs, temp))
			{
				vs.push_back(Split(temp, ','));
				j++;
			}

			//如果有信息直接将taxi对象根据容器的内容进行初始化,没有信息不进行任何操作(此时taxi内信息即为默认构造函数的对应信息)
			if (j)
			{
				vtaxi[i - taxiinit_pos].taxi_init(vs);
			}

		}

		//打不开输出对应序号名的错误信息
		else cout << i + 1 << "is error!" << endl;
		
		//关闭文件清空容器信息,输出成功信息,准备进行下一次的处理,
		fs.close();
		vs.clear();
		vector<vector<string>>().swap(vs);
		cout << i + 1 << "is ok" << endl;

	}

	//逐个将所有处理过的出租车信息进行序列化,总文件中保存了这一部分的全部出租车信息
	for (int i = 0; i < taxiinit_num; i++) {
		oa << vtaxi[i];
	}

	//关闭写入文件
	ofs.close();

	//循环打开多个文件进行写入,每个文件存储一个出租车信息,文件名称根据出租车序列名决定
	for (int i = taxiinit_pos; i < taxiinit_num + taxiinit_pos; i++)
	{
		ofs.open("E:\\taxitotal\\taxi" + to_string(i + 1));
		boost::archive::text_oarchive oa2(ofs);
		oa2 << vtaxi[i - taxiinit_pos];
		ofs.close();
	}

	//写入完毕,删除对应暂存的出租车内容
	delete[]vtaxi;

	return;
}

//文件处理版本
void boost_archive_taxi2(const int taxiinit_pos)
{
	//出租车文件总结果写入的磁盘目录和文件名,例如此处可以是taxitotal0-10的一个文件名;
	ofstream ofs("D:\\taxitotal\\taxitotal" + to_string(taxiinit_pos / taxiinit_num)+".txt");

	//新建taxi指针指向需要创建的taxi数组大小
	taxi* vtaxi = new taxi[taxiinit_num];

	//数据集文件目录
	string k = "F:\\vs 2019gongzuoqu\\bighomework\\bighomework\\taxi_log_2008_by_id\\";
	ifstream fs;

	//新建二维容器将打开数据集文件的文件处理结果放入其中
	vector<vector<string>> vs;

	//循环处理对应的出租车信息
	for (int i = taxiinit_pos; i < taxiinit_num + taxiinit_pos; i++) {

		//打开数据集的某个出租车文件
		fs.open(k + to_string(i + 1) + ".txt", ios::in);

		//如果能够打开
		if (fs.is_open())
		{
			int j = 0;
			string temp;

			//按行读取信息, 每行信息经过函数处理完毕后作为一个元素放入容器中
			while (getline(fs, temp))
			{
				vs.push_back(Split(temp, ','));
				j++;
			}

			//如果有信息直接将taxi对象根据容器的内容进行初始化,没有信息不进行任何操作(此时taxi内信息即为默认构造函数的对应信息)
			if (j)
			{
				vtaxi[i - taxiinit_pos].taxi_init(vs);
			}

		}

		//打不开输出对应序号名的错误信息
		else cout << i + 1 << "is error!" << endl;

		//关闭文件清空容器信息,输出成功信息,准备进行下一次的处理,
		fs.close();
		vs.clear();
		vector<vector<string>>().swap(vs);
		cout << i + 1 << "is ok" << endl;

	}

	//逐个将所有处理过的出租车信息进行写入,总文件中保存了这一部分的全部出租车信息
	for (int i = 0; i < taxiinit_num; i++) {
		ofs << vtaxi[i].getname() << "," << vtaxi[i].getsize() << "\n";
		for (int j = 0; j < vtaxi[i].getsize(); j++)
		{
			Point temp = vtaxi[i].getPoint(j);
			
			ofs <<setiosflags(ios::fixed) << setprecision(5) << temp.getLongitude() << "," << temp.getLatitude() << "," << temp.getday() << "," << temp.gethour() << "," << temp.getminute() << "," << temp.getsecond() << "\n";
		}
	}

	//关闭写入文件
	ofs.close();

	//循环打开多个文件进行写入,每个文件存储一个出租车信息,文件名称根据出租车序列名决定
	for (int i = taxiinit_pos; i < taxiinit_num + taxiinit_pos; i++)
	{
		ofs.open("D:\\taxitotal\\taxi" + to_string(i + 1) + ".txt");
		ofs << vtaxi[i- taxiinit_pos].getname() << "," << vtaxi[i- taxiinit_pos].getsize() << "\n";
		for (int j = 0; j < vtaxi[i- taxiinit_pos].getsize(); j++)
		{
			Point temp = vtaxi[i- taxiinit_pos].getPoint(j);
			ofs << setiosflags(ios::fixed) << setprecision(5) << temp.getLongitude() << "," << temp.getLatitude() << "," << temp.getday() << "," << temp.gethour() << "," << temp.getminute() << "," << temp.getsecond() << "\n";
		}
		ofs.close();
	}

	//写入完毕,删除对应暂存的出租车内容
	delete[]vtaxi;
	return;
}



//出租车类反序列化实现:
//根据传入的出租车序号名进行相应的调用
taxi boost_iarchive_taxi(int i) {

	//建立临时对象
	taxi newvtaxi;

	//打开需要的出租车对象的文件
	ifstream ifs;
	ifs.open("E:\\taxitotal\\taxi" + to_string(i), std::ios::binary);
	
	//文本的输入归档类，使用一个ifstream来构造
	boost::archive::text_iarchive ia(ifs);

	//利用反序列化对taxi对象初始化
	ia >> newvtaxi;
	
	//输出完成信息并关闭文件
	cout << i << "finish!" << endl;
	ifs.close();
	return newvtaxi;
}

//文件处理版本
taxi boost_iarchive_taxi2(int i) {

	//建立临时对象
	taxi newvtaxi;

	//打开需要的出租车对象的文件
	ifstream ifs;
	ifs.open("D:\\taxitotal\\taxi" + to_string(i)+".txt");

	//将文件内已经分割的信息合并到一个string容器中
	vector<vector<string>> vs;
	string stemp;
	while (getline(ifs, stemp, '\n')) {
	
		vs.push_back(Split(stemp, ','));
	}

	//对象根据容器信息进行初始化
	newvtaxi.taxi_init2(vs);

	//输出完成信息并关闭文件
	cout << i << "finish!" << endl;

	ifs.close();
	return newvtaxi;
}


//taxi反序列化信息输入到文件中进行正常表示,方便其他操作
void taxifile(taxi& t) 
{
	//打开写入文件
	ofstream ofs;
	ofs.open("E:\\taxibase\\taxi" + to_string(t.getname())+".txt", ios::trunc);
	
	//遍历要写入的出租车的全部有效轨迹点,将其有效信息全部写入文件的每行中
	for (int i = 0; i < t.getsize(); i++) 
	{
		Point temp = t.getPoint(i);
		ofs << t.getname() << ",2008-02-0" << temp.getday() << " " << temp.gethour() << ":" << temp.getminute() << ":" << temp.getminute() << "," << setiosflags(ios::fixed) << setprecision(5) << temp.getLongitude() << "," << temp.getLatitude() << "\n";
	}
	return;
}






//出租车时间分布类序列化实现:
//根据已被清洗过数据的出租车信息进行调用
//用于对出租车轨迹点时间信息的分类和处理,以分钟为单位
//每次处理一天的数据(不然内存不够用),然后将序列化的结果保存在磁盘对应路径上
const int dayfor_time = 2;   //被处理的日期
void boost_archive_time() 
{
	//新建time_distribution指针指向需要创建的time_distribution数组大小
	//此处为24*60,对应一天的全部信息
	time_distribution***td = new time_distribution**[1];
	for(int i=0;i<1;i++)
	{
		td[i] = new time_distribution * [24];
		for(int j=0;j<24;j++)
		{
			td[i][j] = new time_distribution[60];
		}
	}

	//循环读取全部出租车信息进行序列化准备,然后将出租车对应的时间信息传入对应类进行信息更新
	for (int i = 0; i < taxisize; i++) {
		taxi temp;
		ifstream ifs("E:\\taxitotal\\taxi"+to_string(i+1));
		boost::archive::text_iarchive ia(ifs);
		ia >> temp;
		if (ifs.is_open())
		{
			for (int i = 0; i < 1; i++) 
			{
				for (int j = 0; j < 24; j++)
				{
					for (int k = 0; k < 60; k++) {
						td[i][j][k].time_distribution_update(temp, dayfor_time,j,k);
					}
				}
			}
		}
		//有错误则输出错误信息,否则输出正确信息
		else cout << i+1 << "is error!" << endl;
		ifs.close();
		cout << i + 1 << "is ok" << endl;
	}

	//循环打开多个文件进行写入,每个文件存储一个时间分布类信息,文件名称根据对应时间决定
	ofstream ofs;
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 24; j++)
		{
			for (int k = 0; k < 60; k++) {
				ofs.open("E:\\time_taxidata\\" + to_string(dayfor_time) + "号-" + to_string(j) + "时-" + to_string(k) + "分");
				boost::archive::text_oarchive oa(ofs);
				oa << td[i][j][k];
				ofs.close();
			}
		}
	}

	//写入完毕,删除对应暂存的时间分布类内容
	delete[]td;
	return;
}

//文件处理版本
void boost_archive_time2(int needday,int kkk) 
{
	//新建time_distribution指针指向需要创建的time_distribution数组大小
	//此处为kkk*24*60,对应kkk天的全部信息,开始日期为needday
	time_distribution*** td = new time_distribution * *[kkk];
	for (int i = 0; i < kkk; i++)
	{
		td[i] = new time_distribution * [24];
		for (int j = 0; j < 24; j++)
		{
			td[i][j] = new time_distribution[60];
		}
	}

	//循环读取全部出租车信息进行文件写入准备,然后将出租车对应的时间信息传入对应类进行信息更新
	for (int i = 0; i < taxisize; i++) {
		taxi temp;
		ifstream ifs("E:\\taxitotal\\taxi" + to_string(i + 1));
		boost::archive::text_iarchive ia(ifs);
		ia >> temp;
		if (ifs.is_open())
		{
			for (int i = 0; i < kkk; i++)
			{
				for (int j = 0; j < 24; j++)
				{
					for (int k = 0; k < 60; k++) {
						td[i][j][k].time_distribution_update(temp, needday+i, j, k);
					}
				}
			}
		}
		//有错误则输出错误信息,否则输出正确信息
		else cout << i + 1 << "is error!" << endl;
		ifs.close();
		cout << i + 1 << "is ok" << endl;
	}

	//循环打开多个文件进行写入,每个文件存储一个时间分布类信息,文件名称根据对应时间决定
	ofstream ofs;
	for (int i = 0; i < kkk; i++) {
		int flag;
		for (int j = 0; j < 24; j++)
		{
			for (int k = 0; k < 60; k++) {
				ofs.open("D:\\time_taxidata\\" + to_string(i+needday) + "号-" + to_string(j) + "时-" + to_string(k) + "分" + ".txt");
				auto temps=td[i][j][k].getallpoint();
				for (auto i : temps)
				{
					ofs << i.first << "," << i.second.size() << "\n";
					for(auto j:i.second)
					{
						ofs << setiosflags(ios::fixed) << setprecision(5) << j.getLongitude() << "," << j.getLatitude() << "," << j.getday() << "," << j.gethour() << "," << j.getminute() << "," << j.getsecond() << "\n";
					}
				}
				ofs.close();
			}
		}
	}

	//写入完毕,删除对应暂存的时间分布类内容
	delete[]td;
	return;
}



//时间分布类反序列化实现:
//根据传入的时间信息进行相应的调用
time_distribution boost_iarchive_time(int i, int j, int k) {
	
	//建立临时对象
	time_distribution newtd;

	//打开对应文件
	ifstream ifs;
	ifs.open("E:\\time_taxidata\\" + to_string(i) + "号-" + to_string(j) + "时-" + to_string(k) + "分", std::ios::binary);
	
	//文本的输入归档类，使用一个ifstream来构造
	boost::archive::text_iarchive ia(ifs);
	
	//利用反序列化对time_distribution对象初始化
	ia >> newtd;

	//输出完成信息并关闭文件
	cout << i << "号" << j << "时" << k << "分" << "finish" << endl;
	ifs.close();
	return newtd;
}

//文件处理版本
time_distribution boost_iarchive_time2(int i, int j, int k) {

	//建立临时对象
	time_distribution newtd;

	//打开对应文件
	ifstream ifs;
	ifs.open("D:\\time_taxidata\\" + to_string(i) + "号-" + to_string(j) + "时-" + to_string(k) + "分.txt", std::ios::binary);

	//将文件内已经分割的信息合并到一个string容器中
	vector<vector<string>> vs;
	string stemp;
	while (getline(ifs, stemp, '\n')) {

		vs.push_back(Split(stemp, ','));
	}

	//对象根据容器信息进行初始化
	newtd.time_distribution_init(vs);

	//输出完成信息并关闭文件
	cout << i << "号" << j << "时" << k << "分" << "finish" << endl;
	ifs.close();
	return newtd;
}









//出租车北京内空间分布类序列化实现:
//根据已被清洗过数据的出租车信息进行调用
//用于对出租车轨迹点北京内空间信息的分类和处理,以0.01经纬度变化为单位
//每次处理addLong*100*170个对象,包括addLong经度-1.7纬度变化范围
const double number = 2.0;    //变化次数,如第一次变化是116.40~116.40+addLong经度和39.40~41.10纬度
const double addLong = 0.2;   //变化的经度大小,此处对应每次序列化0.2经度
const double pos_init_Longitude = 115.40 + number * addLong; //实际序列化的起始经度
const double pos_init_Latitude = 39.40; //实际序列化的起始纬度
void boost_archive_position_in()
{

	//新建position_distribution_inBeiJing指针指向需要创建的position_distribution_inBeiJing数组大小
	//此处为20*170,对应一定空间范围的信息
	position_distribution_inBeiJing** pdi = new position_distribution_inBeiJing * [20];
	for (int i = 0; i < 20; i++)
	{
		pdi[i] = new position_distribution_inBeiJing[170];
	}

	//循环读取全部出租车信息进行序列化准备,然后将出租车对应的空间信息传入对应类进行信息更新
	for (int i = 0; i < taxisize; i++) {
		taxi temp;
		ifstream ifs("E:\\taxitotal\\taxi" + to_string(i + 1));
		boost::archive::text_iarchive ia(ifs);
		if (ifs.is_open())
		{
			ia >> temp;
			for (int i = 0; i < 20; i++)
			{
				for (int j = 0; j < 170; j++)
				{
					pdi[i][j].position_distribution_inBeiJing_update
					(temp, double(i) * 0.01 + pos_init_Longitude, double(j * 0.01) + pos_init_Latitude);
				}
			}
		}
		//有错误则输出错误信息,否则输出正确信息
		else cout << i + 1 << "is error!" << endl;
		ifs.close();
		cout << i + 1 << "is ok" << endl;
	}

	//循环打开多个文件进行写入,每个文件存储一个空间分布类信息,文件名称根据对应空间决定
	ofstream ofs;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 170; j++)
		{
			ofs.open("E:\\pos_taxidata\\inbeijing\\" + to_string(double(i) * 0.01 + pos_init_Longitude) + "经度," + to_string(double(j * 0.01) + pos_init_Latitude) + "纬度");
			boost::archive::text_oarchive oa(ofs);
			oa << pdi[i][j];
			ofs.close();
		}
	}
	//写入完毕,删除对应暂存的空间分布类内容
	delete[]pdi;
	return;
}

//文件版本
void boost_archive_position_in2() 
{
	//新建position_distribution_inBeiJing指针指向需要创建的position_distribution_inBeiJing数组大小
	//此处为addLong*100*170,对应一定空间范围的信息
	int addtemp = (int)(addLong * 100);
	position_distribution_inBeiJing** pdi = new position_distribution_inBeiJing * [addtemp];
	for (int i = 0; i < addtemp; i++)
	{
		pdi[i] = new position_distribution_inBeiJing[170];
	}

	//循环读取全部出租车信息进行文件写入准备,然后将出租车对应的空间信息传入对应类进行信息更新
	for (int i = 0; i < taxisize; i++) {
		taxi temp;
		ifstream ifs("E:\\taxitotal\\taxi" + to_string(i + 1));
		boost::archive::text_iarchive ia(ifs);
		if (ifs.is_open())
		{
			ia >> temp;
			for(int i=0;i<temp.getsize();i++)
			{
				Point t = temp.getPoint(i);
				double d1 = t.getLongitude();
				double d2 = t.getLatitude();
				if ( d1>= pos_init_Longitude and d1 < pos_init_Longitude + addLong and d2 >= 39.40 and d2 < 41.10) 
				{
					int index1 = int(floor((d1- pos_init_Longitude) * 100));
					int index2 = int(floor((d2 - pos_init_Latitude) * 100));
					pdi[index1][index2].position_distribution_inBeiJing_update2(t, floor(d1 * 100) / 100, floor(d2 * 100) / 100);
				}
			}
		}
		//有错误则输出错误信息,否则输出正确信息
		else cout << i + 1 << "is error!" << endl;
		ifs.close();
		cout << i + 1 << "is ok" << endl;
	}

	//循环打开多个文件进行写入,每个文件存储一个空间分布类信息,文件名称根据对应空间决定
	ofstream ofs;
	for (int i = 0; i < addtemp; i++) {
		for (int j = 0; j < 170; j++)
		{
			ofs.open("D:\\pos_taxidata\\inbeijing\\" + to_string(double(i) * 0.01 + pos_init_Longitude) + "经度," + to_string(double(j * 0.01) + pos_init_Latitude) + "纬度"+".txt");
			ofs << setiosflags(ios::fixed) << setprecision(5) << double(i) * 0.01 + pos_init_Longitude << "," << double(j * 0.01) + pos_init_Latitude << "\n";
			auto temps = pdi[i][j].getallpoint();
			for (auto i : temps)
			{
				ofs << i.first << "," << i.second.size() << "\n";
				for (auto j : i.second)
				{
					ofs << setiosflags(ios::fixed) << setprecision(5) << j.getLongitude() << "," << j.getLatitude() << "," << j.getday() << "," << j.gethour() << "," << j.getminute() << "," << j.getsecond() << "\n";
				}
			}
			ofs.close();
		}
	}
	//写入完毕,删除对应暂存的空间分布类内容
	delete[]pdi;
	return;
}



//出租车北京外空间分布类序列化实现:
//根据已被清洗过数据的出租车信息进行调用
//用于对出租车轨迹点北京外空间信息的处理
void boost_archive_position_out() 
{
	//新建position_distribution_outBeiJing对象
	position_distribution_outBeiJing p;

	//循环读取全部出租车信息进行序列化准备,然后将出租车对应的空间信息传入对应类进行信息更新
	for (int i = 0; i < taxisize; i++) {
		taxi temp;
		ifstream ifs("E:\\taxitotal\\taxi" + to_string(i + 1));
		boost::archive::text_iarchive ia(ifs);
		if (ifs.is_open())
		{
			ia >> temp;
			p.position_distribution_outBeiJing_update(temp);
		}
		//有错误则输出错误信息,否则输出正确信息
		else cout << i + 1 << "is error!" << endl;
		ifs.close();
		cout << i + 1 << "is ok" << endl;
	}

	//打开单个文件进行写入,其存储了北京外的全部点信息
	ofstream ofs;
	ofs.open("E:\\pos_taxidata\\outbeijing\\outbeijing");
	boost::archive::text_oarchive oa(ofs);
	oa << p;
	ofs.close();
}

void boost_archive_position_out2()
{
	//新建position_distribution_outBeiJing对象
	position_distribution_outBeiJing p;

	//循环读取全部出租车信息进行序列化准备,然后将出租车对应的空间信息传入对应类进行信息更新
	for (int i = 0; i < taxisize; i++) {
		taxi temp;
		ifstream ifs("E:\\taxitotal\\taxi" + to_string(i + 1));
		boost::archive::text_iarchive ia(ifs);
		if (ifs.is_open())
		{
			ia >> temp;
			p.position_distribution_outBeiJing_update(temp);
		}
		//有错误则输出错误信息,否则输出正确信息
		else cout << i + 1 << "is error!" << endl;
		ifs.close();
		cout << i + 1 << "is ok" << endl;
	}

	//打开单个文件进行写入,其存储了北京外的全部点信息
	ofstream ofs;
	ofs.open("D:\\pos_taxidata\\outbeijing\\outbeijing.txt");
	auto temp = p.getallpoint();
	for (auto i : temp)
	{
		ofs << i.first << "," << i.second.size() << "\n";
		for (auto j : i.second)
		{
			ofs << setiosflags(ios::fixed) << setprecision(5) << j.getLongitude() << "," << j.getLatitude() << "," << j.getday() << "," << j.gethour() << "," << j.getminute() << "," << j.getsecond() << "\n";
		}
	}
	ofs.close();
}




//出租车北京内空间分布类反序列化实现:
//根据传入的空间信息进行相应的调用(传入的空间信息应为已经处理过的信息)
position_distribution_inBeiJing boost_iarchive_position_in(double i,double j)
{

	//建立临时对象
	position_distribution_inBeiJing newpdi;

	//打开对应文件
	ifstream ifs;
	ifs.open("E:\\pos_taxidata\\inbeijing\\" + to_string(i) + "经度," + to_string(j) + "纬度", std::ios::binary);

	//文本的输入归档类，使用一个ifstream来构造
	boost::archive::text_iarchive ia(ifs);

	//利用反序列化对position_distribution_inBeiJing对象初始化
	ia >> newpdi;

	//输出完成信息并关闭文件
	cout << i << "经度" << "," << j << "纬度" << "finish" << endl;
	ifs.close();
	return newpdi;
}

//文件版本
position_distribution_inBeiJing boost_iarchive_position_in2(double i, double j) 
{

	//建立临时对象
	position_distribution_inBeiJing newpdi;

	//打开对应文件
	ifstream ifs;
	ifs.open("D:\\pos_taxidata\\inbeijing\\" + to_string(i) + "经度," + to_string(j) + "纬度.txt", std::ios::binary);

	//将文件内已经分割的信息合并到一个string容器中
	vector<vector<string>> vs;
	string stemp;
	while (getline(ifs, stemp, '\n')) {

		vs.push_back(Split(stemp, ','));
	}

	//对象根据容器信息进行初始化
	newpdi.position_distribution_inBeiJing_init(vs);

	//输出完成信息并关闭文件
	cout << i << "经度" << "," << j << "纬度" << "finish" << endl;
	ifs.close();
	return newpdi;
}



//出租车北京内空间分布类反序列化实现:
//对象唯一直接反序列化即可
position_distribution_outBeiJing boost_iarchive_position_out() 
{

	//建立临时对象
	position_distribution_outBeiJing newp;
	
	//打开对应文件
	ifstream ifs;
	ifs.open("E:\\pos_taxidata\\outbeijing\\outbeijing",ios::binary);

	//文本的输入归档类，使用一个ifstream来构造
	boost::archive::text_iarchive ia(ifs);

	//利用反序列化对position_distribution_inBeiJing对象初始化
	ia >> newp;

	//输出完成信息并关闭文件
	cout << "outbeijing finish" << endl;
	ifs.close();
	return newp;
}

//文件版本
position_distribution_outBeiJing boost_iarchive_position_out2() 
{
	//建立临时对象
	position_distribution_outBeiJing newp;

	//打开对应文件
	ifstream ifs;
	ifs.open("D:\\pos_taxidata\\outbeijing\\outbeijing.txt", ios::binary);

	//将文件内已经分割的信息合并到一个string容器中
	vector<vector<string>> vs;
	string stemp;
	while (getline(ifs, stemp, '\n')) {

		vs.push_back(Split(stemp, ','));
	}

	//对象根据容器信息进行初始化
	newp.position_distribution_outBeiJing_init(vs);

	//输出完成信息并关闭文件
	cout << "outbeijing finish" << endl;
	ifs.close();
	return newp;
}









//出租车路径结点+边+图类的序列化实现:
//首先通过其他函数(如更新函数等)实现整个图,再根据本图内容进行序列化,是图类的成员函数
//用于对出租车相邻轨迹点路径信息的处理,以不考虑长路径(没有划分依据的话在整个北京范围内搜索时空代价太大,而且没有统一的一个标准)
//将点、边和图对象一次性全部处理(内存刚好够用)
void pathgraph::pathserialization()
{

	//遍历全部结点,每个元素序列化到一个文件中
	for (auto i : pathNodemap)
	{
		ofstream ofs;
		ofs.open("D:\\pathdata\\pathNode\\Node" + i.first);
		boost::archive::text_oarchive oa(ofs);
		oa << *i.second;
		ofs.close();
	}

	//遍历全部边,每个元素序列化到一个文件中
	for (auto i : pathEdgemap)
	{
		ofstream ofs;
		ofs.open("D:\\pathdata\\pathEdge\\Edge" + i.first);
		boost::archive::text_oarchive oa(ofs);
		oa << *(i.second);
		ofs.close();
	}

	//将整个图对象序列化到一个文件中
	ofstream ofs;
	ofs.open("D:\\pathdata\\pathdatagraph");
	boost::archive::text_oarchive oa(ofs);
	oa << *this;
	ofs.close();
}

//文件版本
void pathgraph::pathserialization2()
{
	//遍历全部结点,每个元素序列化到一个文件中
	for (auto i : pathNodemap)
	{
		ofstream ofs;
		ofs.open("D:\\pathdata\\pathNode\\Node" + i.first +".txt");
		ofs << setiosflags(ios::fixed) << setprecision(5)<< i.second->getLongitudeNode() << "," << i.second->getLatitudeNode() << '\n';
		ofs.close();
	}

	//遍历全部边,每个元素序列化到一个文件中
	for (auto i : pathEdgemap)
	{
		ofstream ofs;
		ofs.open("D:\\pathdata\\pathEdge\\Edge" + i.first + ".txt");
		ofs << setiosflags(ios::fixed) << setprecision(5) 
			<< i.second->getpathNode1()->getLongitudeNode() << ","
			<< i.second->getpathNode1()->getLatitudeNode()<<","
			<< i.second->getpathNode2()->getLongitudeNode() << "," 
			<< i.second->getpathNode2()->getLatitudeNode() << "," 
			<< i.second->getpathcount() << "," 
			<< i.second->getpathlen() << "," 
			<< i.second->getpathshortesttime() <<","
			<< i.second->getshortesttime_taxistart().gettaxiname()<<","
			<< i.second->getshortesttime_taxistart().getLongitude() << "," 
			<< i.second->getshortesttime_taxistart().getLatitude() << "," 
			<< i.second->getshortesttime_taxistart().getday() << "," 
			<< i.second->getshortesttime_taxistart().gethour() << "," 
			<< i.second->getshortesttime_taxistart().getminute() << "," 
			<< i.second->getshortesttime_taxistart().getsecond()<<","
			<< i.second->getshortesttime_taxiend().gettaxiname() << ","
			<< i.second->getshortesttime_taxiend().getLongitude() << "," 
			<< i.second->getshortesttime_taxiend().getLatitude() << "," 
			<< i.second->getshortesttime_taxiend().getday() << "," 
			<< i.second->getshortesttime_taxiend().gethour() << "," 
			<< i.second->getshortesttime_taxiend().getminute() << "," 
			<< i.second->getshortesttime_taxiend().getsecond() << '\n';
		ofs.close();
	}

	//将整个图对象序列化到一个文件中
	ofstream ofs;
	ofs.open("D:\\pathdata\\pathdatagraph.txt");
	for (auto i : pathEdgemap) 
	{
		ofs << setiosflags(ios::fixed) << setprecision(5)
			<< i.second->getpathNode1()->getLongitudeNode() << ","
			<< i.second->getpathNode1()->getLatitudeNode() << ","
			<< i.second->getpathNode2()->getLongitudeNode() << ","
			<< i.second->getpathNode2()->getLatitudeNode() << ","
			<< i.second->getpathcount() << ","
			<< i.second->getpathlen() << ","
			<< i.second->getpathshortesttime() << ","
			<< i.second->getshortesttime_taxistart().gettaxiname() << ","
			<< i.second->getshortesttime_taxistart().getLongitude() << ","
			<< i.second->getshortesttime_taxistart().getLatitude() << ","
			<< i.second->getshortesttime_taxistart().getday() << ","
			<< i.second->getshortesttime_taxistart().gethour() << ","
			<< i.second->getshortesttime_taxistart().getminute() << ","
			<< i.second->getshortesttime_taxistart().getsecond() <<","
			<< i.second->getshortesttime_taxiend().gettaxiname() << ","
			<< i.second->getshortesttime_taxiend().getLongitude() << ","
			<< i.second->getshortesttime_taxiend().getLatitude() << ","
			<< i.second->getshortesttime_taxiend().getday() << ","
			<< i.second->getshortesttime_taxiend().gethour() << ","
			<< i.second->getshortesttime_taxiend().getminute() << ","
			<< i.second->getshortesttime_taxiend().getsecond() << ","
			<< i.first << '\n';
	}
	ofs << ".,." << '\n';
	for (auto i : pathNodemap)
	{
		ofs << i.first << "," << i.second->getLongitudeNode() << "," << i.second->getLatitudeNode() << '\n';
	}
	ofs.close();
}




//出租车路径结点+边+图类的反序列化实现:
//不单独序列化结点和边,而是整体进行图的序列化,按图的信息进行操作
//因为建立临时对象的代价太大,所以只能选择传入引用并修改该变量
void boost_pathserialization_iarchive(pathgraph &p)
{

	//打开对应文件
	ifstream ifs;
	ifs.open("E:\\pathdata\\pathdatagraph");

	//文本的输入归档类，使用一个ifstream来构造
	boost::archive::text_iarchive ia(ifs);
	
	//利用反序列化对pathgraph对象初始化
	ia >> p;

	//输出完成信息并关闭文件
	ifs.close();
	cout << "graph finish" << endl;
	return ;
}

void boost_pathserialization_iarchive2(pathgraph& p) 
{
	//打开对应文件
	ifstream ifs;
	ifs.open("D:\\pathdata\\pathdatagraph.txt");

	//将文件内已经分割的信息合并到一个string容器中
	string stemp;
	while (getline(ifs, stemp, '\n')) {
		
		vector<vector<string>> vs;

		vs.push_back(Split(stemp, ','));
		
		//对象根据容器信息进行初始化
		p.pathgraph_init(vs);
	}




	//输出完成信息并关闭文件
	ifs.close();
	cout << "graph finish" << endl;
	return;
}