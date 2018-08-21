#include<iostream>
#include<fstream>
#include<math.h>
#include<string>
#include<vector>
#include<stdlib.h>
#include<set>
#include"tcm.h" 
#include"GSS.h"
#include<time.h>
#include"Graph.h" 
#define MILLION 1000000 
//#define addHashTable
using namespace std;
class inputEdge
{
public:
	string src;
	string dst;
	bool operator <(const inputEdge & a)const
	{
		if (src == a.src&&dst == a.dst)return false;
		return true;
	}
};
int main(int argc, char *argv[])
{
	string filename = argv[1];
	int low,high,interval;
	low = stoi(argv[2]);
	high = stoi(argv[3]);
	interval = stoi(argv[4]);
	ifstream fin(filename);
	graph *g = new graph();

	vector<string> src;
	set<inputEdge> edgeSet;
	vector<string> dst;
	vector<int> wei;
	string s1, s2,s3,s4,s5,s6,s7;
	while (fin >> s1)
	{
		fin >> s2;
		fin >> s3;
		fin >> s4:
		fin >> s5;
		fin >> s6;
		fin >> s7;
		int weight;
		weight= 1;
		int timestamp;
		//fin >> timestamp;
		src.push_back(s3);
		dst.push_back(s4);
		wei.push_back(weight);
		inputEdge  tmp;
		tmp.src  = s3;
		tmp.dst  = s4;
		edgeSet.insert(tmp);
	}
	cout << "unique edge num:" << edgeSet.size() << endl;

	fin.close();
	int n = src.size();
	clock_t start, finish;
	start = clock();
	for (int i = 0; i < n; ++i)
	{
		g->insert(src[i], dst[i], 0, wei[i]);
	}
	finish = clock();
	cout << "graph insertion done" << endl;
	double gtime = double(finish - start) / CLOCKS_PER_SEC;
	cout<<"time:"<<gtime<<endl;
	gtime = n / gtime / MILLION;

	

	ofstream edgeAAE(filename+"edgeQueryAAE.txt");
	ofstream edgeARE(filename + "edgeQueryARE.txt");
	edgeAAE << "Width"  << "\t" << "fsize=12" << "\t" << "fsize=16" << "\t" << "TCM(8*memory)" << endl;
	edgeARE << "Width" << "\t" << "fsize=12" << "\t" << "fsize=16" << "\t" << "TCM(8*memory)" << endl;

	for (int w = low; w <= high; w += interval)
	{

		int tcm_w = w*4.5;//tcm 32bit/room,gss (2fsize+4bit index+32bit)/room
		int uns_w = w;
		TCM tcm(tcm_w, tcm_w, 4);
		GSS uns12(uns_w, 8, 8, 1, 4, 12);
		GSS uns16(uns_w, 8, 8, 1, 4, 16);
		int n = src.size();

		clock_t start, finish;
		start = clock();
		for (int i = 0; i < n; ++i)
		{

			uns12.insert(src[i], dst[i], wei[i]);
		}
		finish = clock();

		double d1 = double(finish - start) / CLOCKS_PER_SEC ;
		cout<<"GSS12 time:"<<d1<<endl;
		d1 = n/d1/MILLION;
		cout << "GSS12 insertion done:" <<d1<<endl;

		start = clock();
		for (int i = 0; i < n; ++i)
		{

			uns16.insert(src[i], dst[i], wei[i]);
		}
		finish = clock();
			
		double d2 = double(finish - start) / CLOCKS_PER_SEC;
		d2 = n / d2 / MILLION;
		cout << "GSS16 insertion done:" << d1 << endl;


		start = clock();
		for (int i = 0; i < n; ++i)
		{
			tcm.insert(((const unsigned char*)src[i].c_str()), ((const unsigned char*)dst[i].c_str()), wei[i], src[i].length(), dst[i].length());
		}
		finish = clock();
		double d3 = double(finish - start) / CLOCKS_PER_SEC ;
		d3 = n/d3/MILLION;
		cout << "TCM insertion done:" << d3 << endl;
		cout<<w<<"\t" << gtime << "\t" << d1 << "\t" << d2 << "\t" << d3 << endl;
		int sum = 0;
		for (int i = 0; i < uns12.n; i++)
		{
			linknode* l = uns12.buffer[i];
			while (l != NULL)
			{
				sum++;
				l = l->next;
			}
		}
		cout << "GSS12 buffer node:" << uns12.n << endl;
		cout << "GSS12 buffer edge sum:" << sum << endl;
		sum = 0;
		for (int i = 0; i < uns16.n; i++)
		{
			linknode* l = uns16.buffer[i];
			while (l != NULL)
			{
				sum++;
				l = l->next;
			}
		}
		cout << "GSS16 buffer node:" << uns16.n << endl;
		cout << "GSS16 buffer edge sum:" << sum << endl;


		double tcmEdgeAE = 0;
		double tcmEdgeRE = 0;
		double gss12EdgeAE = 0;
		double gss12EdgeRE = 0;
		double gss16EdgeAE = 0;
		double gss16EdgeRE = 0;
		int edgeNum = edgeSet.size();
		set<inputEdge>::iterator IT;
		start = clock();
		for (IT = edgeSet.begin(); IT != edgeSet.end(); ++IT)
		{
			//uniqueEdge << IT->src << " " << IT->dst << endl;
			int gEdge = g->query(IT->src, IT->dst, 0);
			int tcmEdge = tcm.edgeQuery(((const unsigned char*)(IT->src).c_str()), ((const unsigned char*)(IT->dst).c_str()),
				(IT->src).length(), (IT->dst).length());
			int gss12Edge = uns12.edgeQuery(IT->src, IT->dst);
			int gss16Edge = uns16.edgeQuery(IT->src, IT->dst);
			tcmEdgeAE += tcmEdge - gEdge;
			tcmEdgeRE += (tcmEdge - gEdge) / gEdge;
			gss12EdgeAE += gss12Edge - gEdge;
			gss12EdgeRE += (gss12Edge - gEdge) / gEdge;
			gss16EdgeAE += gss16Edge - gEdge;
			gss16EdgeRE += (gss16Edge - gEdge) / gEdge;
		}
		finish  = clock();
		cout<<"Query Time"<<double(finish-start)/CLOCKS_PER_SEC<<endl;
		edgeAAE << w << "\t" << gss12EdgeAE / edgeNum << "\t" << gss16EdgeAE / edgeNum << "\t" << tcmEdgeAE / edgeNum << endl;
		edgeARE << w << "\t" << gss12EdgeRE / edgeNum << "\t" << gss16EdgeRE / edgeNum << "\t" << tcmEdgeRE / edgeNum << endl;
	}
	return 0;
}
