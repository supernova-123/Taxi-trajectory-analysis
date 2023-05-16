#ifndef TAXI_HEADER_H
#define TAXI_HEADER_H

//�������·����������ʹ�õ�ͷ�ļ�
#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <unordered_map>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/string.hpp> 
using namespace std;

//�������ֱ�����
extern const double lodividela;    //����γ�Ⱥ;��ȵı�ֵ,���ڼ���ʵ�ʾ���ʱ��Ϊ��������ʹ��
extern const int taxisize;         //��Ҫ���ǵĳ��⳵����,�����ݼ���Ӧ10357�����⳵
extern const double inBeijing_Longitude_min; //������Χ����С����;
extern const double inBeijing_Longitude_max; //������Χ����󾭶�;
extern const double inBeijing_Latitude_min;  //������Χ����Сγ��;
extern const double inBeijing_Latitude_max;  //������Χ�����γ��;
extern const int changetime;       //�Թ��ܺ�������r*r��С����ʱ����������ӡ���
								   //���������ɵĳ��⳵��Ӧ��ʱ����

extern const double r_for_distance; //r�Ļ�����λ����
/*�Ը��ֱ������Ƶ�˵��:
*   Point:
	taxiname/i:���⳵�������
	times/s1:ʱ������,�ֱ𱣴����ա�ʱ���ֺ���,s1���ַ�����ʽ,times��int��������
	Longitude/s2:����,ǰ��double,����string
	Latitude/s3:γ��,ǰ��double,����string

*   taxi:
	name:���⳵�������
	size:�洢��Point������
	pv:ָ��,ָ����ЧPoint����
	vs:һ�����⳵��ȫ������Ϣ����Ч�ֽ��Ľ��
	i/pos:���⳵�������е�λ��

*   CMP����˵��

*	time_distribution:
	timemap����ϣ��,�洢��-ֵ��,����ϢΪ���⳵�������,ֵ��ϢΪ��ʱ����ȫ������Ϣ
	vt:���ڳ�ʼ���ĳ��⳵
	i1��i2��i3:�ֱ��Ӧ�ö�����Ҫ�洢���ա�ʱ���֡���
	n:����ѯ�ĳ��⳵�����
	day��hour��minute:����ѯ��ʱ��ֵ
	p:double����,��СΪ4,��һ��Ϊ��ѯ��Χ�����ϽǾ��ȡ��ڶ���Ϊ��ѯ��Χ�����½Ǿ��ȡ�������Ϊ��ѯ��Χ�����Ͻ�γ�ȡ����ĸ�Ϊ��ѯ��Χ�����½�γ��
	countnum:ͳ�ƶ��������г��ֳ��⳵��������Ϣ
	taxi_haveuse:bool [taxisize]����,�ڶ�Ӧ��λ�ñ�ע�˳��⳵�Ƿ�ͳ�ƹ�

*	position_distribution_inBeiJing:
	posmap����ϣ��,�洢��-ֵ��,����ϢΪ���⳵�������,ֵ��ϢΪ�ÿռ䷶Χ��ȫ������Ϣ
	init_Longitude:�������ͳ�ƵĿռ���ʼ����
	init_Latitude:�������ͳ�ƵĿռ���ʼγ��
	vt:���ڳ�ʼ���ĳ��⳵
	d1,d2:����ʼ���Ŀռ䷶Χ����ʼ���Ⱥ�γ��
	n:����ѯ�ĳ��⳵�����
	day��hour��minute:����ѯ��ʱ�䷶Χ����ʼֵ
	p:double����,��СΪ4,��һ��Ϊ��ѯ��Χ�����ϽǾ��ȡ��ڶ���Ϊ��ѯ��Χ�����½Ǿ��ȡ�������Ϊ��ѯ��Χ�����Ͻ�γ�ȡ����ĸ�Ϊ��ѯ��Χ�����½�γ��
	countnum:ͳ�ƶ��������г��ֳ��⳵��������Ϣ
	time_span:����Ҫɨ������ʱ�䷶Χ(��ʹ���������ʼֵ��ӳ�������ʱ��Ҳ����,�Ѿ�����������)
	taxi_haveuse:bool [taxisize]����,�ڶ�Ӧ��λ�ñ�ע�˳��⳵�Ƿ�ͳ�ƹ�

*	position_distribution_outBeiJing:
	vt:���ڳ�ʼ���ĳ��⳵
	n:����ѯ�ĳ��⳵�����
*/

