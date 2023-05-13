#include <iostream>
#include <vector>
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

//Split�������ڽ���ʼ���ݼ��еĳ��⳵ÿ�еĵ���Ϣ������Ч��Ϣ
vector<string> Split(const string& str, char split) {
	
	//����istringstream����,���ڶ�ȡstr�ַ������� 
	istringstream ss(str);

	//�ݴ��item�ַ�������
	string item;
	
	//�������Ϣ��ɵ����ݵĴ洢����
	vector<std::string> elems;
	
	//ѭ����ȡss�����ڵĴ�������,ÿ�ζ�ȡ��ֹͣ����splitΪ׼�������ݼ���Ϊ���ţ�
	while (getline(ss, item, split)) {
		if (!item.empty()) {
			elems.push_back(item);
		}
	}

	//�����̺������Ϣ�Ķ�Ӧ����
	return elems;
}


//���⳵�����л�ʵ��:
//ÿ�����л�1000�����⳵(����̫����),Ȼ�����л��Ľ�������ڴ��̶�Ӧ·����
const int taxiinit_pos = 0;        //���л����⳵����ʼλ�á�
const int taxiinit_num = 1000;     //ÿ�����л����⳵������
void boost_archive_taxi() {
	
	//���⳵���л��ܽ��д��Ĵ���Ŀ¼���ļ���,����˴�������taxitotal0-10��һ���ļ���;
	ofstream ofs("E:\\taxitotal\\taxitotal" + to_string(taxiinit_pos / taxiinit_num));

	//�ı�������鵵�࣬ʹ��һ��ofstream������
	boost::archive::text_oarchive oa(ofs);
	
	//�½�taxiָ��ָ����Ҫ������taxi�����С
	taxi* vtaxi = new taxi[taxiinit_num];

	//���ݼ��ļ�Ŀ¼
	string k = "F:\\vs 2019gongzuoqu\\bighomework\\bighomework\\taxi_log_2008_by_id\\";
	ifstream fs;

	//�½���ά�����������ݼ��ļ����ļ���������������
	vector<vector<string>> vs;

	//ѭ�������Ӧ�ĳ��⳵��Ϣ
	for (int i = taxiinit_pos; i < taxiinit_num + taxiinit_pos; i++) {

		//�����ݼ���ĳ�����⳵�ļ�
		fs.open(k + to_string(i + 1) + ".txt", ios::in);

		//����ܹ���
		if (fs.is_open())
		{
			int j = 0;
			string temp;

			//���ж�ȡ��Ϣ, ÿ����Ϣ��������������Ϻ���Ϊһ��Ԫ�ط���������
			while (getline(fs, temp))
			{
				vs.push_back(Split(temp, ','));
				j++;
			}

			//�������Ϣֱ�ӽ�taxi����������������ݽ��г�ʼ��,û����Ϣ�������κβ���(��ʱtaxi����Ϣ��ΪĬ�Ϲ��캯���Ķ�Ӧ��Ϣ)
			if (j)
			{
				vtaxi[i - taxiinit_pos].taxi_init(vs);
			}

		}

		//�򲻿������Ӧ������Ĵ�����Ϣ
		else cout << i + 1 << "is error!" << endl;
		
		//�ر��ļ����������Ϣ,����ɹ���Ϣ,׼��������һ�εĴ���,
		fs.close();
		vs.clear();
		vector<vector<string>>().swap(vs);
		cout << i + 1 << "is ok" << endl;

	}

	//��������д�����ĳ��⳵��Ϣ�������л�,���ļ��б�������һ���ֵ�ȫ�����⳵��Ϣ
	for (int i = 0; i < taxiinit_num; i++) {
		oa << vtaxi[i];
	}

	//�ر�д���ļ�
	ofs.close();

	//ѭ���򿪶���ļ�����д��,ÿ���ļ��洢һ�����⳵��Ϣ,�ļ����Ƹ��ݳ��⳵����������
	for (int i = taxiinit_pos; i < taxiinit_num + taxiinit_pos; i++)
	{
		ofs.open("E:\\taxitotal\\taxi" + to_string(i + 1));
		boost::archive::text_oarchive oa2(ofs);
		oa2 << vtaxi[i - taxiinit_pos];
		ofs.close();
	}

	//д�����,ɾ����Ӧ�ݴ�ĳ��⳵����
	delete[]vtaxi;

	return;
}


