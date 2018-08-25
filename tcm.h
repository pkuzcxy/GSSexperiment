#include"hashfunction.h"
#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "hashTable.h"
#endif // HASHTABLE_H
#include<math.h>
#include<string>
#include<iostream>
#include<memory.h>
#include<queue>
using namespace std;
class TCM
{
private:
    int width;
    int depth;
    int hashnum;
    int **value;
public:
	hashTable<string> *mapTable;
	int tablesize;
    TCM(int w,int d,int h);
    ~TCM()
    {
		for (int i=0; i<hashnum; ++i)
        {
                delete [] value[i];
        }
        delete [] value;
		delete [] mapTable;
    }
    void saveOperation(int target_d,int target_w,int hashnum, int weight);
    void insert(const unsigned char* v1,const unsigned char* v2,int weight,int len1,int len2);
    int query(const unsigned char* v1,const unsigned char* v2,int len1,int len2);
    int nodequery(const unsigned char*v1, int len, int type);
	int nodedegreequery(const unsigned char*v1, int len, int type);
    bool transquery(const unsigned char*v1,const unsigned char*v2, int len1,int len2);
};

bool TCM::transquery(const unsigned char*v1, const unsigned char*v2, int len1,int  len2)
{
	for(int i=0;i<hashnum;i++)
	{
		bool found = false;
		bool* checked = new bool[width];
		for(int j=0;j<width;j++)
			checked[j] = false;
		unsigned int src=((*hfunc[i])(v1,len1))%depth;
		unsigned int dest=((*hfunc[i])(v2,len2))%width;
		queue<int> q;
		unsigned int v1 = src;
		unsigned int v2 = dest;
		int hash = v1*width+v2;
		if(value[i][hash]>0)//?��?��?����?
			continue;
		else
		{
			q.push(v1);
			checked[v1]=true; 
			while(!q.empty())
			{
				v1 = q.front();
				if(value[i][v1*width+dest]>0)
				{
				//	cout<<"found"<<endl;
					found = true;
					break;
				}
				for(int j=0;j<width;j++)
				{
					hash = v1*width+j;
					if(value[i][hash]>0&&checked[j]==false)
					{
						q.push(j);
				//		cout<<"in!";
						checked[j]=true;
					}
				}
				q.pop();
			}
			if(!found)
				return false;
		}
		delete [] checked;
	}
	return true;
}
TCM::TCM(int w,int d,int h)
{
    width = w;
	tablesize = w;
    depth = d;
    hashnum = h;
    value = new int*[hashnum];
	mapTable = new hashTable<string>[hashnum];
    for (int i=0; i<hashnum; ++i)
    {
        value[i] = new int[width*depth];
        memset(value[i],0,sizeof(int)*width*depth);
		mapTable[i].init(tablesize);
    }      
}

void TCM::saveOperation(int target_d,int target_w,int hashnum,int weight)
{
    if(value[hashnum][target_d*width + target_w] + weight > 0) value[hashnum][target_d*width+target_w] += weight;
}

void TCM::insert(const unsigned char* v1,const unsigned char* v2,int weight,int len1,int len2)
{
	for (int i = 0; i < hashnum; i++)
	{
		unsigned int hash1 = ((*hfunc[i])(v1, len1));
		unsigned int hash2 = ((*hfunc[i])(v2, len2));
		hash1 = hash1 % depth;
		hash2 = hash2 % width;
		value[i][hash1*width + hash2] += weight;
#ifdef addHashTable
		string s1 = (char *)v1;
		string s2 = (char *)v2;
		mapTable[i].insert(hash1, s1);
		mapTable[i].insert(hash2, s2);
#endif // addHashTable
	}
}

int TCM::query(const unsigned char* v1,const unsigned char* v2,int len1,int len2)
{
    int min=INT32_MAX;
    for(int i=0;i<hashnum;i++)
    {
    	unsigned int hash1 = ((*hfunc[i])(v1,len1))%depth;
    	unsigned int hash2 = ((*hfunc[i])(v2,len2))%width;
    	int v = value[i][hash1*width+hash2];
    	if(v<min)
    	min = v;
	}
    return min;
}

int TCM::nodequery(const unsigned char* v1, int len, int type )
{
	if(type==0)
	{
		//int min=65523*1024;
		int min = INT32_MAX;
		for(int i=0;i<hashnum;i++)
		{
			unsigned int hash1 = ((*hfunc[i])(v1,len))%depth;
			hash1 = hash1*width;
			int sum=0;
			for(int j=0;j<width;j++)
			{
				sum+=value[i][hash1+j];
			}
			if(sum<min)
			min=sum;
			if(min==0)
				break;
		}
		return min;
	}
	else
	{
				//int min=65536*1024;
				int min = INT32_MAX;
		for(int i=0;i<hashnum;i++)
		{
			unsigned int hash1 = ((*hfunc[i])(v1,len))%width;
			int sum=0;
			for(int j=0;j<depth;j++)
				sum+=value[i][j*width+hash1];
			if(sum<min)
			min=sum;
			if(min==0)
				break;
		}
		return min;	
	}
 } 


int TCM::nodedegreequery(const unsigned char* v1, int len, int type)
{
	int min = INT32_MAX;
	if (type == 0)
	{
		for (int i = 0; i<hashnum; i++)
		{
			unsigned int hash1 = ((*hfunc[i])(v1, len)) % depth;
			hash1 = hash1*width;
			int sum = 0;
			for (int j = 0; j<width; j++)
			{
				if (value[i][hash1 + j] > 0)sum+=mapTable[i].countIDnums(j);
			}
			if (sum < min)
				min = sum;
			if (min == 0)
				break;
		}
	}
	else
	{
		for (int i = 0; i<hashnum; i++)
		{
			unsigned int hash1 = ((*hfunc[i])(v1, len)) % width;
			int sum = 0;
			for (int j = 0; j < depth; j++)
			{
				if (value[i][j*width + hash1] > 0) sum+=mapTable[i].countIDnums(j);
			
			}
			if (sum < min)
				min = sum;
			if (min == 0)
				break;
		}
	}
	return min;
}
