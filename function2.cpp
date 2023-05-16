#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include "taxi_header_use.h"
#include "taxi_init.h"

//��������5:Ƶ��·������1��һ��·����Ƶ���ȿ��Զ���Ϊ����·����ͨ�е������������������û������Ĳ���k���������x��ͳ�������������У����ȳ���x����ΪƵ����ǰk��·��
//�������x��ʾ����,k��ʾ����
//���ؽ�㾭γ����Ϣ����,ÿ��������Ԫ��Ϊһ������,�洢�ĸ�Ԫ��,�ֱ��Ӧ�˽��1�ľ��ȡ�γ��,���2�ľ��Ⱥ�γ��
vector<vector<string>> function_path_frequent_analysis1(double x,int k, bool output)
{
	//�ж�k��x�Ƿ����
	if (k >= 0 and k < 10000 and x >= 0) {
		
		//���������Ѿ�����õ�file�ļ�(�洢��pathEdge�Ķ���)��class�ļ�(�洢�����Ϣ)
		ifstream ifs1("D:\\pathdata\\pathsortedge_forcount_file", ios::in | ios::binary);
		ifstream ifs2("D:\\pathdata\\pathsortedge_forcount_class.txt", ios::in);

		//�����������
		vector<pathEdge> result;

		//�Ƚ������������Ϊk��С
		for (int i = 0; i < k; i++)
		{
			pathEdge temp;
			result.push_back(temp);
		}

		int i = 0;
		
		//��������Ҫ��ı߶�Ӧ�Ľ�㾭γ����Ϣ(Ŀǰ�޷�д���ȡ�����ָ�����)
		vector<vector<string>> vstemp;

		//����ȫ��pathEdge����
		while (ifs1.read((char*)(&result[i]), sizeof(pathEdge)))
		{
			//��õ�ǰ�߶�Ӧ�Ľ�㾭γ�Ⱥ�������Ϣ
			string stemp;
			getline(ifs2, stemp, '\n');

			//�����Ѿ�����Ƶ��������,��������,�ж��䳤���Ƿ�Ҫ��
			if (result[i].getpathlen() >= x)
			{
				//����Ҫ��ʱ��ÿ�н�㾭γ����Ϣ���浽vstemp��
				vstemp.push_back(Split(stemp, ','));

				//++i��ʾ�Ѿ��洢�������Ϣ������һ��
				i++;

			}
			//����õ���k������Ҫ���·�����Խ���
			if (i == k) 
			{ break; }
		}
		if (output) {
			ofstream ofs("D:\\result\\function_path_frequent_analysis1.txt",ios::trunc);
			ofs << i << '\n';
			//������������е���Ч·������
			for (int j = 0; j < i; j++)
			{
				//�����������е���Ч��Ϣ�����汣��Ľ�㾭γ����Ϣ
				ofs << setiosflags(ios::fixed) << setprecision(5)
					<< vstemp[j][0] << ","
					<< vstemp[j][1] << ","
					<< vstemp[j][2] << ","
					<< vstemp[j][3] << ","
					<< result[j].getpathcount() << ","
					<< result[j].getpathlen() << '\n';
			}
			ofs.close();
		}
		return vstemp;
	}

	//k��x�������������������Ϣ
	else 
	{
		ofstream ofs("D:\\result\\function_path_frequent_analysis1.txt", ios::trunc);
		ofs << -1 << '\n';
		ofs.close();
		vector<vector<string>> vstemp;

		//������Ч��Ϣ
		return vstemp;
	}
	
}