//���⳵�෴���л�ʵ��:
//���ݴ���ĳ��⳵�����������Ӧ�ĵ���
taxi boost_iarchive_taxi(int i) {

	//������ʱ����
	taxi newvtaxi;

	//����Ҫ�ĳ��⳵������ļ�
	ifstream ifs;
	ifs.open("E:\\taxitotal\\taxi" + to_string(i), std::ios::binary);
	
	//�ı�������鵵�࣬ʹ��һ��ifstream������
	boost::archive::text_iarchive ia(ifs);

	//���÷����л���taxi�����ʼ��
	ia >> newvtaxi;
	
	//��������Ϣ���ر��ļ�
	cout << i << "finish!" << endl;
	ifs.close();
	return newvtaxi;
}


//taxi�����л���Ϣ���뵽�ļ��н���������ʾ,������������
void taxifile(taxi& t) 
{
	//��д���ļ�
	ofstream ofs;
	ofs.open("E:\\taxibase\\taxi" + to_string(t.getname())+".txt", ios::trunc);
	
	//����Ҫд��ĳ��⳵��ȫ����Ч�켣��,������Ч��Ϣȫ��д���ļ���ÿ����
	for (int i = 0; i < t.getsize(); i++) 
	{
		Point temp = t.getPoint(i);
		ofs << t.getname() << ",2008-02-0" << temp.getday() << " " << temp.gethour() << ":" << temp.getminute() << ":" << temp.getminute() << "," << setiosflags(ios::fixed) << setprecision(5) << temp.getLongitude() << "," << temp.getLatitude() << "\n";
	}
	return;
}


//���⳵ʱ��ֲ������л�ʵ��:
//�����ѱ���ϴ�����ݵĳ��⳵��Ϣ���е���
//���ڶԳ��⳵�켣��ʱ����Ϣ�ķ���ʹ���,�Է���Ϊ��λ
//ÿ�δ���һ�������(��Ȼ�ڴ治����),Ȼ�����л��Ľ�������ڴ��̶�Ӧ·����
const int dayfor_time = 2;   //�����������
void boost_archive_time() 
{
	//�½�time_distributionָ��ָ����Ҫ������time_distribution�����С
	//�˴�Ϊ24*60,��Ӧһ���ȫ����Ϣ
	time_distribution***td = new time_distribution**[1];
	for(int i=0;i<1;i++)
	{
		td[i] = new time_distribution * [24];
		for(int j=0;j<24;j++)
		{
			td[i][j] = new time_distribution[60];
		}
	}

	//ѭ����ȡȫ�����⳵��Ϣ�������л�׼��,Ȼ�󽫳��⳵��Ӧ��ʱ����Ϣ�����Ӧ�������Ϣ����
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
		//�д��������������Ϣ,���������ȷ��Ϣ
		else cout << i+1 << "is error!" << endl;
		ifs.close();
		cout << i + 1 << "is ok" << endl;
	}

	//ѭ���򿪶���ļ�����д��,ÿ���ļ��洢һ��ʱ��ֲ�����Ϣ,�ļ����Ƹ��ݶ�Ӧʱ�����
	ofstream ofs;
	for (int i = 0; i < 1; i++) {
		for (int j = 0; j < 24; j++)
		{
			for (int k = 0; k < 60; k++) {
				ofs.open("E:\\time_taxidata\\" + to_string(dayfor_time) + "��-" + to_string(j) + "ʱ-" + to_string(k) + "��");
				boost::archive::text_oarchive oa(ofs);
				oa << td[i][j][k];
				ofs.close();
			}
		}
	}

	//д�����,ɾ����Ӧ�ݴ��ʱ��ֲ�������
	delete[]td;
	return;
}


