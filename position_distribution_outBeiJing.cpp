#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include "taxi_header_use.h"

using namespace std;

//���캯��,��ʼΪ��
position_distribution_outBeiJing::position_distribution_outBeiJing() :posmap({}) {}

//��������
position_distribution_outBeiJing::~position_distribution_outBeiJing() {
	for (auto i : posmap)
	{
		i.second.clear();
		i.second.shrink_to_fit();
	}
	unordered_map<int, vector<Point>> uu;
	posmap.swap(uu);
}

//position_distribution_outBeiJing��ĳ�ʼ��,ÿ������һ�����⳵,����ÿռ䷶Χ����Ч��Ϣ�;�����Ӧ����Ϣ����
void position_distribution_outBeiJing::position_distribution_outBeiJing_update(taxi& vt)
{
	int name = vt.getname();
	if (name != 0) {
		for (int i = 0; i < vt.getsize(); i++) {
			Point p = vt.getPoint(i);
			double d11 = p.getLongitude();
			double d22 = p.getLatitude();

			//���ڱ�����Χ�ھͽ��б���
			if (d11 < 115.40 or d11 >= 117.60 or d22 < 39.40 or d22>41.10)
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
			else continue;
		}
	}
	return;
}

//�ռ�ֲ�����ļ���ȡ��ʼ������
void position_distribution_outBeiJing::position_distribution_outBeiJing_init(vector<vector<string>>& vs)
{
	int name =0;
	for (auto i : vs)
	{

		//�����ĳ����������Ϣһ����Ϣ
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
void position_distribution_outBeiJing::print_position_distribution_outBeiJing()
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
	
unordered_map<int, vector<Point>> position_distribution_outBeiJing::getallpoint() 
{
	return posmap;
}

//���ضԳ��⳵������Ĳ�ѯ��Ϣ,��ѯ���˾ͽ��ó��ÿռ䷶Χ��������Ϣ����,���򷵻������Ϊ0����Ч��Ϣ��
pair<int, vector<Point>>& position_distribution_outBeiJing::get_onetime_onetaxi(int n)
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