//���⳵�켣����,����ʱ����Ϣ�ͳ���
class Point 
{
	//����ʽ���л��ͷ����л�,�������л����������Ԫ,���������Ȩ�޽������л�
	friend class boost::serialization::access;

private:
	
	//Point��Ա�������⳵�������,���ֵĹ켣����ա�ʱ���֡���,�����Ⱥ�γ��
	int taxiname;
	int times[4];
	double Longitude;
	double Latitude;

	//�������л��ͷ����л���ִ�к���
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& taxiname;
		ar& times;
		ar& Longitude;
		ar& Latitude;
	}
public:

	//Point���޲ι��캯��:
	Point();
	
	//Point���вι��캯��:
	Point(int i1, int i2, int i3, int i4, int i5, double d1, double d2);
	
	//Point�ĳ�ʼ������,��ֻ�������л�ʱ�����ݼ���������Ĵ���,�Ժ�ֱ�ӷ����л�����,��������:
	void Point_init(int i, string& s1, string& s2, string& s3);
	//��һ����ʼ������,�������ļ���д���е���Ϣʱ,ʹ��Ƶ��
	void Point_init2(int i1, int i2, int i3, int i4, int i5, double d1, double d2);

	//��ó��⳵�켣�����������
	int gettaxiname();

	//��ó��⳵�켣�㾭�Ⱥ���
	double getLongitude();
	
	//��ó��⳵�켣��γ�Ⱥ���
	double getLatitude();
	
	//��ó��⳵�켣������ڡ�Сʱ�����ӡ�����Ϣ
	int getday();
	int gethour();
	int getminute();
	int getsecond();

	//���ַ�����ʽ,���س��⳵�켣��ʱ����Ϣ,�������ʱ������û����
	string gettimes();

	//���⳵�켣��ȫ��Ϣ���:
	void printPoint();

	//���⳵�켣��ıȽ�,�����ж��Ƿ���ͬһ����
	bool operator== (const Point& p) const;
};


//���⳵��,�Ե�Ϊ��λ��ϳ�Ϊһ�����⳵
class taxi
{
	friend class boost::serialization::access;

private:
	
	//���⳵��Ա�������⳵�����,���⳵�ڵ���Ч��������;���ĵ���Ϣ
	int name;
	int size;
	Point* pv;
	
	//���л���������, ע��taxi�е�pv�Ƕ�̬����, ���л��ͷ����л�Ҳ��Ҫ��̬����, ���������
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& name;
		ar& size;
		if (pv == 0)
		{
			pv = new Point[size];
		}
		for (int i = 0; i < size; ++i) {
			ar& pv[i];
		}
	}

public:
	
	//taxi���޲ι��캯��
	taxi();

	//taxi�Ŀ������캯��
	taxi(const taxi& t);

	//taxi��operator=�����
	taxi operator=(const taxi& t);

	//taxi������
	~taxi();

	//taxi��ʼ������,��һ�������л����ļ�д��ʹ��,�ڶ������ļ���ȡʹ��(�����л�����Ҫ����)
	void taxi_init(vector<vector<string>>& vs);
	void taxi_init2(vector<vector<string>>& vs);

	//taxi�ĸ�����Ϣ���غ���,����Ϊ������,�������,ĳ��ľ���,ĳ���γ��,ĳ�������
	int getsize();
	int getname();
	double getLongitude2(int i);
	double getLatitude2(int i);
	Point& getPoint(int pos);

	//taxi��ȫ����Ϣ���
	void printtaxi_track();

};


//����ıȽϺ���,���ڶԳ��⳵�켣������
class CMP {
public:
	bool operator() (Point a, Point b)const;
};


//ʱ��ֲ���,�Է���Ϊ��λ�����г��⳵��Ϣ��ʱ����
class time_distribution
{
	friend class boost::serialization::access;

private:
	
	//��һ�������ϣ����ж�ĳ��ʱ�̵ĵ���Ϣ�洢
	//��ֵΪ���⳵�����,ֵΪ�ó��⳵��Ӧʱ���ȫ������Ϣ
	unordered_map<int, vector<Point>> timemap;
	
	//���л�+�����л�
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& timemap;
	}