//��������6:Ƶ��·������2������������������A��B��������A��B����ΪƵ����ǰk��ͨ��·����
//�������pos1��ʾ����1,pos2��ʾ����2,k��ʾ����
//���ؽ�㾭γ����Ϣ����,ÿ��������Ԫ��Ϊһ������,�洢�ĸ�Ԫ��,�ֱ��Ӧ�˽��1�ľ��ȡ�γ��,���2�ľ��Ⱥ�γ��
vector<vector<double>> function_path_frequent_analysis2(double* pos1, double* pos2, int k, bool output)
{
	//�������ǰk��Ƶ��·���ռ�
	vector<pathEdge> k_for;

	//���ڷ��صĽ�㾭γ����Ϣ����
	vector<vector<double>> result = {};

	//���ú�������3������������εĳ���������Ϣ
	set<Point, CMP> information_rect_exchange = function_region_association_analysis_one(pos1, pos2, 2, 0, 0, 60 * 24 * 7, false);
	
	//�洢���������ĳ��⳵����
	set<int> taxi_exchange_name;

	//��ó��������ĳ��⳵����
	for (auto i : information_rect_exchange) 
	{
		taxi_exchange_name.insert(i.gettaxiname());
	}

	//�������⳵ָ��
	taxi* taxi_use = new taxi[taxi_exchange_name.size()];
	int index = 0;
	//�Գ��������ĳ��⳵���з����л�
	for (auto i : taxi_exchange_name) 
	{
		taxi_use[index++] = boost_iarchive_taxi2(i);
	}

	//����·����ͼ,���ڷ�����������
	pathgraph p;

	//ѭ��������⳵
	for (int i = 0; i < taxi_exchange_name.size(); i++) {

		//����Ƿ��е���information_rect_exchange�г��ֹ�
		int flag = 0;

		//��¼��ʼ�����ڽ�������ĵ�λ��,���ս��������ڽ�������ĵ�λ��,��ÿ�����ֵ��ڳ��⳵�Ķ�Ӧλ��
		pair<pair<int,int>, vector<int>> record = {};

		//ѭ�����⳵��ÿ����
		for(int j=0;j<taxi_use[i].getsize();j++)
		{
			Point temp = taxi_use[i].getPoint(j);

			//���û���ֹ���������ĵ������
			if (!flag and information_rect_exchange.find(temp) == information_rect_exchange.end())
			{
				continue;
			}
			//��һ�γ��ֱ���λ�ú���Ϣ,�ñ��Ϊ1
			else if (!flag and information_rect_exchange.find(temp) != information_rect_exchange.end())
			{
				flag = 1;
				record.first.first=j;
				record.second.push_back(j);
			}
			//֮��ÿ�γ��ֶ�����λ�ú���Ϣ,�������һ�α���ľ������һ�����ֵ��λ����
			else if (flag and information_rect_exchange.find(temp) != information_rect_exchange.end())
			{
				record.first.second = j;
				record.second.push_back(j);
			}
			//����ʲôҲ����
			else continue;
		}
		//��ʼ��ÿ������һ���ھ����г��ֵĵ�
		Point temp1 = taxi_use[i].getPoint(record.second[0]);
		//��������ӿ�ʼ�������ھ��γ��ֵĵ��ȫ����Ϣ
		for (int k = record.first.first; k <= record.first.second; k++)
		{
			//���µ�ͼ��
			Point temp2 = taxi_use[i].getPoint(k);
			p.pathgreaphupdate2(temp1, temp2);
		}
	}

	//���ͼ��ȫ����
	unordered_map<string, pathEdge*> maptemp = p.getpathEdgemap();

	//�洢ͼ��ȫ���߼��ߵ�Ƶ����,����������(����ķ����ʽ����)
	vector<pair<int, pathEdge>> sortpathEdge;
	for (auto i : maptemp) {
		sortpathEdge.push_back({ (i.second)->getpathcount(),*(i.second) });
	}
	sort(sortpathEdge.begin(), sortpathEdge.end(), [](const pair<int, pathEdge>& a, const pair<int, pathEdge>& b) { return a.first > b.first; });
	
	//��Ƶ������ߵĿ�ʼ����
	int x = 0;
	for(auto i: sortpathEdge)
	{
		//�ж�������Ƿ���ھ�����,������򽫸ñ߷�����������
		double iLo1 = i.second.getpathNode1()->getLongitudeNode();
		double iLa1 = i.second.getpathNode1()->getLatitudeNode();
		double iLo2 = i.second.getpathNode2()->getLongitudeNode();
		double iLa2 = i.second.getpathNode2()->getLatitudeNode();
		if ((iLo1 >= pos1[0] and iLa1 >= pos1[2] and iLo1 <= pos1[1] and iLa1 <= pos1[3] and iLo2 >= pos2[0] and iLa2 >= pos2[2] and iLo2 <= pos2[1] and iLa2 <= pos2[3]) or (iLo2 >= pos1[0] and iLa2 >= pos1[2] and iLo2 <= pos1[1] and iLa2 <= pos1[3] and iLo1 >= pos2[0] and iLa1 >= pos2[2] and iLo1 <= pos2[1] and iLa1 <= pos2[3]))
		{
			k_for.push_back(i.second);
			result.push_back({ iLo1 ,iLa1,iLo2,iLa2 });
			x++;
			//����k����ֹͣ,����ȫ������һ��
			if (x == k) { break; }
		}
	}

	//���ǰk��·���������Ϣ
	if (output) {
		ofstream ofs("D:\\result\\function_path_frequent_analysis2.txt", ios::trunc);
		ofs << x << '\n';
		for (auto i : k_for)
		{
			ofs << setiosflags(ios::fixed) << setprecision(5)
				<< i.getpathNode1()->getLongitudeNode() << ","
				<< i.getpathNode1()->getLatitudeNode() << ","
				<< i.getpathNode2()->getLongitudeNode() << ","
				<< i.getpathNode2()->getLatitudeNode() << ","
				<< i.getpathcount() << ","
				<< i.getpathlen() << '\n';

		}
		ofs.close();
	}

	//����·���Ľ������
	return result;
}

