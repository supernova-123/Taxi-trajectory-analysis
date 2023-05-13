#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include "taxi_header_use.h"

using namespace std;

//���캯��,�߼�ͬr��
position_distribution_outr::position_distribution_outr(int i) :vsize(i)
{
	information_regional_density.resize(vsize);
	count.resize(vsize);
	for (int i = 0; i < vsize; i++)
	{
		count[i] = 0;
	}
}

//��������,�߼�ͬr��
position_distribution_outr::~position_distribution_outr()
{
	count.clear();
	count.shrink_to_fit();
	information_regional_density.clear();
	information_regional_density.shrink_to_fit();
}

//��������ʱ�γ��⳵���ִ���֮��
int position_distribution_outr::getallvcount()
{
	int temp = 0;
	for (int i = 0; i < vsize; i++)
	{
		temp += count[i];
	}
	return temp;
}

//���ظ����ڹ��ж��ٸ�ʱ��
int position_distribution_outr::getvsize()
{
	return vsize;
}

//���ظ�ʱ����ĳ��ʱ�α�����ĳ��⳵����
int position_distribution_outr::getcountsize(int i) 
{
	//�޶���i��Χ
	if (i < vsize)
	{
		return count[i];
	}
	else return -1;
}

//���ظ�ʱ����ĳ��ʱ�α������ȫ�����⳵��Ϣ
set<Point, CMP> position_distribution_outr::getallPoint(int i)
{
	//�޶���i��Χ
	if (i < vsize)
		return information_regional_density[i];
	else return {};
}

//������Ӧʱ���ڲ������Ҫ��Ĺ켣��
void position_distribution_outr::insert(Point p, int i)
{
	//�Ƚ�����Ϣֱ�Ӳ����Ӧλ��
	information_regional_density[i].insert(p);

	//�жϴ˳��⳵�Ƿ���ֹ�,������ֹ���ִ���κβ���,���ֹ��򽫳�����Ӧλ����Ϊtrue,����+1
	if (!taxi_haveuse[p.gettaxiname()])
	{
		count[i]++;
		taxi_haveuse[p.gettaxiname()] = true;
	}
	return;
}