//ʱ��ֲ��෴���л�ʵ��:
//���ݴ����ʱ����Ϣ������Ӧ�ĵ���
time_distribution boost_iarchive_time(int i, int j, int k) {
	
	//������ʱ����
	time_distribution newtd;

	//�򿪶�Ӧ�ļ�
	ifstream ifs;
	ifs.open("E:\\time_taxidata\\" + to_string(i) + "��-" + to_string(j) + "ʱ-" + to_string(k) + "��", std::ios::binary);
	
	//�ı�������鵵�࣬ʹ��һ��ifstream������
	boost::archive::text_iarchive ia(ifs);
	
	//���÷����л���time_distribution�����ʼ��
	ia >> newtd;

	//��������Ϣ���ر��ļ�
	cout << i << "��" << j << "ʱ" << k << "��" << "finish" << endl;
	ifs.close();
	return newtd;
}


//���⳵�����ڿռ�ֲ������л�ʵ��:
//�����ѱ���ϴ�����ݵĳ��⳵��Ϣ���е���
//���ڶԳ��⳵�켣�㱱���ڿռ���Ϣ�ķ���ʹ���,��0.01��γ�ȱ仯Ϊ��λ
//ÿ�δ���20*170������,����0.2����-1.7γ�ȱ仯��Χ
const double number = 0.0;    //�仯����,���һ�α仯��116.40~116.60���Ⱥ�39.40~41.10γ��
const double addLong = 0.2;   //�仯�ľ��ȴ�С,ÿ�����л�0.2����
const double pos_init_Longitude = 115.40 + number * addLong; //ʵ�����л�����ʼ����
const double pos_init_Latitude = 39.40; //ʵ�����л�����ʼγ��
void boost_archive_position_in()
{

	//�½�position_distribution_inBeiJingָ��ָ����Ҫ������position_distribution_inBeiJing�����С
	//�˴�Ϊ20*170,��Ӧһ���ռ䷶Χ����Ϣ
	position_distribution_inBeiJing** pdi = new position_distribution_inBeiJing * [20];
	for (int i = 0; i < 20; i++)
	{
		pdi[i] = new position_distribution_inBeiJing[170];
	}

	//ѭ����ȡȫ�����⳵��Ϣ�������л�׼��,Ȼ�󽫳��⳵��Ӧ�Ŀռ���Ϣ�����Ӧ�������Ϣ����
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
		//�д��������������Ϣ,���������ȷ��Ϣ
		else cout << i + 1 << "is error!" << endl;
		ifs.close();
		cout << i + 1 << "is ok" << endl;
	}

	//ѭ���򿪶���ļ�����д��,ÿ���ļ��洢һ���ռ�ֲ�����Ϣ,�ļ����Ƹ��ݶ�Ӧ�ռ����
	ofstream ofs;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 170; j++)
		{
			ofs.open("E:\\pos_taxidata\\inbeijing\\" + to_string(double(i) * 0.01 + pos_init_Longitude) + "����," + to_string(double(j * 0.01) + pos_init_Latitude) + "γ��");
			boost::archive::text_oarchive oa(ofs);
			oa << pdi[i][j];
			ofs.close();
		}
	}
	//д�����,ɾ����Ӧ�ݴ�Ŀռ�ֲ�������
	delete[]pdi;
	return;
}


//���⳵������ռ�ֲ������л�ʵ��:
//�����ѱ���ϴ�����ݵĳ��⳵��Ϣ���е���
//���ڶԳ��⳵�켣�㱱����ռ���Ϣ�Ĵ���
void boost_archive_position_out() 
{
	//�½�position_distribution_outBeiJing����
	position_distribution_outBeiJing p;

	//ѭ����ȡȫ�����⳵��Ϣ�������л�׼��,Ȼ�󽫳��⳵��Ӧ�Ŀռ���Ϣ�����Ӧ�������Ϣ����
	for (int i = 0; i < taxisize; i++) {
		taxi temp;
		ifstream ifs("E:\\taxitotal\\taxi" + to_string(i + 1));
		boost::archive::text_iarchive ia(ifs);
		if (ifs.is_open())
		{
			ia >> temp;
			p.position_distribution_outBeiJing_update(temp);
		}
		//�д��������������Ϣ,���������ȷ��Ϣ
		else cout << i + 1 << "is error!" << endl;
		ifs.close();
		cout << i + 1 << "is ok" << endl;
	}

	//�򿪵����ļ�����д��,��洢�˱������ȫ������Ϣ
	ofstream ofs;
	ofs.open("E:\\pos_taxidata\\outbeijing\\outbeijing");
	boost::archive::text_oarchive oa(ofs);
	oa << p;
	ofs.close();
}


