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
#define TOTALNUM 450000000
using namespace std;
class inputEdge
{
public:
	char src[16];
	char dst[16];
	bool operator <(const inputEdge & a)const
	{
		if (strcmp(src, a.src) == 0 && strcmp(dst, a.dst) == 0)return false;
		return true;
	}
};
int main(int argc, char *argv[])
{
	string filename = argv[1];
	int low, high, interval;
	low = stoi(argv[2]);
	interval = stoi(argv[3]);
	high = stoi(argv[4]);
	int testEdgeNum = stoi(argv[5]);
	ifstream fin(filename);
	graph *g = new graph();

	char ** src = new char*[TOTALNUM];
	set<inputEdge> edgeSet;
	char ** dst = new char*[TOTALNUM];
	vector<int> wei;
	char  s1[16];
	int n = 0;
	while (fin >> s1)
	{
		char s2[16], s5[16], s6[16], s7[16];
		char * s3 = new char[12];
		char * s4 = new char[12];
		fin >> s2;
		fin >> s3;
		fin >> s4;
		fin >> s5;
		fin >> s6;
		fin >> s7;
		if (strlen(s3) == 0 || strlen(s4) == 0)continue;
		src.push_back(s3);
		dst.push_back(s4);
		++n;
		inputEdge  tmp;
		strcpy(tmp.src, s3);
		strcpy(tmp.dst, s4);
		edgeSet.insert(tmp);
	}
	cout << n << endl;;
	cout << "unique edge num:" << edgeSet.size() << endl;
	fin.close();
	clock_t start, finish;
	start = clock();
	for (int i = 0; i < n; ++i)
	{
		g->insert(src[i], dst[i], 0, 1);
	}
	finish = clock();
	cout << "graph insertion done" << endl;
	double gtime = double(finish - start) / CLOCKS_PER_SEC;
	gtime = n / gtime / MILLION;



	ofstream edgeAAE(filename + "edgeQueryAAE.txt");
	ofstream edgeARE(filename + "edgeQueryARE.txt");
	edgeAAE << "Width" << "\t" << "fsize=12" << "\t" << "fsize=16" << "\t" << "TCM(8*memory)" << endl;
	edgeARE << "Width" << "\t" << "fsize=12" << "\t" << "fsize=16" << "\t" << "TCM(8*memory)" << endl;
	vector<double> gss12ARERV;
	vector<double> gss12AAERV;
	for (int w = low; w <= high; w += interval)
	{

		int uns12_w = 1.07*w;
		GSS uns12(uns12_w, 12, 16, 1, 4, 12);

		clock_t start, finish;

		start = clock();
		for (int i = 0; i < n; ++i)
		{

			uns12.insert(src[i], dst[i], 1);
		}
		finish = clock();

		double d1 = double(finish - start) / CLOCKS_PER_SEC;
		d1 = n / d1 / MILLION;
		cout << "GSS12 insertion done:" << d1 << endl;
		double gss12EdgeAE = 0;
		double gss12EdgeRE = 0;
		int edgeNum = edgeSet.size();
		set<inputEdge>::iterator IT;
		int countEdge = 0;
		for (IT = edgeSet.begin(); IT != edgeSet.end(); ++IT)
		{
			int gEdge = g->query(IT->src, IT->dst, 0);
			if (gEdge == 0) cout << IT->src << endl;
			int gss12Edge = uns12.edgeQuery(IT->src, IT->dst);
			gss12EdgeAE += gss12Edge - gEdge;
			gss12EdgeRE += (gss12Edge - gEdge) / gEdge;
			countEdge++;
			if (countEdge == testEdgeNum)
				break;
		}
		gss12AAERV.push_back(gss12EdgeAE / countEdge);
		gss12ARERV.push_back(gss12EdgeRE / countEdge);
		cout << "w: " << w << " gss12 " << gss12EdgeAE / countEdge << " " << gss12EdgeRE / countEdge << endl;
	}
	vector<double> gss16ARERV;
	vector<double> gss16AAERV;
	for (int w = low; w <= high; w += interval)
	{

		int uns16_w = w;
		GSS uns16(uns16_w, 12, 16, 1, 4, 16);


		clock_t start, finish;

		start = clock();
		for (int i = 0; i < n; ++i)
		{

			uns16.insert(src[i], dst[i], 1);
		}
		finish = clock();

		double d1 = double(finish - start) / CLOCKS_PER_SEC;
		d1 = n / d1 / MILLION;
		cout << "GSS16 insertion done:" << d1 << endl;
		double gss16EdgeAE = 0;
		double gss16EdgeRE = 0;
		int edgeNum = edgeSet.size();
		set<inputEdge>::iterator IT;
		int countEdge = 0;
		for (IT = edgeSet.begin(); IT != edgeSet.end(); ++IT)
		{
			int gEdge = g->query(IT->src, IT->dst, 0);
			int gss16Edge = uns16.edgeQuery(IT->src, IT->dst);
			gss16EdgeAE += gss16Edge - gEdge;
			gss16EdgeRE += (gss16Edge - gEdge) / gEdge;
			countEdge++;
			if (countEdge == testEdgeNum)
				break;
		}
		gss16AAERV.push_back(gss16EdgeAE / countEdge);
		gss16ARERV.push_back(gss16EdgeRE / countEdge);
		cout << "w: " << w << " gss16 " << gss16EdgeAE / countEdge << " " << gss16EdgeRE / countEdge << endl;
	}


	vector<double> tcmAAERV;
	vector<double> tcmARERV;
	for (int w = low; w <= high; w += interval)
	{
		int tcm_w = w*24.5;
		TCM tcm(tcm_w, tcm_w, 4);


		clock_t start, finish;

		start = clock();
		for (int i = 0; i < n; ++i)
		{
			tcm.insert((const unsigned char*)src[i], (const unsigned char*)dst[i], 1, strlen(src[i]), strlen(dst[i]));
		}
		finish = clock();
		double d3 = double(finish - start) / CLOCKS_PER_SEC;
		d3 = n / d3 / MILLION;
		cout << "TCM insertion done:" << d3 << endl;

		double tcmEdgeAE = 0;
		double tcmEdgeRE = 0;
		int edgeNum = edgeSet.size();
		set<inputEdge>::iterator IT;
		int countEdge = 0;
		for (IT = edgeSet.begin(); IT != edgeSet.end(); ++IT)
		{
			int gEdge = g->query(IT->src, IT->dst, 0);
			int tcmEdge = tcm.query((const unsigned char*)(IT->src), (const unsigned char*)(IT->dst),
				strlen(IT->src), strlen(IT->dst));
			tcmEdgeAE += tcmEdge - gEdge;
			tcmEdgeRE += (tcmEdge - gEdge) / gEdge;
			countEdge++;
			if (countEdge == testEdgeNum)
				break;
		}
		tcmAAERV.push_back(tcmEdgeAE / countEdge);
		tcmARERV.push_back(tcmEdgeRE / countEdge);
		cout << "w: " << w << " tcm " << tcmEdgeAE / countEdge << " " << tcmEdgeRE / countEdge << endl;

	}
	for (int w = low, i = 0; w <= high; w += interval, ++i)
	{
		edgeAAE << w << "\t" << gss12AAERV[i] << "\t" << gss16AAERV[i] << "\t" << tcmAAERV[i] << endl;
		edgeARE << w << "\t" << gss12ARERV[i] << "\t" << gss16ARERV[i] << "\t" << tcmARERV[i] << endl;
	}
	for (int i = 0; i < n; ++i)
	{
		delete[] src[i];
		delete[] dst[i];
	}
	delete[] src;
	delete[] dst;
	return 0;
}
