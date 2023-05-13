#include <iostream>
#include <cmath>
#include <fstream>
#include <unordered_map>
#include "taxi_header_use.h"

//pathNode

//���캯��,�����Ķ�Ӧ��γ�ȳ�ʼ��(��ȷ��0.001��)
pathNode::pathNode(double d1, double d2) :LongitudeNode(d1), LatitudeNode(d2) {}

//���ؽ�㾭��
double pathNode::getLongitudeNode()
{
	return LongitudeNode;
}

//���ؽ��γ��
double pathNode::getLatitudeNode()
{
	return LatitudeNode;
}


//pathEdge

//�޲ι��캯��,���ڷ����л�ʹ��
pathEdge::pathEdge() :len(0), shortesttime(0), p1(NULL), p2(NULL), count(0) {}


//���캯��,���ݴ���������켣��������ڵĽ����й���(�˴�����ֵ�Ѿ�����Ԥ����,�жϳ�����ͬһ�����߹켣��)
pathEdge::pathEdge(Point a, Point b, pathNode* po1, pathNode* po2) :count(1), p1(po1), p2(po2)
{
	//floor����ȡ��,����ȷ�ȸ��ߵĵ��ʾ�����ڸ�0.001��ȷ����
	double LongitudeNode1 = floor(a.getLongitude() * 1000) / 1000;
	double LongitudeNode2 = floor(b.getLongitude() * 1000) / 1000;
	double LatitudeNode1 = floor(a.getLatitude() * 1000) / 1000;
	double LatitudeNode2 = floor(b.getLatitude() * 1000) / 1000;

	//���㴫�����㾭��ʱ��,��ֻ����һ�Ե�����Ĭ����Ϊ���ʱ��
	shortesttime = gettime(a.getday(), a.gethour(), a.getminute(), a.getsecond(), b.getday(), b.gethour(), b.getminute(), b.getsecond());

	//����������������
	len = 111194.9266 * sqrt(pow(LongitudeNode2 - LongitudeNode1, 2) + pow((LatitudeNode2 - LatitudeNode1) * lodividela, 2));
	
	//���洫��������Ϊ���ʱ��������յ�
	shortesttime_taxistart = a;
	shortesttime_taxiend = b;
}

//�߸��º���,���ݴ���������켣����о��������ĸ��º����ʱ��ĸ��º����ʱ�����ý��ĸ���
void pathEdge::pathEdgeupdate(Point a, Point b)
{
	//���㴫������ľ���ʱ��
	int time = gettime(a.getday(), a.gethour(), a.getminute(), a.getsecond(), b.getday(), b.gethour(), b.getminute(), b.getsecond());
	
	//�߾�������+1
	count++;

	//�Ƚϴ����ʱ���ԭ�е����ʱ��,���ǰ�߸��̾͸������ʱ��Ͷ�Ӧ��㡢�յ�
	if (shortesttime > time)
	{
		shortesttime = time;
		shortesttime_taxistart = a;
		shortesttime_taxiend = b;
	}
	return;
}

//���رߵ�һ�����
pathNode* pathEdge::getpathNode1()
{
	return p1;
}

//���رߵ���һ�����
pathNode* pathEdge::getpathNode2()
{
	return p2;
}

//���ر߱������Ĵ���
int pathEdge::getpathcount()
{
	return count;
}

//���ؾ����ñߵ����ʱ��
int pathEdge::getpathshortesttime()
{
	return shortesttime;
}

//���ظñߵĳ���
double pathEdge::getpathlen()
{
	return len;
}

//����ñߵ�ȫ����Ϣ
void pathEdge::printpath_information()
{
	cout << "path:" << endl;
	cout << "Node1Lo:" << p1->getLongitudeNode() << endl;
	cout << "Node1La" << p1->getLatitudeNode() << endl;
	cout << "Node2Lo:" << p2->getLongitudeNode() << endl;
	cout << "Node2La:" << p2->getLatitudeNode() << endl;
	cout << "path count:" << count << endl;
	cout << "path len:" << len << endl;
	cout << "path shortesttime:" << this->getpathshortesttime() << endl << endl;
	return;
}

//��������5ר�õ�����ñߵ���Ϣ
void pathEdge::printpath_information2(double d1,double d2,double d3,double d4)
{
	cout << "path:" << endl;
	cout << "Node1Lo:" << d1 << endl;
	cout << "Node1La" << d2 << endl;
	cout << "Node2Lo:" << d3 << endl;
	cout << "Node2La:" << d4 << endl;
	cout << "path count:" << count << endl;
	cout << "path len:" << len << endl << endl;
}


//pathgraph

//�޲ι��캯��,�������κβ���
pathgraph::pathgraph() {}

