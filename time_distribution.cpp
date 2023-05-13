#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include "taxi_header_use.h"
using namespace std;

//���캯��,��ʼΪ��
time_distribution::time_distribution() :timemap({}) {}

//��������,��Ҫ�����ڲ���vector��������Ϊ0,�Դ��ڳ��ڴ�ռ�,Ҳ��Ҫ��unordered_map���кͿ�map�Ľ���,�Դ��ڳ��ڴ�ռ�,���������������Ϣͳ�����׳��ֲ�������
time_distribution::~time_distribution()
{
	for (auto i : timemap)
	{
		//���Ԫ��
		i.second.clear();
		//������ӦԪ������
		i.second.shrink_to_fit();
	}
	unordered_map<int, vector<Point>> uu;
	timemap.swap(uu);
}

//time_distribution��ĳ�ʼ��,�Է���Ϊ��λ,ÿ������һ�����⳵,�����ʱ������Ч��Ϣ�;�����Ӧ����Ϣ����
//i1,i2,i3��Ӧ����Ҫ���µ�ʱ�����ʱ����Ϣ
void time_distribution::time_distribution_update(taxi& vt, int i1, int i2, int i3)
{
	int name = vt.getname();
	if (name != 0) {
		for (int i = 0; i < vt.getsize(); i++) {
			Point p = vt.getPoint(i);
			int i11 = p.getday();
			int i22 = p.gethour();
			int i33 = p.getminute();

			//�켣��ʱ�����Ҫ��ͱ���
			if (i11 == i1 and i22 == i2 and i33 == i3)
			{
				//�Ȳ�ѯ�ó��⳵�Ƿ񱣴��ĳ����,û���򴴽��µļ�ֵ�Բ��뱣��
				auto temp = timemap.find(name);
				if (temp == timemap.end())
				{
					vector<Point> vtemp;
					vtemp.push_back(p);
					timemap.insert({ name , vtemp });
				}
				//�б������ֵ�������������Ϣ����
				else
				{
					temp->second.push_back(p);
				}
			}

			//�켣��ʱ����ϳ���Ҫ��ͽ���(���⳵�ڵ���Ϣ��ʱ��������)
			else if (i11 > i1 or (i11 == i1 and i22 > i2) or
				(i11 == i1 and i22 == i2 and i33 > i3))
			{
				break;
			}

			//�켣��ʱ�䲻��Ҫ��ͼ���
			else continue;

		}
	}
	return;
}

//�����ʱ����ڵ�ȫ����Ϣ
void time_distribution::print_time_distribution()
{
	for (auto i : timemap)
	{
		cout << i.first << ":" << endl;
		for (auto j = 0; j < i.second.size(); j++) {
			cout << i.second[j].getLongitude() << "," << i.second[j].getLatitude() << "," << i.second[j].gettimes() << endl;
		}
	}
	return;
}

//���ضԳ��⳵������Ĳ�ѯ��Ϣ,��ѯ���˾ͽ��ó���ʱ��������Ϣ����,���򷵻������Ϊ0����Ч��Ϣ��
pair<int, vector<Point>>& time_distribution::get_onetime_onetaxi(int n)
{
	if (timemap.find(n) != timemap.end())
	{
		pair<int, vector<Point>> temp = *timemap.find(n);
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

//����ȫ����ʱ�εĳ��⳵�켣����Ϣ
unordered_map<int, vector<Point>> time_distribution::getallpoint()
{
	return timemap;
}

//��ʱ��Ϊ��������,��ѯ��ĳ����Χ�ڵĳ��⳵�켣����Ϣ,countnum��taxi_haveuse���ڱ�����ֵ�����������Щ����
set<Point, CMP> time_distribution::counttaxi(int day, int hour, int minute, double* p, int& countnum, bool* taxi_haveuse)
{
	//�������ؽ������
	set<Point, CMP> taxinum;
	
	//������ѯ��Ϣ
	for (auto i : timemap)
	{
		for (auto j = 0; j < i.second.size(); j++) {

			//һ���е��ڲ�ѯ��Χ��,���뵽��Ӧ�����
			if (i.second[j].getLongitude() >= p[0] and i.second[j].getLongitude() <= p[1]
				and i.second[j].getLatitude() >= p[2] and i.second[j].getLatitude() <= p[3])
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