//��Ϊֻ�������ڵ��·�����
vector<vector<string>> function_path_frequent_analysis2_neighbor(double* pos1, double* pos2, int k, bool output)
{
	//���������Ѿ�����õ�file�ļ�(�洢��pathEdge�Ķ���)��class�ļ�(�洢�����Ϣ)
	ifstream ifs1("D:\\pathdata\\pathsortedge_forcount_file", ios::in | ios::binary);
	ifstream ifs2("D:\\pathdata\\pathsortedge_forcount_class.txt", ios::in);

	//�����������
	vector<pathEdge> result;

	//�Ƚ������������Ϊk��С
	for (int i = 0; i < k; i++)
	{
		pathEdge temp;
		result.push_back(temp);
	}

	int i = 0;

	//��������Ҫ��ı߶�Ӧ�Ľ�㾭γ����Ϣ(Ŀǰ�޷�д���ȡ�����ָ�����)
	vector<vector<string>> vstemp;

	//����ȫ��pathEdge����
	while (ifs1.read((char*)(&result[i]), sizeof(pathEdge)))
	{
		//��õ�ǰ�߶�Ӧ�Ľ�㾭γ�Ⱥ�������Ϣ
		string stemp;
		getline(ifs2, stemp, '\n');

		//�ָ���Щ��Ϣ,�����ж��Ƿ��ھ�����,��Ϣ�洢��������
		vector<string> vvtemp = Split(stemp, ',');

		double iLo1 = stod(vvtemp[0]);
		double iLa1 = stod(vvtemp[1]);
		double iLo2 = stod(vvtemp[2]);
		double iLa2 = stod(vvtemp[3]);
		//�����Ѿ�����Ƶ��������,��������,�ж��䳤���Ƿ�Ҫ��
		if ((iLo1 >= pos1[0] and iLa1 >= pos1[2] and iLo1 <= pos1[1] and iLa1 <= pos1[3] and iLo2 >= pos2[0] and iLa2 >= pos2[2] and iLo2 <= pos2[1] and iLa2 <= pos2[3]) or (iLo2 >= pos1[0] and iLa2 >= pos1[2] and iLo2 <= pos1[1] and iLa2 <= pos1[3] and iLo1 >= pos2[0] and iLa1 >= pos2[2] and iLo1 <= pos2[1] and iLa1 <= pos2[3]))
		{

			//����Ҫ��ʱ��ÿ�н�㾭γ����Ϣ���浽vstemp��
			vstemp.push_back(vvtemp);
			
			//++i��ʾ�Ѿ��洢�������Ϣ������һ��,��ʱresult[i]�Ľ���̶���
			i++;

		}
		//����õ���k������Ҫ���·�����Խ���
		if (i == k)
		{
			break;
		}
	}
	if (output) {
		ofstream ofs("D:\\result\\function_path_frequent_analysis2_neighbor.txt", ios::trunc);
		ofs << i << '\n';
		//������������е���Ч·������
		for (int j = 0; j < i; j++)
		{
			//�����������е���Ч��Ϣ�����汣��Ľ�㾭γ����Ϣ
			ofs << setiosflags(ios::fixed) << setprecision(5)
				<< vstemp[j][0] << ","
				<< vstemp[j][1] << ","
				<< vstemp[j][2] << ","
				<< vstemp[j][3] << ","
				<< result[j].getpathcount() << ","
				<< result[j].getpathlen() << '\n';
		}
		ofs.close();
	}
	return vstemp;
}


