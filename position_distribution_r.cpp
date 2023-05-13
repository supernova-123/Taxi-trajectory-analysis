#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include "taxi_header_use.h"

using namespace std;
int position_distribution_r::r = 0;
int position_distribution_r::vsize = 0;
//���캯��,ȷ��������̬����r��vsize
position_distribution_r::position_distribution_r(int i, int j) 
{
	r = i, vsize = j;
}

//������������������ȫ������ڳ��ڴ����ں�����������
position_distribution_r::~position_distribution_r()
{
	taxijudge.clear();
	taxijudge.shrink_to_fit();
	information_regional_density.clear();
	information_regional_density.shrink_to_fit();
}

//��������ʱ�γ��⳵���ִ���֮��
int position_distribution_r::getallcount()
{
	int temp = 0;
	//taxijudge.size()��ʾ��Чʱ��
	for (int i = 0; i < taxijudge.size(); i++)
	{
		temp += taxijudge[i].size();
	}
	return temp;
}

//���ظ����ڹ��ж��ٸ�ʱ��
int position_distribution_r::getvsize()
{
	return vsize;
}

//���ظ�ʱ����ĳ��ʱ�θ�����ĳ��⳵���������,ʱ�δ�����iָ��
vector<int> position_distribution_r::gettaxijudge(int i)
{
	//�޶���i��Χ
	if (i < vsize and i < taxijudge.size())
	{
		return taxijudge[i];
	}
	else return {};
}

//���ظ�ʱ����ĳ��ʱ�θ�����ĳ��⳵����,ʱ�δ�����iָ��
int position_distribution_r::getcountsize(int i) 
{
	if (i < taxijudge.size() and i < vsize) {
		return taxijudge[i].size();
	}
	else return 0;
}


//���ظ�ʱ����ĳ��ʱ�θ������ȫ�����⳵��Ϣ,ʱ�δ�����iָ��
set<Point, CMP> position_distribution_r::getallPoint(int i)
{
	//�޶���i��Χ
	if (i < vsize and i< information_regional_density.size())
		return information_regional_density[i];
	else return {};
}

//������Ӧʱ���ڲ������Ҫ��Ĺ켣��,��Ӧʱ����iָ��
void position_distribution_r::insert(Point p, int i)
{
	//���ж�������С�Ƿ�����Ҫ��,������������(Ϊ��ʡ�ڴ�)
	if (information_regional_density.size() <= i)
	{
		information_regional_density.resize(i + 1, {});
		//������Ϣֱ�Ӳ����Ӧλ��
		information_regional_density[i].insert(p);
	}
	//����ֱ�Ӳ���
	else 
	{
		information_regional_density[i].insert(p);
	}

	//���ж�������С�Ƿ�����Ҫ��,������������(Ϊ��ʡ�ڴ�)
	if (taxijudge.size()<=i)
	{
		taxijudge.resize(i + 1, {});
		//�жϴ˳��⳵�Ƿ���ֹ�,������ֹ���ִ���κβ���,���ֹ��򽫳��������Ӧ������,����+1
		if (find(taxijudge[i].begin(), taxijudge[i].end(), p.gettaxiname()) == taxijudge[i].end())
		{
			taxijudge[i].push_back(p.gettaxiname());
		}
	}
	//����������һ���ж�
	else 
	{
		if (find(taxijudge[i].begin(), taxijudge[i].end(), p.gettaxiname()) == taxijudge[i].end())
		{
			taxijudge[i].push_back(p.gettaxiname());
		}
	}
	return;
}