public:
	
	//�޲ι��캯��
	time_distribution();
	
	//����:
	~time_distribution();

	//ʱ��ֲ���ĳ�ʼ������,�򲻶ϴ�����⳵������Ϣ,ȡ��Ϊupdate
	void time_distribution_update(taxi& vt, int i1, int i2, int i3);
	
	//ʱ��ֲ�����ļ���ȡ��ʼ������
	void time_distribution_init(vector<vector<string>>& vs);
	
	//���ʱ��ֲ����ʱ�̵�ȫ����Ϣ:
	void print_time_distribution();
	
	//���ĳ�����⳵�ڸ�ʱ�̵�ȫ����Ϣ
	pair<int, vector<Point>>& get_onetime_onetaxi(int n);

	//���ظ�ʱ����ڵ�ȫ�����⳵��ȫ����Ϣ
	unordered_map<int, vector<Point>> getallpoint();

	//��ʱ��Ϊ��������,��ѯ��ĳ����Χ�ڵĳ��⳵�켣����Ϣ,countnum��taxi_haveuse���ڱ�����ֵ�����������Щ���⳵����
	set<Point, CMP> counttaxi(int day, int hour, int minute, double* p, int& countnum, bool* taxi_haveuse);
};


//�����Ŀռ�ֲ���,��0.01*0.01��γ�ȱ仯��Ϊһ����
class position_distribution_inBeiJing
{

	friend class boost::serialization::access;

private:

	//��һ�������ϣ����ж�ĳ���ռ䷶Χ�ĵ���Ϣ�洢
	//��ֵΪ���⳵�����,ֵΪ�ó��⳵��Ӧ�ռ䷶Χ��ȫ������Ϣ
	unordered_map<int, vector<Point>> posmap;

	//�ÿռ䷶Χ����ʼ���Ⱥ�γ��
	double init_Longitude;
	double init_Latitude;

	//���л�+�����л�
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& posmap;
		ar& init_Longitude;
		ar& init_Latitude;
	}

public:

	//�޲ι��캯��
	position_distribution_inBeiJing();

	//��������
	~position_distribution_inBeiJing();

	//�ռ�ֲ���ĳ�ʼ������,�򲻶ϴ�����⳵������Ϣ,ȡ��Ϊupdate
	void position_distribution_inBeiJing_update(taxi& vt, double d1, double d2);

	//�ռ�ֲ������һ����ʼ������,�򲻶ϴ����ĸ�����Ϣ,ȡ��Ϊupdate2
	void position_distribution_inBeiJing_update2(Point& t, double d1, double d2);

	//�ռ�ֲ�����ļ���ȡ��ʼ������
	void position_distribution_inBeiJing_init(vector<vector<string>>& vs);

	//����ռ�ֲ���ÿռ䷶Χ�ڵ�ȫ����Ϣ:
	void print_position_distribution_inBeiJing();

	//��ö�Ӧ����
	double getinit_Longitude();

	//��ö�Ӧγ��
	double getinit_Latitude();

	//���ظÿռ��ڵ�ȫ�����⳵��ȫ����Ϣ
	unordered_map<int, vector<Point>> getallpoint();

	//���ĳ�����⳵�ڸÿռ䷶Χ�ڵ�ȫ����Ϣ
	pair<int, vector<Point>>& get_onetime_onetaxi(int n);

	//�Կռ�Ϊ��������,��ѯ��ĳ����Χ��,ĳ��ʱ���ڵĳ��⳵�켣����Ϣ,countnum��taxi_haveuse���ڱ�����ֵ�����������Щ���⳵����
	set<Point, CMP> counttaxi(int day, int hour, int minute, double* p, int& countnum, int time_span, bool* taxi_haveuse);

};


//������Ŀռ�ֲ���,������Ϣ��,��Ϊһ������ʹ��,�Ƕ���һ����Ĳ���
class position_distribution_outBeiJing
{

	friend class boost::serialization::access;

private:

	//ֻ��һ�������ϣ����ж�ĳ���ռ䷶Χ�ĵ���Ϣ�洢
	//��ֵΪ���⳵�����,ֵΪ�ó��⳵��Ӧ�ռ䷶Χ��ȫ������Ϣ
	unordered_map<int, vector<Point>> posmap;

	//���л�+�����л�
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& posmap;
	}
	