//��������,������map��Ԫ��������ڳ��ڴ������������
pathgraph::~pathgraph()
{
	unordered_map<string, pathNode*>temp1;
	pathNodemap.swap(temp1);
	unordered_map<string, pathEdge*>temp2;
	pathEdgemap.swap(temp2);
}

//���º���,���ݴ�����⳵���ж�ͼ�ĸ���
void pathgraph::pathgraphupdate(taxi& t)
{
	//���ݳ��⳵����ʱ��켣���������
	for (int i = 0; i < t.getsize() - 1; i++)
	{
		//�����������
		Point a = t.getPoint(i);
		Point b = t.getPoint(i + 1);

		//���������������Ӧ�����ڵĽ��
		double LongitudeNode1 = floor(a.getLongitude() * 1000) / 1000;
		double LongitudeNode2 = floor(b.getLongitude() * 1000) / 1000;
		double LatitudeNode1 = floor(a.getLatitude() * 1000) / 1000;
		double LatitudeNode2 = floor(b.getLatitude() * 1000) / 1000;
		
		//�ж��Ƿ���ͬһ�����,����ھͼ����ж���һ��켣��
		if (LongitudeNode1 == LongitudeNode2 and LatitudeNode1 == LatitudeNode2)
		{
			continue;
		}

		//����Ϊ�������켣�����ӽ��
		pathNode* p1;
		pathNode* p2;
		
		//�����������
		string flagNode1 = to_string(LongitudeNode1) + "-" + to_string(LatitudeNode1);
		string flagNode2 = to_string(LongitudeNode2) + "-" + to_string(LatitudeNode2);
		
		//��ѯ�����Ƿ���ڶ�Ӧ���,�������ָ�����н��,�粻�����򴴽��½�㲢���뵽������
		auto j1 = pathNodemap.find(flagNode1);
		auto j2 = pathNodemap.find(flagNode2);
		if (j1 == pathNodemap.end())
		{
			p1 = new pathNode(LongitudeNode1, LatitudeNode1);
			pathNodemap.insert({ flagNode1 ,p1 });
		}
		else
		{
			p1 = (*j1).second;
		}
		if (j2 == pathNodemap.end())
		{
			p2 = new pathNode(LongitudeNode2, LatitudeNode2);
		}
		else
		{
			p2 = (*j2).second;
			pathNodemap.insert({ flagNode2 ,p2 });
		}

		//����������,��ѯ�ߵ���Ϣ���Ƿ��Ѵ��������ı�(�������,��������Ҫ��),�������ֱ�ӽ������켣��ͽ�㴫����ڵı߽�����Ϣ����,���������½�һ���߲��뵽�ߵ���Ϣ����,���ô���Ľ��͹켣����г�ʼ��
		string flagpath1 = to_string(LongitudeNode1) + "-" + to_string(LatitudeNode1) + "-" + to_string(LongitudeNode2) + "-" + to_string(LatitudeNode2);
		string flagpath2 = to_string(LongitudeNode2) + "-" + to_string(LatitudeNode2) + "-" + to_string(LongitudeNode1) + "-" + to_string(LatitudeNode1);
		auto j3 = pathEdgemap.find(flagpath1);
		if (j3 == pathEdgemap.end())
		{
			auto k = pathEdgemap.find(flagpath2);
			if (k == pathEdgemap.end()) {
				pathEdge* p = new pathEdge(a, b, p1, p2);
				pathEdgemap.insert({ flagpath1,p });
			}
			else
			{
				k->second->pathEdgeupdate(a, b);
			}
		}
		else
		{
			j3->second->pathEdgeupdate(a, b);
		}
	}
	return;
}
//��������6�ĸ�������,���ݴ���ĵ���Ϣ���ж�ͼ��Ϣ�ĸ���
void pathgraph::pathgreaphupdate2(Point a, Point b)
{
	//���������������Ӧ�����ڵĽ��
	double LongitudeNode1 = floor(a.getLongitude() * 1000) / 1000;
	double LongitudeNode2 = floor(b.getLongitude() * 1000) / 1000;
	double LatitudeNode1 = floor(a.getLatitude() * 1000) / 1000;
	double LatitudeNode2 = floor(b.getLatitude() * 1000) / 1000;

	//�ж��Ƿ���ͬһ�����,����ھͽ�������
	if (LongitudeNode1 == LongitudeNode2 and LatitudeNode1 == LatitudeNode2)
	{
		return;
	}

	//����Ϊ�������켣�����ӽ��
	pathNode* p1;
	pathNode* p2;

	//�����������
	string flagNode1 = to_string(LongitudeNode1) + "-" + to_string(LatitudeNode1);
	string flagNode2 = to_string(LongitudeNode2) + "-" + to_string(LatitudeNode2);

	//��ѯ�����Ƿ���ڶ�Ӧ���,�������ָ�����н��,�粻�����򴴽��½�㲢���뵽������
	auto j1 = pathNodemap.find(flagNode1);
	auto j2 = pathNodemap.find(flagNode2);
	if (j1 == pathNodemap.end())
	{
		p1 = new pathNode(LongitudeNode1, LatitudeNode1);
		pathNodemap.insert({ flagNode1 ,p1 });
	}
	else
	{
		p1 = (*j1).second;
	}
	if (j2 == pathNodemap.end())
	{
		p2 = new pathNode(LongitudeNode2, LatitudeNode2);
	}
	else
	{
		p2 = (*j2).second;
		pathNodemap.insert({ flagNode2 ,p2 });
	}

	//����������,��ѯ�ߵ���Ϣ���Ƿ��Ѵ��������ı�(�������,��������Ҫ��),�������ֱ�ӽ������켣��ͽ�㴫����ڵı߽�����Ϣ����,���������½�һ���߲��뵽�ߵ���Ϣ����,���ô���Ľ��͹켣����г�ʼ��
	string flagpath1 = to_string(LongitudeNode1) + "-" + to_string(LatitudeNode1) + "-" + to_string(LongitudeNode2) + "-" + to_string(LatitudeNode2);
	string flagpath2 = to_string(LongitudeNode2) + "-" + to_string(LatitudeNode2) + "-" + to_string(LongitudeNode1) + "-" + to_string(LatitudeNode1);
	auto j3 = pathEdgemap.find(flagpath1);
	if (j3 == pathEdgemap.end())
	{
		auto k = pathEdgemap.find(flagpath2);
		if (k == pathEdgemap.end()) {
			pathEdge* p = new pathEdge(a, b, p1, p2);
			pathEdgemap.insert({ flagpath1,p });
		}
		else
		{
			k->second->pathEdgeupdate(a, b);
		}
	}
	else
	{
		j3->second->pathEdgeupdate(a, b);
	}
	return;
}

