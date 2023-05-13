#include <iostream>
#include <set>
#include <unordered_map>
#include <cmath>
#include "taxi_header_use.h"
#include "taxi_init.h"
using namespace std;


//��������1ʵ��
//set<Point, CMP>& sΪ����,��Ϊ�˽������ڵĵ���Ϣ���浽����,�Դ��������������������Ҫ������������ʱ�ܹ���������Ϣ
//���ؾ��������ڵĳ��⳵����Ŀ
int function_findtaxinum_somewhere(set<Point, CMP>& s, double* pos, int day, int hour, int minute, int time_span , bool output)
{

	//��ʼ���þ���������⳵����
	int counttaxi = 0;

	//��ʼ���жϸ�������⳵������Ƿ��������
	bool taxiflag[10375] = { 0 };

	//�жϴ�����ε��������Ƿ���ڱ�������
	if (pos[0] >= inBeijing_Longitude_min and pos[1] <= inBeijing_Longitude_max
		and pos[2] >= inBeijing_Latitude_min and pos[3] <= inBeijing_Latitude_max)
	{
		//�����,���ݿռ�ֲ��������Ϣ��ʼ��

		//��ʼ���������������Ӧ�Ŀռ�ֲ����������С��γ��
		double startLongitude = floor(pos[0] * 100) / 100;
		double endLongitude = ceil(pos[1] * 100) / 100;
		double startLatitude = floor(pos[2] * 100) / 100;
		double endLatitude = ceil(pos[3] * 100) / 100;

		//������Ҫ���г�ʼ���Ŀռ�ֲ���ľ�������������Ϣ
		int row = round(endLongitude * 100 - startLongitude * 100);
		int col = round(endLatitude * 100 - startLatitude * 100);

		//���жԶ�Ӧ�����Ŀռ�ֲ���ĳ�ʼ��
		position_distribution_inBeiJing** newpdi = new position_distribution_inBeiJing * [row];
		for (int i = 0; i < row; i++)
		{
			newpdi[i] = new position_distribution_inBeiJing[col];
		}

		//���ݸ�����������Ϣ����ȫ���ڷ�Χ�ڵĿռ�ֲ�����ĳ�ʼ��
		for (double d1 = startLongitude; d1 < endLongitude; d1 += 0.01)
		{
			for (double d2 = startLatitude; d2 < endLatitude; d2 += 0.01)
			{
				//���л��ռ�ֲ������
				newpdi[(int)((d1 - startLongitude) * 100)][(int)((d2 - startLatitude) * 100)] = boost_iarchive_position_in(d1, d2);
			}
		}

		//�жϸ����ĳ�ʼ��ʼ����ʱ���Ƿ����ʵ�����ݼ�Ҫ��
		if (day >= 2 and day <= 8 and hour >= 0 and hour <= 59 and minute >= 0 and minute <= 59)
		{
			//����Ҫ��,����ȫ�������л�����Ϣ��
			for (int i = 0; i < row; i++)
			{
				for (int j = 0; j < col; j++) {

					//�����ֲ��������������,����Ҫ��͸���counttaxi��taxiflag��temp
					set<Point, CMP> temp = newpdi[i][j].counttaxi(day, hour, minute, pos, counttaxi, time_span, taxiflag);

					//��s����temp�洢�����е���Ϣ
					s.insert(temp.begin(), temp.end());
				}
			}

			//����ʹ�ô�,����s����Ϣ��counttaxi���з���
			if (output) {
				cout << "in here and in this time , taxinum =" << counttaxi << endl;

				for (auto i : s)
				{
					cout << i.gettaxiname() << " " << i.getday() << " " << i.gethour() << " " << i.getminute() << " " << i.getsecond() << " " << i.getLongitude() << "  " << i.getLatitude() << endl;
				}
			}
		}

		//ʹ�ù������,ɾ�������л���Ϣ
		for (int i = 0; i < row; i++)
		{
			delete[] newpdi[i];
		}
		delete[]newpdi;
	}

	//������㲻���ڱ�����,����ʱ��ֲ��������Ϣ��ʼ��
	else
	{

		//�жϸ����ĳ�ʼ��ʼ����ʱ���Ƿ����ʵ�����ݼ�Ҫ��
		if (day >= 2 and day <= 8 and hour >= 0 and hour <= 59 and minute >= 0 and minute <= 59)
		{
			//����ʱ��ֲ�������
			time_distribution* newtd;

			//ͳ��ʱ��ֲ��������С
			int countnum;

			//������Ҫ���г�ʼ����ʱ��ֲ��������Ԫ�ظ�����Ϣ
			//�����ʼʱ��+ʱ�䳤��<=���ݼ����һ�����һ����,����Ԫ�ظ���=���ݼ����ʱ��-��ʼʱ��(�Է���Ϊ��λ)
			if (gettime(day, hour, minute, 0, 8, 23, 59, 59) + 1 >= 60 * time_span)
			{
				newtd = new time_distribution[time_span];
				countnum = time_span;
			}

			//�����ʼʱ��+ʱ�䳤��>���ݼ����һ�����һ����,����Ԫ�ظ���=ʱ�䳤��(�Է���Ϊ��λ)
			else
			{
				countnum = (8 - day) * 24 * 60 + (23 - hour) * 60 + 59 - minute;
				newtd = new time_distribution[countnum];
			}

			//�ӳ�ʼʱ�俪ʼ����ÿһ���ӵ�������������ʱ��������ݼ�����ʱ��
			int dayreal = day, hourreal = hour, minutereal = minute;
			for (int i = 0; i < countnum; i++) {

				//���л�ʱ��ֲ������
				newtd[i] = boost_iarchive_time(dayreal, hourreal, minutereal);
				minutereal++;
				//������һ��ʱ���Ƿ�Ҫ��Сʱ/��
				if (minutereal > 59)
				{
					minutereal %= 60;
					hourreal++;
				}
				if (hourreal > 23)
				{
					hourreal %= 24;
					dayreal++;
				}
			}

			//����ȫ�������л�����Ϣ��
			for (int i = 0; i < countnum; i++)
			{
				//�����ֲ��������������, ����Ҫ��͸���counttaxi��taxiflag��temp
				set<Point, CMP> temp = newtd[i].counttaxi(day, hour, minute, pos, counttaxi, taxiflag);
				s.insert(temp.begin(), temp.end());
			}

			//��������ʹ�ô�
			if (output) {
				cout << "in here and in this time , taxinum =" << counttaxi << endl;
				for (auto i : s)
				{
					cout << i.gettaxiname() << " " << i.getday() << " " << i.gethour() << " " << i.getminute() << " " << i.getsecond() << " " << i.getLongitude() << "  " << i.getLatitude() << endl;
				}
			}

			//ʹ�ý���,ɾ�������л���Ϣ
			delete[]newtd;
		}
	}
	//���ؾ��������ڵĳ��⳵����Ŀ
	return counttaxi;
}