public:

	//�޲ι��캯��
	position_distribution_outBeiJing();

	//��������
	~position_distribution_outBeiJing();
	
	//�ռ�ֲ���ĳ�ʼ������,�򲻶ϴ�����⳵������Ϣ,ȡ��Ϊupdate
	void position_distribution_outBeiJing_update(taxi& vt);

	//�ռ�ֲ�����ļ���ȡ��ʼ������
	void position_distribution_outBeiJing_init(vector<vector<string>>& vs);

	//����ռ�ֲ���ÿռ䷶Χ�ڵ�ȫ����Ϣ:
	void print_position_distribution_outBeiJing();

	//��øÿռ䷶Χ��ȫ�����⳵��ȫ������Ϣ:
	unordered_map<int, vector<Point>> getallpoint();
	
	//���ĳ�����⳵�ڸÿռ䷶Χ�ڵ�ȫ����Ϣ
	pair<int, vector<Point>>& get_onetime_onetaxi(int n);

};


//·�������,���ڴ洢��Ӧ·���Ĵ��±����ֵ��ռ�λ����Ϣ
class pathNode
{

	friend class boost::serialization::access;

private:
	
	//���洢·���Ķ�Ӧ��ž�γ��(��ȷ��0.001��)
	double LongitudeNode;
	double LatitudeNode;
	
	//���л�+�����л�
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& LongitudeNode;
		ar& LatitudeNode;
	}
public:
	
	//��㹹�캯��,�洢�����ڵľ�γ��
	pathNode(double d1 = 0, double d2 = 0);

	//��ý�㾭��
	double getLongitudeNode();

	//��ý��γ��
	double getLatitudeNode();

};


//·������,���ڴ洢����·���Ŀռ���Ϣ
class pathEdge
{

	friend class boost::serialization::access;

private:

	//p1��p2�ֱ�Ϊ�����õ��������(����)
	pathNode* p1;
	pathNode* p2;

	//�ñ߶�ξ���ʱ�����ѵ����ʱ��
	int shortesttime;

	//�ñߵ�Ȩֵ,��Ϊ�ñ߱������Ĵ���,��Ӧ��Ƶ����
	int count = 0;

	//�ñߵĵڶ�Ȩֵ,��Ϊ�ñ߶�Ӧ��������
	double len;
	
	//������ʱ��ʱ���õ���ʼ����յ�(����)
	Point shortesttime_taxistart;
	Point shortesttime_taxiend;

	//���л�+�����л�
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& p1;
		ar& p2;
		ar& len;
		ar& shortesttime;
		ar& count;
		ar& shortesttime_taxistart;
		ar& shortesttime_taxiend;
	}

public:

	//�޲ι��캯��,���ڷ����л�ʹ��
	pathEdge();

	//���캯��,���ݴ���������켣��������ڵĽ����й���(�˴�����ֵ�Ѿ��жϲ�����ͬһ�����߹켣��)
	pathEdge(Point a, Point b, pathNode* po1, pathNode* po2);

	//���º���,���ݴ���������켣����о��������ĸ��º����ʱ��ĸ��º����ʱ�����ý��ĸ���
	void pathEdgeupdate(Point a, Point b);

	//��ʼ������,�����ļ��Ķ�ȡ��Ϣ���г�ʼ��
	void pathEdge_init(double d1,double d2,double d3,double d4,int i5,double d6,int i7,int  i8,double i9,double i10,int i11,int i12,int i13,int i14,int i15,double i16,double i17,int i18,int i19,int i20,int i21);


	//��ñ����õ���������е�һ��
	pathNode* getpathNode1();
	pathNode* getpathNode2();

	//��ÿ�ʼ���ʱ���ͽ������ʱ���
	Point getshortesttime_taxistart();
	Point getshortesttime_taxiend();

	//��þ����ñߵĴ���
	int getpathcount();

	//��þ����ñߵ����ʱ��
	int getpathshortesttime();

	//��øñߵ�������
	double getpathlen();

	//����ñߵ���Ϣ
	void printpath_information();

	//��������5ר�õ�����ñߵ���Ϣ
	void printpath_information2(double d1,double d2,double d3,double d4);
};


//·��ͼ��,���ڴ洢���нڵ�ͱ�,����һ��ͼ
class pathgraph
{

	friend class boost::serialization::access;

private:

	//��һ�������ϣ�����ȫ�����⳵�Ľ����Ϣ�洢
	//��ֵΪ�ɽ��λ�����ɵ��ַ���(��������ַ���Ψһ),ֵΪ�ý���γɵ�����Ϣ
	unordered_map<string, pathNode*> pathNodemap;

