#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include "taxi_header_use.h"

using namespace std;

//���캯��,��ʼΪ��,��γ��Ϊ0
position_distribution_inBeiJing::position_distribution_inBeiJing() :posmap({}), init_Longitude(0), init_Latitude(0) {}

//��������,��Ҫ�����ڲ���vector��������Ϊ0,�Դ��ڳ��ڴ�ռ�,Ҳ��Ҫ��unordered_map���кͿ�map�Ľ���,�Դ��ڳ��ڴ�ռ�,���������������Ϣͳ�����׳��ֲ�������
position_distribution_inBeiJing::~position_distribution_inBeiJing() {
	for (auto i : posmap)
	{
		//���Ԫ��
		i.second.clear();
		//������ӦԪ������
		i.second.shrink_to_fit();
	}
	unordered_map<int, vector<Point>> uu;
	posmap.swap(uu);
}

//position_distribution_inBeiJing��ĳ�ʼ��,��0.01*0.01��γ�ȵı仯Ϊ��λ,ÿ������һ�����⳵,����ÿռ䷶Χ����Ч��Ϣ�;�����Ӧ����Ϣ����
//d1,d2��Ӧ����Ҫ���µĿռ�ľ�γ����Ϣ
void position_distribution_inBeiJing::position_distribution_inBeiJing_update(taxi& vt, double d1, double d2)
{
	init_Longitude = d1;
	init_Latitude = d2;
	int name = vt.getname();
	if (name != 0) {
		for (int i = 0; i < vt.getsize(); i++) {
			Point p = vt.getPoint(i);
			double d11 = p.getLongitude();
			double d22 = p.getLatitude();
			//�켣��ռ����Ҫ��ͱ���
			if (d11 < d1 + 0.01 and d22 < d2 + 0.01 and d11 >= d1 and d22 >= d2)
			{
				//�Ȳ�ѯ�ó��⳵�Ƿ񱣴��ĳ����,û���򴴽��µļ�ֵ�Բ��뱣��
				auto temp = posmap.find(name);
				if (temp == posmap.end())
				{
					vector<Point> vtemp;
					vtemp.push_back(p);
					posmap.insert({ name , vtemp });
				}
				//�б������ֵ�������������Ϣ����
				else
				{
					temp->second.push_back(p);
				}
			}
			//�켣��ռ䲻����Ҫ��ͼ���(��Ϊ���⳵�켣������Ҫ�ٶ�Ϊ��ȫ�����)
			else continue;
		}
	}
	return;
}
//position_distribution_inBeiJing��ĳ�ʼ��,��0.01*0.01��γ�ȵı仯Ϊ��λ,ÿ������һ������Ϣ���б���
//d1,d2��Ӧ����Ҫ���µĿռ�ľ�γ����Ϣ
void position_distribution_inBeiJing::position_distribution_inBeiJing_update2(Point& t, double d1, double d2)
{
	init_Longitude = d1;
	init_Latitude = d2;
	auto i = posmap.find(t.gettaxiname());
	if (i == posmap.end())
	{
		vector<Point>temp;
		temp.push_back(t);
		posmap.insert({ t.gettaxiname(),temp });

	}
	else 
	{
		i->second.push_back(t);
	}
	return;
}

//�ռ�ֲ�����ļ���ȡ��ʼ������
void position_distribution_inBeiJing::position_distribution_inBeiJing_init(vector<vector<string>>& vs)
{
	//-1��0��Ӧ���Ǳ���Ƿ��ǵ�һ������Ϣ
	int name = -1;
	for (auto i : vs)
	{
		//�ǵ�һ��
		if(name==-1)
		{
			//�洢�Ķ�Ӧ��γ�Ƚ��г�ʼ��
			init_Longitude = stod(i[0]);
			init_Latitude = stod(i[1]);
			name = 0;
			continue;
		}

		//���ǵ�һ���������ĳ����������Ϣһ����Ϣ
		if (i.size() == 2)
		{
			//�����Ӧ������Ϳ�����
			name = stoi(i[0]);
			vector<Point> temp;
			posmap.insert({ name, temp });
		}

		//��������е�ĳ����
		else
		{
			Point temp;

			//��ĳ�ʼ��
			temp.Point_init2(name, stoi(i[2]), stoi(i[3]), stoi(i[4]), stoi(i[5]), stod(i[0]), stod(i[1]));

			//����복�Ķ�Ӧ������
			auto itemp = posmap.find(name);


			itemp->second.push_back(temp);
		}
	}
	return;
}

//����ÿռ䷶Χ�ڵ�ȫ����Ϣ
void position_distribution_inBeiJing::print_position_distribution_inBeiJing()
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

//��ö�Ӧ����
double position_distribution_inBeiJing::getinit_Longitude() { return init_Longitude; }

//��ö�Ӧγ��
double position_distribution_inBeiJing::getinit_Latitude() { return init_Latitude; }

//���ظÿռ��ڵ�ȫ�����⳵��ȫ����Ϣ
unordered_map<int, vector<Point>> position_distribution_inBeiJing::getallpoint() 
{ return posmap; }

//���ضԳ��⳵������Ĳ�ѯ��Ϣ,��ѯ���˾ͽ��ó��ÿռ䷶Χ��������Ϣ����,���򷵻������Ϊ0����Ч��Ϣ��
pair<int, vector<Point>>& position_distribution_inBeiJing::get_onetime_onetaxi(int n)
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

//�Կռ�Ϊ��������,��ѯ��ĳ����Χ��,ĳ��ʱ���ڵĳ��⳵�켣����Ϣ,countnum��taxi_haveuse���ڱ�����ֵ�����������Щ����
set<Point, CMP> position_distribution_inBeiJing::counttaxi(int day, int hour, int minute, double* p, int& countnum, int time_span, bool* taxi_haveuse)
{
	//�������ؽ������
	set<Point, CMP> taxinum;

	//������ѯ��Ϣ
	for (auto i : posmap)
	{
		for (auto j = 0; j < i.second.size(); j++) {

			//��Ҫ�����ѯʱ���������ʱ����жϼ��㷶Χ
			int temptime = gettime(day, hour, minute, 0, i.second[j].getday(), i.second[j].gethour(), i.second[j].getminute(), i.second[j].getsecond());
			
			//һ���е��ڲ�ѯ��Χ������ʱ����,���뵽��Ӧ�����
			//temptime <= time_span*60 �� temptime >= 0�ǳ�Ҫ����,ֻҪ����һ���ڸ�ʱ�����
			if (temptime <= time_span * 60 and temptime >= 0 and i.second[j].getLongitude() >= p[0] and i.second[j].getLongitude() <= p[1] and i.second[j].getLatitude() >= p[2] and i.second[j].getLatitude() <= p[3])
			{
				taxinum.insert(i.second[j]);
				
				//���¶�Ӧ�ĳ��⳵������Ϣ���������Ϣ,����������,δ���־�++count����Ǵ����������λ��Ϊtrue
				if (!taxi_haveuse[i.second[j].gettaxiname()])
				{
					countnum++;
					taxi_haveuse[i.second[j].gettaxiname()] = true;
				}
			}
		}
	}

	//���ؽ��
	return taxinum;
}