//��������2ʵ��
void function_regional_density_analysis(int r, int day, int hour, int minute, int time_span, bool output)
{
	//�жϸ����ĳ�ʼ��ʼ����ʱ���Ƿ����ʵ�����ݼ�Ҫ��
	if (day >= 2 and day <= 8 and hour >= 0 and hour <= 59 and minute >= 0 and minute <= 59)
	{
		//����ʱ��ֲ�������
		time_distribution* newtd;

		//ͳ��ʱ��ֲ��������С
		int countnum;

		//������Ҫ���г�ʼ����ʱ��ֲ��������Ԫ�ظ�����Ϣ
		//�����ʼʱ��+ʱ�䳤��<=���ݼ����һ�����һ����,����Ԫ�ظ���=���ݼ����ʱ��-��ʼʱ��(�Է���Ϊ��λ)
		if (gettime(day, hour, minute, 0, 8, 23, 59, 59) + 12 >= 60 * time_span)
		{
			countnum = time_span;
			newtd = new time_distribution[countnum];
		}

		//�����ʼʱ��+ʱ�䳤��>���ݼ����һ�����һ����,����Ԫ�ظ���=ʱ�䳤��(�Է���Ϊ��λ)
		else
		{
			countnum = (8 - day) * 24 * 60 + (24 - hour) * 60 + 59 - minute;
			newtd = new time_distribution[countnum];
		}

		//����ͳ�Ƶ�ʱ��θ���,����ʱ����Ϊ10����,����changetime���޸�
		int changesize = ceil(countnum / changetime);

		//�ӳ�ʼʱ�俪ʼ����ÿһ���ӵ�������������ʱ��������ݼ�����ʱ��
		int dayreal = day, hourreal = hour, minutereal = minute;
		for (int i = 0; i < countnum; i++) {
			
			//���л�ʱ��ֲ������
			newtd[i] = boost_iarchive_time(dayreal, hourreal, minutereal);

			//������һ��ʱ���Ƿ�Ҫ��Сʱ/��
			minutereal++;
			if (minutereal > 59)
			{
				minutereal %= 60;
				hourreal++;
			}
			if (hourreal > 23)
			{
				hourreal %= 24;
				dayreal++;
			}
		}

		//���㱱���ڰ�r�����ָ�ɵľ�γ�ȸ��Զ�Ӧ���������
		//��1��Ӧ��Ե�ĵ�
		int countrnumLong = ceil(2200 / r) + 1;
		int countrnumLa = ceil(1700 / r) + 1;

		//������Ӧ������r�����������,���г�ʼ��
		position_distribution_r*** pdr = new position_distribution_r * *[countrnumLong];
		for (int i = 0; i < countrnumLong; i++)
		{
			pdr[i] = new position_distribution_r * [countrnumLa];
			for (int j = 0; j < countrnumLa; j++)
			{
				pdr[i][j] = new position_distribution_r(r, changesize);
			}
		}

		//������������������,���г�ʼ��
		position_distribution_outr* pdor = new position_distribution_outr(changesize);

		//����dtemp1��dtemp2�洢Ҫ�����ĵ�ľ�γ��
		double dtemp1, dtemp2;

		//ѭ���������Ҫ������е���Ϣ(��ʱ��ֲ�����),����Щ��Ϣ��r��������ʽ���б���
		//��ʱ��ηָ����
		for (int n = 0; n < changesize; n++)
		{
			//������ʱ��α������Ϣ
			for (int i = 0; i < changetime; i++)
			{
				int index = n * changetime + i;
				
				//���������Χ��ֹͣ
				if (index >= countnum) { break; }

				//����ȫ��ʱ�䷶Χ�ڵĵ���Ϣ
				unordered_map<int, vector<Point>> temp = newtd[index].getallpoint();
				int t = 0;
				
				//��������Ϣ,��Ҫ����й���
				for (auto j : temp)
				{
					for (auto k : j.second)
					{
						dtemp1 = k.getLongitude();
						dtemp2 = k.getLatitude();
						
						//��������ڱ����ھͱ��浽��Ӧ��r������������
						if (dtemp1 >= 115.40 and dtemp1 <= 117.60 and dtemp2 >= 39.40 and dtemp2 <= 41.10)
						{
							pdr[(int)floor((dtemp1 - 115.40) / r * 1000)][(int)floor((dtemp2 - 39.40) / r * 1000)]->insert(k, n);
						}
						//������ͱ��浽������
						else
						{
							pdor->insert(k, n);
						}
					}
				}
			}

			//�������򱣴���Ϣ�����û���������
			if (output) {
				cout << "n:" << n << endl;
				for (int i = 0; i < countrnumLong; i++)
				{
					for (int j = 0; j < countrnumLa; j++)
					{
						if (pdr[i][j]->getcountsize(n) >= 30)
						{
							cout << "pdr[" << i << "][" << j << "]" << endl;
							cout << pdr[i][j]->getcountsize(n) << endl;
						}
					}
				}
				cout << "podr:" << endl;
				cout << pdor->getcountsize(n) << endl;
			}
		}

		//����ִ�����,ɾ��ȫ����ʱ�����л���Ϣ��r������Ϣ
		for (int i = 0; i < countrnumLong; i++)
		{
			for (int j = 0; j < countrnumLa; j++)
			{
				delete pdr[i][j];
			}
			delete[]pdr[i];
		}
		delete[]pdr;
		delete pdor;
	}
}