	//����һ�������ϣ�����ȫ�����⳵�ı���Ϣ�洢
	//��ֵΪ�ɱ����ɵ��ַ���(����������ַ���Ψһ),ֵΪ�ñ��γɵ�����Ϣ
	unordered_map<string, pathEdge*> pathEdgemap;

	//���л�+�����л�
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& pathNodemap;
		ar& pathEdgemap;
	}

public:

	//ͼ���޲ι��캯��
	pathgraph();

	//��������
	~pathgraph();

	//���ݴ���ĳ��⳵���ж�ͼ��Ϣ�ĸ���
	void pathgraphupdate(taxi& t);

	//ͼ���ļ���ȡ��ʼ������
	void pathgraph_init(vector<vector<string>>& vs);
	
	//��������6�ĸ�������,���ݴ���ĵ���Ϣ���ж�ͼ��Ϣ�ĸ���
	void pathgreaphupdate2(Point p1, Point p2);

	//����ȫ�������Ϣ
	unordered_map<string, pathNode*> getpathNodemap();
	
	//����ȫ������Ϣ
	unordered_map<string, pathEdge*> getpathEdgemap();

	void printgetpathNodemap();

	//��ͼ�������л�
	void pathserialization();

	//�ļ��汾
	void pathserialization2();

	//��������5�ĸ������� 
	void function_path_frequent_analysis_init();
};   


//r������,����ĳ����r*r�ĸ��ӵĸ���ʱ�����Ϣ�洢��һ��,r�Ļ�����λ��0.001��γ��
class position_distribution_r
{

private:
	
	//r��ָ����С,��r=10��ÿ���Ӵ�СΪ0.01*0.01
	static int r;

	//��ʱ��γ���Ҫ������ʱ��仯��λ
	static int vsize;

	//���������Ӹ�ʱ�γ��ֵĳ��⳵������������Ӧ��һ��������,������Ϊȫ����ʱ�θ��������ļ���,��ÿ��������size��С��Ӧ�˵�������ÿʱ�γ��ֵĳ��⳵����,����ͳ�Ƴ����ܶ�
	vector<vector<int>> taxijudge;
	
	//��������߼�����,���Ǵ洢���ǵ�������ÿʱ�γ��ֵ�ȫ�����⳵�켣����Ϣ,���ڷ����������
	vector<set<Point, CMP>> information_regional_density;

public:

	//���캯��,�����޲κ���
	position_distribution_r(int i, int j);
	
	//��������
	~position_distribution_r();

	//��������ʱ�γ��⳵���ִ���֮��
	int getallcount();

	//���ظ����ڹ��ж��ٸ�ʱ��
	int getvsize();

	//���ظ�ʱ����ĳ��ʱ�θ�����ĳ��⳵���������
	vector<int> gettaxijudge(int i);

	//���ظ�ʱ����ĳ��ʱ�θ�����ĳ��⳵����
	int getcountsize(int i);
	
	//���ظ�ʱ����ĳ��ʱ�θ������ȫ�����⳵��Ϣ
	set<Point, CMP> getallPoint(int i);

	//������Ӧʱ���ڲ������Ҫ��Ĺ켣��
	void insert(Point p, int i);

};


//�����ֻ���Ǳ�����,���ཫ��������Ϣ
//ͳ�ϵ�һ����Ϊ���忼��
class position_distribution_outr
{

private:

	//��ʱ��γ���Ҫ������ʱ��仯��λ
	int vsize;
	
	//�жϳ��⳵�Ƿ�ͳ�ƹ�
	bool taxi_haveuse[10357] = { 0 };
	
	//�洢���Ǳ�����ÿʱ�γ��ֵĳ��⳵����(������ֻͳ��һ��),����ͳ�Ƴ����ܶ�
	vector<int> count;

	//��������߼�����,���Ǵ洢���Ǳ�����ÿʱ�γ��ֵ�ȫ�����⳵�켣����Ϣ,���ڷ����������
	vector<set<Point, CMP>> information_regional_density;

public:

	//���캯��,����Ҫ������ʱ����
	position_distribution_outr(int i);

	//��������
	~position_distribution_outr();

	//��������ʱ�γ��⳵���ִ���֮��
	int getallvcount();

	//���ظ����ڹ��ж��ٸ�ʱ��
	int getvsize();

	//���ظ�ʱ����ĳ��ʱ�α�����ĳ��⳵����
	int getcountsize(int i);

