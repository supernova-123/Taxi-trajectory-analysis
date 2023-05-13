#include <iostream>
#include <vector>
#include "taxi_header_use.h"
using namespace std;

//���캯��:
taxi::taxi() :name(0), size(0), pv(NULL) {}

//�������캯��,��ֵֹ����ʱ��������
taxi::taxi(const taxi& t) 
{
	name=t.name;
	size=t.size;
	pv = new Point[size];
	for(int i=0;i<size;i++)
	{
		pv[i] = t.pv[i];
	}
}

//��ֵ�������ֵֹ����ʱ��������
taxi taxi::operator=(const taxi& t)
{
	name = t.name;
	size = t.size;
	if (pv != NULL)
		delete[]pv;
	pv = new Point[size];
	for (int i = 0; i < size; i++)
	{
		pv[i] = t.pv[i];
	}
	return *this;
}

//taxi������,pvһ���ǿջ���ָ��ѵ�ָ��,���Է���delete
taxi::~taxi()
{
	if (pv != NULL)
	{
		delete[]pv;
		pv = NULL;
	}
}

//taxi��ʼ������,ֻ��ԭ���ݴ���ʱʹ��,�Ժ��÷����л�����˺���(����ٶ�̫����)
//�������Ϊ����õĳ��⳵��Ϣ��ά����,��Ϊ����,��Ϊ��Ϣ��(����,ʱ�����ʱ����,�ռ�ľ�γ��)
void taxi::taxi_init(vector<vector<string>>& vs)
{
	//�����л���������
	name = stoi(vs[0][0]);

	//���ȫ���������
	int tempsize = int(vs.size());
	
	//������̬�켣��ָ��,ָ��ȫ������Ϣ
	Point* temppv = new Point[vs.size()];
	size = 0;
	int j = 0;
	int i;
	
	//��ϴ����,�Ե���Ϣ���д���,����Ч��ȥ��
	
	for (i = 0; i < tempsize; i++)
	{
		//��������ľ���С��100����γ��С��20ֱ���϶�Ϊ��Ч��(�����ܿ�������ȥ��)
		if (stoi(vs[j][2]) < 100 or stoi(vs[j][3]) <= 20) {
			i--; tempsize--;
			j++; continue;
		}
		//������������ĵ��������ʼ������Ϣ
		(*(temppv + i)).Point_init(name, vs[j][1], vs[j][2], vs[j][3]);
		
		//i==0�ǵ�һ����,��Ϊ�ο���,
		if (i != 0) {

			//��õ�ǰ���ǰһ����ľ��Ȳ�:
			double d1 = (*(temppv + i)).getLongitude() - (*(temppv + i - 1)).getLongitude();

			//��õ�ǰ���ǰһ�����γ�Ȳ�:
			double d2 = (*(temppv + i)).getLatitude() - (*(temppv + i - 1)).getLatitude();
			
			//�������ľ�γ�Ⱦ��������ʵ�ʵľ����
			double distance = sqrt(d1 * d1 * 100000 * 100000 + d2 * d2 * 111320 * 111320);

			//������������ʱ��,����Ϊ��λ
			double time = gettime(
				(*(temppv + i - 1)).getday(), (*(temppv + i - 1)).gethour(),
				(*(temppv + i - 1)).getminute(), (*(temppv + i - 1)).getsecond(),
				(*(temppv + i)).getday(), (*(temppv + i)).gethour(),
				(*(temppv + i)).getminute(), (*(temppv + i)).getsecond()
			);

			//�������֮����ٶȳ�����67m/s,˵�����ǳ����ٶ���ô��,�������ݲ����Ĵ���,��Ϊ��Ч��
			if (distance / (time + 0.00001) > 67)
			{
				cout << "name:" << name << endl;
				cout << "��" << j + 1 << "�� is error" << endl;
				i--;
				tempsize--;
			}
		}
		j++;
	}

	//size������Ч������
	size = tempsize;
	
	//pv������Ч����Ϣ
	pv = new Point[size];
	for (i = 0; i < size; i++)
	{
		pv[i] = temppv[i];
	}

	//��ʡ�ڴ�ռ�
	delete[]temppv;
	return;
}

//����taxi�ĸ�����Ч��Ϣ
int taxi::getsize() { return size; }
int taxi::getname() { return name; }
double taxi::getLongitude2(int i) 
{
	//�����������Ҫ�����Ƿ�Խ��
	if (pv != NULL and i < size and i >= 0)
		return pv[i].getLongitude();
	else return 0;
}
double taxi::getLatitude2(int i) 
{
	if (pv != NULL and i < size and i >= 0)
		return pv[i].getLatitude();
	else return 0;
}
Point& taxi::getPoint(int pos)
{
	Point temp;
	if (pv != NULL and pos < size and pos >= 0)
		return pv[pos];
	else return temp;
}


//taxi��ȫ����Ϣ���
void taxi::printtaxi_track()
{
	cout << "name:" << name << endl;
	if (pv == NULL)
	{
		cout << "No data!" << endl;
		return;
	}
	for (int i = 0; i < size; i++)
	{
		cout << "Point:" << endl;
		cout << "time:" << pv[i].gettimes() << endl;
		cout << "Longitude:" << setprecision(8) << pv[i].getLongitude() << endl;
		cout << "Latitude:" << setprecision(8) << pv[i].getLatitude() << endl;
		//ע��㾭γ����Ϣ��ౣ����8λ��Ч����
	}
	return;
}