//��������3ʵ��
set<Point, CMP> function_region_association_analysis_one(double* pos1, double* pos2, int day, int hour, int minute, int time_span, bool output)
{
	//�½�����1��2�;������ߵĳ����Ķ�Ӧ��Ϣ����
	set<Point, CMP>* information_rect1 = new set<Point, CMP>;
	set<Point, CMP>* information_rect2 = new set<Point, CMP>;
	set<Point, CMP>* information_rect_exchange = new set<Point, CMP>;

	//���ú�������1,��þ���1��2�ڵ�ȫ�������Ϣ
	function_findtaxinum_somewhere(*information_rect1, pos1, day, hour, minute, time_span,false);
	function_findtaxinum_somewhere(*information_rect2, pos2, day, hour, minute, time_span,false);

	//taxicmp���ڱ��������������ж����ֵĳ��⳵�������
	set<int> taxicmp;

	//����1����
	for (auto i : *information_rect1)
	{
		//�Ȳ���1��ȫ�����⳵�����
		taxicmp.insert(i.gettaxiname());
	}

	//�ٱ���taxicmp(��ʱ��Ӧ������1��ȫ�����⳵�������Ϣ)
	for (set<int>::iterator iter = taxicmp.begin(); iter != taxicmp.end();)
	{

		//�������
		int flag = 1;

		//����ܹ���2�з��������Ϣ
		for (auto j : *information_rect2)
		{
			if (j.gettaxiname() == *iter)
			{
				//�������
				flag = 0;
			}
		}

		//û����˵��û�н��г�������,��Ǵ�ʱ��һ
		if (flag)
		{
			//��û���ֵ������ɾ��
			iter = taxicmp.erase(iter);
		}
		else iter++;
	}

	//��ʱtaxicmp������ȫ���ľ���1��2�Ľ������⳵�����,��������
	for (auto i : taxicmp)
	{
		//����1����,���ͳ��ֵ��������صĵ���Ϣ���б���
		for (auto j : *information_rect1)
		{
			if (j.gettaxiname() == i)
			{
				(*information_rect_exchange).insert(j);
			}
		}
		//����2����,���ͳ��ֵ��������صĵ���Ϣ���б���
		for (auto j : *information_rect2)
		{
			if (j.gettaxiname() == i)
			{
				(*information_rect_exchange).insert(j);
			}
		}
	}
	//�����ȫ���ĳ������⳵�켣����Ϣ,�����û�����������
	if (output) {
		cout << "taxi exchange number: " << taxicmp.size() << endl;
		for (auto i : *information_rect_exchange) {
			cout << "taxi information: " << i.gettaxiname() << " " << i.getLongitude() << " " << i.getLatitude() << " " << i.gettimes() << endl;
		}
	}

	//���ܵ��ý���,��վ�����Ϣ,�ڳ��ڴ�Ϊ��������ʹ��
	information_rect1->clear();
	information_rect2->clear();
	delete information_rect1;
	delete information_rect2;
	return *information_rect_exchange;
}