	//���ظ�ʱ����ĳ��ʱ�α������ȫ�����⳵��Ϣ
	set<Point, CMP> getallPoint(int i);
	
	//������Ӧʱ���ڲ������Ҫ��Ĺ켣��
	void insert(Point p, int i);

};     


//��������ʱ���ĺ���,����ʱ�������Ϊ��λ��
int gettime(int day1, int hour1, int minute1, int second1, int day2, int hour2, int minute2, int second2);



/*��������1:����Χ���ҡ�ͳ����ĳ���ض���ʱ��Σ��û�ָ���ľ��������ڵĳ��⳵����Ŀ������ľ����������ͨ���������ε����ϽǺ����½ǵľ�γ��������ȷ����
�������:
	sΪ�������ڷ���Ҫ������е���Ϣ,�Ȱ��ճ��⳵���������,�ٰ��ճ��⳵�켣���ʱ������
	pos����Ϊ����ľ�������,pos[0]��Ӧ���ϽǾ�����Ϣ,pos[1]��Ӧ���½Ǿ�����Ϣ,
	                      pos[2]��Ӧ���Ͻ�γ����Ϣ,pos[3]��Ӧ���½�γ����Ϣ
	day,hour,minute��Ӧ�˲��ҵ���ʼʱ��
	time_spac��Ӧ�˲��ҵ�ʱ�䷶Χ,ע��time_span��δָ���������Ĭ��Ϊ10,��λΪ����,Ҳ���Ƿ����ض�ʱ���ʱһ��ٶ���һ��ʱ�䳤��Ϊ10����
	output��ʾ�Ƿ�Ҫ������������Ϣ,һ��Ϊtrue,��ʾҪ���,����������������ڲ����þ���ʽָ��Ϊfalse����
*/
int function_findtaxinum_somewhere(set<Point, CMP>& s, double* pos, int day, int hour, int minute, int time_span = 10, bool output = true);


/*��������2: �������ܶȷ����������������r,��������ͼ���ֳ�����ÿ�����ӵĴ�С��r*r��ͳ�Ʒ����ڲ�ͬ��ʱ��Σ������������������ڵĳ����ܶȵı仯��
�������:
	rΪ0.001��γ�Ȼ�����λ��Ӧ�Ĵ���ֵ,��r=2��ÿ�������СΪ0.002*0.002��γ�ȵ�����	
	day,hour,minute��Ӧ�˲��ҵ���ʼʱ��
	time_spac��Ӧ�˲��ҵ�ʱ�䷶Χ,ע��time_span��δָ���������Ĭ��Ϊ60,��λΪ����,Ҳ���Ƿ����ض�ʱ���ʱһ��ٶ���һ��ʱ�䳤��Ϊ60����
	output��ʾ�Ƿ�Ҫ������������Ϣ,һ��Ϊtrue,��ʾҪ���,����������������ڲ����þ���ʽָ��Ϊfalse����
*/
void function_regional_density_analysis(int r, int day, int hour, int minute, int time_span = 60, bool output = true);


/*��������3:�����������1���û�ָ��������������ͳ���ڲ�ͬ��ʱ��Σ���������������֮��ĳ������ı仯��
�������:
	pos1����Ϊ����ĵ�һ����������,pos1[0]��Ӧ���ϽǾ�����Ϣ,pos1[1]��Ӧ���½Ǿ�����Ϣ,
							     pos1[2]��Ӧ���Ͻ�γ����Ϣ,pos1[3]��Ӧ���½�γ����Ϣ
	pos2����Ϊ����ĵ�һ����������,pos2[0]��Ӧ���ϽǾ�����Ϣ,pos2[1]��Ӧ���½Ǿ�����Ϣ,
								 pos2[2]��Ӧ���Ͻ�γ����Ϣ,pos2[3]��Ӧ���½�γ����Ϣ
	day,hour,minute��Ӧ�˲��ҵ���ʼʱ��
	time_spac��Ӧ�˲��ҵ�ʱ�䷶Χ,ע��time_span��δָ���������Ĭ��Ϊ60,��λΪ����,Ҳ���Ƿ����ض�ʱ���ʱһ��ٶ���һ��ʱ�䳤��Ϊ60����
	output��ʾ�Ƿ�Ҫ������������Ϣ,һ��Ϊtrue,��ʾҪ���,����������������ڲ����þ���ʽָ��Ϊfalse����
�����������������ཻʱ�ĳ��⳵���������,���ں�������6�ķ���
*/
set<Point, CMP> function_region_association_analysis_one(double* pos1, double* pos2, int day, int hour, int minute, int time_span = 60, bool output = true);


