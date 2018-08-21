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
int main(int argc, char *argv[])
{
	string filename = argv[1];
	ifstream fin(filename);
	graph *g = new graph();

	vector<string> src;
	vector<string> dst;
	vector<int> wei;
	string s1, s2;
	while (fin >> s1)
	{
		fin >> s2;
		int weight;
		fin >> weight;
		int timestamp;
		fin >> timestamp;
		src.push_back(s1);
		dst.push_back(s2);
		wei.push_back(weight);
	}
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
	gtime = n / gtime / MILLION;


#ifdef addHashTable
	string foutname = filename + "speed(HashTable).txt";
#else
	string foutname = filename + "speed.txt";
#endif // addHashTable

	ofstream fout(foutname);
	fout << "Width"<<"\t" <<"AdjacentList"<< "\t" <<"fsize=12"<<"\t"<<"fsize=16"<<"\t" << "TCM(256*memory)" << endl;

	for (int w = 700; w <= 1200; w += 50)
	{

		int tcm_w = w*24.5;
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

		fout<<w<<"\t" << gtime << "\t" << d1 << "\t" << d2 << "\t" << d3 << endl;
	}
	return 0;
}