//��������4ʵ��
void function_region_association_analysis_two(double* pos, int day, int hour, int minute, int time_span, bool output)
{
	//�½�Ҫ�жϵľ��κ;����þ��μ����ĳ����Ķ�Ӧ��Ϣ����
	set<Point, CMP>* information_rect = new set<Point, CMP>;
	set<Point, CMP>* information_rect_exchange = new set<Point, CMP>;

	//���ú�������1,����жϾ����ڵ�ȫ�������Ϣ
	function_findtaxinum_somewhere(*information_rect, pos, day, hour, minute, time_span,false);

	//taxicount�ڱ����жϾ��ε�ȫ�����⳵�����
	set<int> taxicount;

	//taxicount_exchange�ڱ������жϾ����ڳ������������ֵ�ȫ�����⳵�����
	set<int> taxicount_exchange;

	//�����жϾ���,���ͳ��ֵ��������صĵ���Ϣ���б���
	for (auto i : *information_rect)
	{
		taxicount.insert(i.gettaxiname());
	}

	//taxisize���������ͷ�ļ�����,�Ǿֲ�����,��Ӧ�жϾ����ڳ��⳵���������
	int taxisize = taxicount.size();

	//�½�taxi����,��СΪ�жϾ����ڳ��⳵���������
	taxi* newvtaxi = new taxi[taxisize];
	int j = 0;
	
	//����taxicount�ĳ��⳵�����,����Ӧ�ĳ��⳵�ļ������л�,��øó��⳵ȫ����Ϣ
	for (auto i : taxicount) {
		newvtaxi[j++] = boost_iarchive_taxi(i);
	}

	//������õ�ȫ�����⳵��Ϣ,�жϷ�����ʱ����ڵ����й켣���Ƿ�û�г����ھ�����
	for (int i = 0; i < taxisize; i++)
	{
		int flag = 1;
		//����ĳ������ȫ���켣��
		for (int j = 0; j < newvtaxi[i].getsize(); j++)
		{
			//�ݴ��
			Point tppot = newvtaxi[i].getPoint(j);
			int timetemp = gettime(day, hour, minute, 0, tppot.getday(), tppot.gethour(), tppot.getminute(), tppot.getsecond());
			//�ݴ�������ʱ�䷶Χ��
			if (timetemp < 0)
			{
				continue;
			}
			else if (timetemp >= 0 and timetemp <= time_span * 60)
			{
				//�ݴ�㲻�ھ�����,˵�������������
				if (find((*information_rect).begin(), (*information_rect).end(), tppot) == (*information_rect).end())
				{
					//��������������Ϣ
					(*information_rect_exchange).insert(tppot);
					flag = 0;
				}
			}
			//�����ʱ�䷶Χȫ���������,ֹͣ��������
			else
			{
				break;
			}
		}

		//�������������
		for (auto i : *information_rect_exchange)
		{
			//ÿ����ȥ��Ӧ�����ڵĵ�
			for (auto j : *information_rect)
			{
				//�����һ�����ľͰѾ����ڵ���ϢҲ���뽻��������,�൱�����˶��μ���
				if (i.gettaxiname() == j.gettaxiname())
					(*information_rect_exchange).insert(j);
			}
		}

		//���������⳵������ǵ�һ�α����浽����������,���⳵��������뵽�����ļ�����
		if (!flag)
		{
			taxicount_exchange.insert(newvtaxi[i].getname());
		}
	}

	//�����û���Ҫ��������Ϣ
	if (output) {
		cout << "taxicount_exchange: " << taxicount_exchange.size() << endl;
		for (auto i : *information_rect_exchange)
		{
			cout << "taxi information: " << i.gettaxiname() << " " << i.getLongitude() << " " << i.getLatitude() << " " << i.gettimes() << endl;
		}
	}
	//���ܵ��ý���,��վ�����Ϣ,�ڳ��ڴ�Ϊ��������ʹ��
	information_rect->clear();
	information_rect_exchange->clear();
	delete information_rect;
	delete information_rect_exchange;
	delete[]newvtaxi;
}