/*��������4:�����������2���û�ָ��һ����������ͳ�������þ����������������ĳ�������ʱ��ı仯��
�������:
	pos����Ϊ����ľ�������,pos[0]��Ӧ���ϽǾ�����Ϣ,pos[1]��Ӧ���½Ǿ�����Ϣ,
	                      pos[2]��Ӧ���Ͻ�γ����Ϣ,pos[3]��Ӧ���½�γ����Ϣ
	day,hour,minute��Ӧ�˲��ҵ���ʼʱ��
	time_spac��Ӧ�˲��ҵ�ʱ�䷶Χ,ע��time_span��δָ���������Ĭ��Ϊ60,��λΪ����,Ҳ���Ƿ����ض�ʱ���ʱһ��ٶ���һ��ʱ�䳤��Ϊ60����
	output��ʾ�Ƿ�Ҫ������������Ϣ,һ��Ϊtrue,��ʾҪ���,����������������ڲ����þ���ʽָ��Ϊfalse����
*/
void function_region_association_analysis_two(double* pos, int day, int hour, int minute, int time_span = 60, bool output = true);


/*
��������5:Ƶ��·������1��һ��·����Ƶ���ȿ��Զ���Ϊ����·����ͨ�е������������������û������Ĳ���k���������x��ͳ�������������У����ȳ���x����ΪƵ����ǰk��·����\
�������x��ʾ����,k��ʾ����,output��ʾ�Ƿ�Ҫ������������Ϣ
���ؽ�㾭γ����Ϣ����,ÿ��������Ԫ��Ϊһ������,�洢�ĸ�Ԫ��,�ֱ��Ӧ�˽��1�ľ��ȡ�γ��,���2�ľ��Ⱥ�γ��
*/
vector<vector<string>> function_path_frequent_analysis1(double x, int k, bool output = true);

/*
��������6:Ƶ��·������2������������������A��B��������A��B����ΪƵ����ǰk��ͨ��·����
�������pos1��ʾ����1,pos2��ʾ����2,k��ʾ����,output��ʾ�Ƿ�Ҫ������������Ϣ
���ؽ�㾭γ����Ϣ����,ÿ��������Ԫ��Ϊһ������,�洢�ĸ�Ԫ��,�ֱ��Ӧ�˽��1�ľ��ȡ�γ��,���2�ľ��Ⱥ�γ��
*/
vector<vector<double>> function_path_frequent_analysis2(double* pos1, double* pos2, int k, bool output = true);

//��Ϊֻ�������ڵ��·�����
vector<vector<string>> function_path_frequent_analysis2_neighbor(double* pos1, double* pos2, int k, bool output = true);


/*��������7:ͨ��ʱ�����������������������A��B�������ڲ�ͬ��ʱ��Σ����⳵��A��B��ͨ��ʱ����̵�·�����Լ���Ӧ��ͨ��ʱ�䡣
	�������:
	pos1��ʾ����1, pos2��ʾ����2
	pos1����Ϊ����ľ�������,pos1[0]��Ӧ���ϽǾ�����Ϣ,pos1[1]��Ӧ���½Ǿ�����Ϣ,
						  pos1[2]��Ӧ���Ͻ�γ����Ϣ,pos1[3]��Ӧ���½�γ����Ϣ
	pos2����Ϊ����ľ�������,pos2[0]��Ӧ���ϽǾ�����Ϣ,pos2[1]��Ӧ���½Ǿ�����Ϣ,
						  pos2[2]��Ӧ���Ͻ�γ����Ϣ,pos2[3]��Ӧ���½�γ����Ϣ
	day,hour,minute��Ӧ�˲��ҵ���ʼʱ��
	time_spac��Ӧ�˲��ҵ�ʱ�䷶Χ,
	output��ʾ�Ƿ�Ҫ������������Ϣ,ע��time_spanĬ�Ϸ���һ���ʱ��,�����ֶ�ָ��
	����һϵ�еĵ�,�����ʱ��Ŀ�ʼ�������������֮���ȫ�����ʾһ����ȷ��·��
*/
vector<Point> function_pathuse_shortesttime(double* pos1, double* pos2, int day, int hour, int minute, int time_span = 60 * 24, bool output = true);
#endif