//��������7:ͨ��ʱ�����������������������A��B�������ڲ�ͬ��ʱ��Σ����⳵��A��B��ͨ��ʱ����̵�·�����Լ���Ӧ��ͨ��ʱ�䡣
//�������pos1��ʾ����1,pos2��ʾ����2,day hour minute��ʾ��ʼʱ��,time_span��ʾʱ�䷶Χ
//����һϵ�еĵ�, �����ʱ��Ŀ�ʼ�������������֮���ȫ�����ʾһ����ȷ��·��
vector<Point> function_pathuse_shortesttime(double* pos1, double* pos2, int day, int hour, int minute, int time_span, bool output)
{
	//���ú�������3�������Ҫ��ʱ�����������εĳ���������Ϣ
	set<Point, CMP> information_rect_exchange = function_region_association_analysis_one(pos1, pos2, day, hour, minute, time_span,false);

	//�������ʱ���Ӧ�������յ�
	Point a, b;

	//�������ʱ��,��ʼʱΪint�������ֵ
	int shortest_time = INT_MAX;

	//���彻������vector�汾,��Ҫ�����ڱ���
	vector<Point> information_rect_exchange_use = {};
	
	//��������������
	vector<Point> result = {};

	//������������,����Ϣ���浽vector�汾��
	for (const auto& i : information_rect_exchange)
	{
		information_rect_exchange_use.push_back(i);
	}

	//����vector�汾�Ľ����������Ϣ(����һ��������)
	for (auto j = information_rect_exchange_use.begin(); j != information_rect_exchange_use.end();)
	{
		//jΪ��һ����,iΪǰһ����
		auto i = j++;

		//���jû���������ͽ��з���
		if (j != information_rect_exchange_use.end()) {

			//������Ϊͬһ�����⳵�Ĺ켣��
			if (i->gettaxiname() == j->gettaxiname())
			{
				//���������ľ�γ��
				double iLo1 = i->getLongitude();
				double iLa1 = i->getLatitude();
				double iLo2 = j->getLongitude();
				double iLa2 = j->getLatitude();

				//�ж����������������������Ҳ���ͬһ������
				if ((iLo1 >= pos1[0] and iLa1 >= pos1[2] and iLo1 <= pos1[1] and iLa1 <= pos1[3] and iLo2 >= pos2[0] and iLa2 >= pos2[2] and iLo2 <= pos2[1] and iLa2 <= pos2[3]) or (iLo2 >= pos1[0] and iLa2 >= pos1[2] and iLo2 <= pos1[1] and iLa2 <= pos1[3] and iLo1 >= pos2[0] and iLa1 >= pos2[2] and iLo1 <= pos2[1] and iLa1 <= pos2[3]))
				{
					//�ж������ʱ���
					int temptime = gettime(i->getday(), i->gethour(), i->getminute(), i->getsecond(), j->getday(), j->gethour(), j->getminute(), j->getsecond());

					//�������ʱ��Ͷ�Ӧ��
					if (shortest_time > temptime) {
						shortest_time = temptime;
						a = *i;
						b = *j;
					}
				}
			}
		}
	}

	//����һ�����⳵�����л�,��Ӧ�������·���Ĺ켣����⳵�����
	taxi t = boost_iarchive_taxi2(a.gettaxiname());

	//�ñ��Ϊ0
	int flag = 0;

	//�������⳵�켣��
	for (int i = 0; i < t.getsize(); i++) 
	{
		//���û�з�������һֱѰ�����
		if (!flag) 
		{
			//�ҵ����ʱ
			if (a == t.getPoint(i))
			{
				//�����1
				flag = 1;

				//������������
				result.push_back(a);
			}
		}

		//����ҵ���������ҵ����յ�
		else if (flag and b == t.getPoint(i))
		{
			//�յ����������
			result.push_back(b);

			//ֹͣ����
			break;
		}

		//���ֻ�ҵ�����㲻���յ�
		else 
		{
			//����Щ�м��Ҳ����������,�γ�һ������·��
			result.push_back(t.getPoint(i));
		}
	}
	//������ʱ����Ϣ
	if (output) {
		ofstream ofs("D:\\result\\function_pathuse_shortesttime.txt", ios::trunc);
		ofs  << shortest_time << '\n';
		for (auto i : result)
		{
			ofs << setiosflags(ios::fixed) << setprecision(5) << i.gettaxiname() << "," << i.getLongitude() << "," << i.getLatitude() << "," << i.getday() << "," << i.gethour() << "," << i.getminute() << "," << i.getsecond() << '\n';
		}
		ofs.close();
	}
	//���ؽ������
	return result;
}