//�������Ϣ����
unordered_map<string, pathNode*> pathgraph::getpathNodemap() { return pathNodemap; }

//���ߵ���Ϣ����
unordered_map<string, pathEdge*> pathgraph::getpathEdgemap() { return pathEdgemap; }

//���ͼ�Ļ�����Ϣ��һЩ�ߵĻ�����Ϣ
void pathgraph::printgetpathNodemap() 
{
	cout << pathNodemap.size() << endl;
	cout << pathEdgemap.size() << endl;
	for (auto i : pathEdgemap) 
	{
		if (i.second->getpathcount() >= 10)
		{
			cout << i.first << ": " << i.second->getpathcount() << " " << i.second->getpathlen() << " " << i.second->getpathshortesttime() << endl;
		}
	}
	return;
}


void pathgraph::function_path_frequent_analysis_init()
{
	//��ͼ�е�ȫ������Ϣ����vector������
	vector<pair<string, pathEdge*>> sortedge = {};
	for (auto i : pathEdgemap) {
		sortedge.push_back(i);
	}

	//��vector�����ڵı߰���Ƶ���Ƚ�������,������ͨ��lambda���ʽ���в�����׽�ͷ���
	sort(sortedge.begin(), sortedge.end(), [](pair<string, pathEdge*>& a, pair<string, pathEdge*>& b) {return a.second->getpathcount()> b.second->getpathcount(); });

	//�����������ļ�����,������Ϣд��
	ofstream ofs1("E:\\pathdata\\pathsortedge_forcount_class.txt", ios::trunc);
	
	for (auto i: sortedge)
	{
		//ofs1����д���㾭γ�Ⱥͱߵ�Ƶ���ȡ����Ⱥ����ʱ��,��������
		ofs1 << (i.second->getpathNode1())->getLongitudeNode() << "," << (i.second->getpathNode1())->getLatitudeNode() << "," << (i.second->getpathNode2())->getLongitudeNode() << "," << (i.second->getpathNode2())->getLatitudeNode() << "," << i.second->getpathcount() << "," << i.second->getpathlen() << "," << i.second->getpathshortesttime() << "\n";
		
		
		
	}

	//�ȹر�ofs1,�ٽ���ofs2,������ֻ�������ͻ
	ofs1.close();

	//ofs2����д��߶���������Ϣ
	ofstream ofs2("E:\\pathdata\\pathsortedge_forcount_file", ios::out | ios::binary);
	for (auto i : sortedge) 
	{
		ofs2.write((char*)(i.second), sizeof(pathEdge));
	}
	//�ر�ofs2
	ofs2.close();
	return;
}