//���⳵�����ڿռ�ֲ��෴���л�ʵ��:
//���ݴ���Ŀռ���Ϣ������Ӧ�ĵ���(����Ŀռ���ϢӦΪ�Ѿ����������Ϣ)
position_distribution_inBeiJing boost_iarchive_position_in(double i,double j)
{

	//������ʱ����
	position_distribution_inBeiJing newpdi;

	//�򿪶�Ӧ�ļ�
	ifstream ifs;
	ifs.open("E:\\pos_taxidata\\inbeijing\\" + to_string(i) + "����," + to_string(j) + "γ��", std::ios::binary);

	//�ı�������鵵�࣬ʹ��һ��ifstream������
	boost::archive::text_iarchive ia(ifs);

	//���÷����л���position_distribution_inBeiJing�����ʼ��
	ia >> newpdi;

	//��������Ϣ���ر��ļ�
	cout << i << "����" << "," << j << "γ��" << "finish" << endl;
	ifs.close();
	return newpdi;
}


//���⳵�����ڿռ�ֲ��෴���л�ʵ��:
//����Ψһֱ�ӷ����л�����
position_distribution_outBeiJing boost_iarchive_position_out() 
{

	//������ʱ����
	position_distribution_outBeiJing newp;
	
	//�򿪶�Ӧ�ļ�
	ifstream ifs;
	ifs.open("E:\\pos_taxidata\\outbeijing\\outbeijing",ios::binary);

	//�ı�������鵵�࣬ʹ��һ��ifstream������
	boost::archive::text_iarchive ia(ifs);

	//���÷����л���position_distribution_inBeiJing�����ʼ��
	ia >> newp;

	//��������Ϣ���ر��ļ�
	cout << "outbeijing finish" << endl;
	ifs.close();
	return newp;
}


//���⳵·�����+��+ͼ������л�ʵ��:
//����ͨ����������(����º�����)ʵ������ͼ,�ٸ��ݱ�ͼ���ݽ������л�,��ͼ��ĳ�Ա����
//���ڶԳ��⳵���ڹ켣��·����Ϣ�Ĵ���,�Բ����ǳ�·��(û�л������ݵĻ�������������Χ������ʱ�մ���̫��,����û��ͳһ��һ����׼)
//���㡢�ߺ�ͼ����һ����ȫ������(�ڴ�պù���)
void pathgraph::pathserialization()
{

	//����ȫ�����,ÿ��Ԫ�����л���һ���ļ���
	for (auto i : pathNodemap)
	{
		ofstream ofs;
		ofs.open("D:\\pathdata\\pathNode\\Node" + i.first);
		boost::archive::text_oarchive oa(ofs);
		oa << *i.second;
		ofs.close();
	}

	//����ȫ����,ÿ��Ԫ�����л���һ���ļ���
	for (auto i : pathEdgemap)
	{
		ofstream ofs;
		ofs.open("D:\\pathdata\\pathEdge\\Edge" + i.first);
		boost::archive::text_oarchive oa(ofs);
		oa << *(i.second);
		ofs.close();
	}

	//������ͼ�������л���һ���ļ���
	ofstream ofs;
	ofs.open("D:\\pathdata\\pathdatagraph");
	boost::archive::text_oarchive oa(ofs);
	oa << *this;
	ofs.close();
}


//���⳵·�����+��+ͼ��ķ����л�ʵ��:
//���������л����ͱ�,�����������ͼ�����л�,��ͼ����Ϣ���в���
//��Ϊ������ʱ����Ĵ���̫��,����ֻ��ѡ�������ò��޸ĸñ���
void boost_pathserialization_iarchive(pathgraph &p)
{

	//�򿪶�Ӧ�ļ�
	ifstream ifs;
	ifs.open("E:\\pathdata\\pathdatagraph");

	//�ı�������鵵�࣬ʹ��һ��ifstream������
	boost::archive::text_iarchive ia(ifs);
	
	//���÷����л���pathgraph�����ʼ��
	ia >> p;

	//��������Ϣ���ر��ļ�
	ifs.close();
	cout << "graph finish" << endl;
	return ;
}