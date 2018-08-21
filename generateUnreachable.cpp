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
	ifstream fin(filename);
	graph *g = new graph();

	vector<string> src;
	set<string> srcSet;
	set<string> dstSet;
	set<inputEdge> edgeSet;
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
		srcSet.insert(s1);
		dst.push_back(s2);
		dstSet.insert(s2);
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
	ofstream fout("WIKI-unreachable");
	int i = 0;
	int k = 1;
	while (i<100)
	{
		k++;
		srand(k * 4);
		int a = rand() % src.size();
		srand(k * 3);
		int b = rand() % dst.size();
		if (src[a] != src[b] && !g->transquery(src[a], src[b], 0))
		{
			i++;
			fout << src[a] << "\t" << dst[b] << endl;
		}
	}
	return 0;
}
