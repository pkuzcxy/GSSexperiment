#include<iostream>
#include<string>
#include<vector>
#include<queue>
#include<set>
#include<map>
#include<cmath>
#include<stdlib.h>
#include<bitset>
#include<memory.h>
#ifndef HASHTABLE_H
#define HASHTABLE_H
#include "hashTable.h" 
#endif // HASHTABLE_H
using namespace std;
#define prime 739
#define bigger_p 1048576
#define timer 5
#define M 80000
#define Roomnum 4
struct basket
{
	unsigned short src[Roomnum];
	unsigned short dst[Roomnum];
	int  weight[Roomnum];
	unsigned int idx;
};
struct mapnode
{
	unsigned int h;
	unsigned short g;
};
struct linknode
{
	unsigned int key;
	short weight;
	linknode* next;
};

class GSS
{
private:
	hashTable<string> mapTable;
	int tablesize;
	int w;
	int r;
	int p;
	int k;
	int s;
	int f;
	
	basket* value;

	public:
		vector<linknode*> buffer;
		map<unsigned int, int> index;
		int n;
		GSS(int width, int range, int p_num, int k_num, int size,int f_num);
		~GSS()
		{
			delete[] value;
			cleanupBuffer();
		 }
		 void insert(string s1, string s2,int weight);
		 void cleanupBuffer();
		 int edgeQuery(string s1, string s2);
		 bool query(string s1, string s2);
		 int nodeValueQuery(string s1, int type);//src_type = 0 dst_type = 1
		 int nodeDegreeQuery(string s1, int type);//src_type = 0 dst_type = 1
		 void nodeSuccessorQuery(string s1, vector<string> &IDs);
		 void Successor_for_Triangle(unsigned int hash1, vector<hashvalue> &HV, bool insequence=false, unsigned int standard=0);
		 void nodePrecursorQuery(string s2, vector<string> &IDs); 
		 void Precursor_for_Triangle(unsigned int hash1, vector<hashvalue> &HV, bool insequence=false, unsigned int standard=0);
		 int TriangleCounting_loop();
		 int TriangleCounting();
};

GSS::GSS(int width, int range, int p_num, int k_num, int size,int f_num)
{
	w = width;
	r = range; /* r x r mapped baskets */
	p = p_num; /*candidate buckets*/
	k = k_num;
	s = size; /*multiple rooms*/
	f = f_num; /*finger print lenth*/
	n = 0;
	value = new basket[w*w];
	memset(value, 0, sizeof(basket)*w*w);
	tablesize = w;
	mapTable.init(tablesize);
}
void GSS::cleanupBuffer()
{
	vector<linknode*>::iterator IT = buffer.begin();
	linknode* e, *tmp;
	for (; IT != buffer.end(); ++IT)
	{
		e = *IT;
		while (e != NULL)
		{
			tmp = e->next;
			delete e;
			e = tmp;
		}
	}
}
void GSS::insert(string s1, string s2,int weight)
{		
		unsigned int hash1 = (*hfunc[0])((unsigned char*)(s1.c_str()), s1.length());
		unsigned int hash2 = (*hfunc[0])((unsigned char*)(s2.c_str()), s2.length());
		unsigned int tmp = pow(2,f)-1;
		unsigned short g1 = hash1 & tmp;
		if(g1==0) g1 += 1;
		unsigned int h1 = (hash1>>f)%w;
		unsigned short g2 = hash2 & tmp;
		if(g2==0) g2 += 1;
		unsigned int h2 = (hash2>>f)%w;
		
		unsigned int k1 = (h1<<f)+g1;
		unsigned int k2 = (h2<<f)+g2;
//#ifdef addHashTable
	
		mapTable.insert(k1, s1);
		mapTable.insert(k2, s2);
//#endif // addHashTable
		int* tmp1 = new int[r];
		int* tmp2 = new int[r];
		tmp1[0] = g1;
		tmp2[0] = g2;
		for(int i=1;i<r;i++)
		{
			tmp1[i]=(tmp1[i-1]*timer+prime)%bigger_p;
			tmp2[i]=(tmp2[i-1]*timer+prime)%bigger_p;
		}
		bool inserted=false;
		long key = g1+g2; 
		for(int i=0;i<p;i++)
		{
			key = (key*timer+prime)%bigger_p;
			int index = key%(r*r);
			int index1 = index/r;
			int index2 = index%r; 
			int p1 = (h1+tmp1[index1])%w;
			int p2 = (h2+tmp2[index2])%w;


			int pos = p1*w + p2;
			for (int j = 0; j < s; j++)
			{
				if ( ( ((value[pos].idx>>(j<<3))&((1<<8)-1)) == (index1|(index2<<4)) ) && (value[pos].src[j]== g1) && (value[pos].dst[j] == g2) )
				{
					value[pos].weight[j] += weight;
					return;
				}
				if (value[pos].src[j] == 0)
				{
					/*if (g1==48793)
					{
						cout<<"find! "<<index1<<' '<<h1<<endl;
					}*/
					value[pos].idx |= ((index1 | (index2 << 4)) << (j<<3));
					value[pos].src[j] = g1;
					value[pos].dst[j] = g2;
					value[pos].weight[j] = weight;
					inserted = true;
					break;
				}
			}
			if(inserted)
				break;
		} 
		if(!inserted)
		{
		
			map<unsigned int, int>::iterator it = index.find(k1);
			if(it!=index.end())
			{
				int tag = it->second;
				linknode* node = buffer[tag];
				while(true)
				{
					if (node->key == k2)
					{   
						node->weight += weight;
						break;
					}
					if(node->next==NULL)
					{
						linknode* ins = new linknode;
						ins->key = k2;
						ins->weight = weight;
						ins->next = NULL;
						node->next = ins;
						break;
					}
					node = node->next;
				}
			}
			else
			{
				index[k1] = n;
				n++;
				linknode* node = new linknode;
				node->key = k1;
				node->weight = 0;
				if (k1 != k2)//k1==k2 means loop
				{
					linknode* ins = new linknode;
					ins->key = k2;
					ins->weight = weight;
					ins->next = NULL;
					node->next = ins;
				}
				else
				{ 
					node->weight += weight;
					node->next = NULL;
				}
				buffer.push_back(node); 
			}	
		}
		delete [] tmp1;
		delete [] tmp2;
	return;
}
void GSS::nodeSuccessorQuery(string s1, vector<string>&IDs)
{
	unsigned int hash1 = (*hfunc[0])((unsigned char*)(s1.c_str()), s1.length());
	int tmp=pow(2,f)-1;
	unsigned short g1=hash1 & tmp;
	unsigned int h1 = (hash1>>f)%w;
	int* tmp1 = new int[r];
	tmp1[0] = g1;
	for (int i = 1; i < r; i++)
	{
		tmp1[i] = (tmp1[i - 1] * timer + prime) % bigger_p;
	}
	for (int i = 0; i < r; i++)
	{
		int p1 = (h1 + tmp1[i]) % w;
		for (int k = 0; k < w; k++)
		{
			int pos = p1*w + k;
			for (int j = 0; j < s; ++j)
			{
				if ((((value[pos].idx >> ((j << 3)))&((1 << 4) - 1)) == i) && (value[pos].src[j] == g1))
				{
					     int tmp_g = value[pos].dst[j];
						 int tmp_s = ((value[pos].idx >> ((j << 3) + 4))&((1 << 4) - 1));
				
						 int shifter = tmp_g;
						 for (int v = 0; v < tmp_s; v++)
							 shifter = (shifter*timer + prime) % bigger_p;
						 int tmp_h = k;
						 while (tmp_h < shifter)
							 tmp_h += w;
						 tmp_h -= shifter;
						 int val = (tmp_h << f) + tmp_g;
						 mapTable.getID(val, IDs);
				}
			}
		}	
	}
		map<unsigned int, int>::iterator it = index.find(hash1);
		if (it != index.end())
		{
			int tag = it->second;
			linknode* node = buffer[tag];
			while (node != NULL)
			{
				mapTable.getID(node->key, IDs);
				node=node->next;
			}
		}
		return; 
}
void GSS::nodePrecursorQuery(string s1, vector<string>&IDs)
{
	unsigned int hash1 = (*hfunc[0])((unsigned char*)(s1.c_str()), s1.length());
	int tmp=pow(2,f)-1;
	unsigned g1=hash1 & tmp;
	unsigned int h1 = (hash1>>f)%w;
	int* tmp1 = new int[r];
	tmp1[0] = g1;
	for (int i = 1; i < r; i++)
	{
		tmp1[i] = (tmp1[i - 1] * timer + prime) % bigger_p;
	}
	for (int i = 0; i < r; i++)
	{
		int p1 = (h1 + tmp1[i]) % w;
		for (int k = 0; k < w; k++)
		{
			int pos = p1 + k*w;
			for (int j = 0; j < s; ++j)
			{
				if ((((value[pos].idx >> ((j << 3)+4))&((1 << 4) - 1)) == i) && (value[pos].dst[j] == g1))
				{
					     int tmp_g = value[pos].src[j];
						 int tmp_s = ((value[pos].idx >> (j << 3))&((1 << 4) - 1));
				
						 int shifter = tmp_g;
						 for (int v = 0; v < tmp_s; v++)
							 shifter = (shifter*timer + prime) % bigger_p;
						 int tmp_h = k;
						 while (tmp_h < shifter)
							 tmp_h += w;
						 tmp_h -= shifter;
						 
						 int val = (tmp_h << f) + tmp_g;
						 mapTable.getID(val, IDs);
				}
			}
		}	
	}
			for (map<unsigned int, int>::iterator it = index.begin(); it != index.end(); ++it)
		{
			int tag = it->second;
			int src = it->first;
			linknode* node = buffer[tag];
			while (node != NULL)
			{
				if(node->key == hash1)
				{
					mapTable.getID(src, IDs);
					break;	
				}
			}
		}
		return;
}
int GSS::edgeQuery(string s1, string s2)
{
	unsigned int hash1 = (*hfunc[0])((unsigned char*)(s1.c_str()), s1.length());
	unsigned int hash2 = (*hfunc[0])((unsigned char*)(s2.c_str()), s2.length());
	int tmp = pow(2, f) - 1;
	unsigned short g1 = hash1 & tmp;
	if (g1 == 0) g1 += 1;
	unsigned int h1 = (hash1 >> f) % w;
	unsigned short g2 = hash2 & tmp;
	if (g2 == 0) g2 += 1;
	unsigned int h2 = (hash2 >> f) % w;
	int* tmp1 = new int[r];
	int* tmp2 = new int[r];
	tmp1[0] = g1;
	tmp2[0] = g2;
	for (int i = 1; i<r; i++)
	{
		tmp1[i] = (tmp1[i - 1] * timer + prime) % bigger_p;
		tmp2[i] = (tmp2[i - 1] * timer + prime) % bigger_p;
	}
	long key = g1 + g2;
	for (int i = 0; i<p; i++)
	{
		key = (key * timer + prime) % bigger_p;
		int index = key % (r*r);
		int index1 = index / r;
		int index2 = index%r;
		int p1 = (h1 + tmp1[index1]) % w;
		int p2 = (h2 + tmp2[index2]) % w;
		int pos = p1*w + p2;
		for (int j = 0; j<s; j++)
		{
		
			if ((((value[pos].idx >> (j << 3))&((1 << 8) - 1)) == (index1 | (index2 << 4))) && (value[pos].src[j] == g1) && (value[pos].dst[j] == g2))
			{
				return value[pos].weight[j];
			}
		}
		
	}
	unsigned int k1 = (h1 << f) + g1;
	unsigned int k2 = (h2 << f) + g2;
	map<unsigned int, int>::iterator it = index.find(k1);
	if (it != index.end())
	{
		int tag = it->second;
		linknode* node = buffer[tag];
		while (node!=NULL)
		{
			if (node->key == k2)
				return node->weight;
			node = node->next;
		}
	}
		return 0;
}
bool GSS::query(string s1, string s2)
 {
	 unsigned int hash1 = (*hfunc[0])((unsigned char*)(s1.c_str()), s1.length());
	 unsigned int hash2 = (*hfunc[0])((unsigned char*)(s2.c_str()), s2.length());
	 int tmp = pow(2, f) - 1;
	 unsigned short g1 = hash1 & tmp;
	 if (g1 == 0) g1 += 1;
	 unsigned int h1 = (hash1 >> f) % w;
	 unsigned short g2 = hash2 & tmp;
	 if (g2 == 0) g2 += 1;
	 unsigned int h2 = (hash2 >> f) % w;
	 int pos;
	 map<unsigned int, bool> checked;
	 queue<mapnode> q;
	 mapnode e;
	 e.h = h1;
	 e.g = g1;
	 q.push(e);
	 checked[((h1 << f) + g1)] = true;
	 map<unsigned int, bool>::iterator	IT;

	 while (!q.empty())
	 {
		 e = q.front();
		 h1 = e.h;
		 g1 = e.g;
		 int* tmp1 = new int[r];
		 int* tmp2 = new int[r];
		 tmp2[0] = g2;
		 tmp1[0] = g1;
		 for (int i = 1; i<r; i++)
		 {
			 tmp1[i] = (tmp1[i - 1] * timer + prime) % bigger_p;
			 tmp2[i] = (tmp2[i - 1] * timer + prime) % bigger_p;
		 }
		 for (int i1 = 0; i1<r; i1++)
		 {
			 int p1 = (h1 + tmp1[i1]) % w;
			 for (int i2 = 0; i2<r; i2++)
			 {
				 int p2 = (h2 + tmp2[i2]) % w;
				 int pos = p1*w + p2;
				 for (int i3 = 0; i3<s; i3++)
				 {
				
					 if ((((value[pos].idx >> (i3 << 3))&((1 << 8) - 1)) == (i1 | (i2 << 4))) && (value[pos].src[i3] == g1) && (value[pos].dst[i3] == g2))
					 {
						 return true;
					 }
				 }
			 }
		 }
		 unsigned int k1 = (h1 << f) + g1;
	//	 bool find = true;
	//	 if (find)
	//	 {
			 map<unsigned int, int>::iterator it = index.find(k1);
			 if (it != index.end())
			 {
				 int tag = it->second;
				 linknode* node = buffer[tag];
				 while (node != NULL)
				 {
					 if (node->key != k1)
					 {
						 unsigned int val = node->key;
						 unsigned int temp_h = (val) >> f;
						 unsigned int tmp = pow(2, f);
						 unsigned short temp_g = (val%tmp);
						 if ((temp_h == h2) && (temp_g == g2))
							 return true;
				
						 IT = checked.find(val);
						 if (IT == checked.end())
						 {
							 mapnode temp_e;
							 temp_e.h = temp_h;
							 temp_e.g = temp_g;
							 q.push(temp_e);
							 checked[val] = true;;
						 }
					 }
					 node = node->next;
				 }
			 }
	//	 }
		 for (int i1 = 0; i1<r; i1++)
		 {
			 int p1 = (h1 + tmp1[i1]) % w;
			 for (int i2 = 0; i2<w; i2++)
			 {
				 int pos = p1*w + i2;
				 for (int i3 = 0; i3<s; i3++)
				 {
		
				
					 if (value[pos].src[i3] == g1 && (((value[pos].idx >> (i3 << 3))&((1 << 4) - 1)) == i1))
					 {
						 int tmp_g = value[pos].dst[i3];
						 int tmp_s = ((value[pos].idx >> ((i3 << 3) + 4))&((1 << 4) - 1));
				
						 int shifter = tmp_g;
						 for (int v = 0; v < tmp_s; v++)
							 shifter = (shifter*timer + prime) % bigger_p;
						 int tmp_h = i2;
						 while (tmp_h < shifter)
							 tmp_h += w;
						 tmp_h -= shifter;


						 unsigned int val = (tmp_h << f) + tmp_g;
					 
						 IT = checked.find(val);
						 if (IT == checked.end())
						 {
						
							 mapnode tmp_e;
							 tmp_e.h = tmp_h;
							 tmp_e.g = tmp_g;
							 q.push(tmp_e);
							 checked[val] = true;
						
						 }
					 }
					
				 }
			 }
		 }
		 delete[] tmp1;
		 delete[] tmp2;
		 q.pop();
	 }
	 return false;
 }
/*type 0 is for successor query, type 1 is for precusor query*/
int GSS::nodeValueQuery(string s1, int type)
{
	int weight = 0;
	unsigned int hash1 = (*hfunc[0])((unsigned char*)(s1.c_str()), s1.length());
	int tmp = pow(2, f) - 1;
	unsigned short g1 = hash1 & tmp;
	if (g1 == 0) g1 += 1;
	unsigned int h1 = (hash1 >> f) % w;
	int* tmp1 = new int[r];
	tmp1[0] = g1;
	for (int i = 1; i < r; i++)
	{
		tmp1[i] = (tmp1[i - 1] * timer + prime) % bigger_p;
	}
	for (int i = 0; i < r; i++)
	{
		int p1 = (h1 + tmp1[i]) % w;
		for (int k = 0; k < w; k++)
		{
			if (type == 0)/*successor query*/
			{
				int pos = p1*w + k;
				for (int j = 0; j < s; ++j)
				{
					if (type == 0 && (((value[pos].idx >> ((j << 3)))&((1 << 4) - 1)) == i) && (value[pos].src[j] == g1))
					{
						weight += value[pos].weight[j];
					}
				}
			}
			else if (type == 1)/*precursor query*/
			{
				int pos = p1 + k*w;
				for (int j = 0; j < s; ++j)
				{
					if (type == 0 && (((value[pos].idx >> ((j << 3) + 4))&((1 << 4) - 1)) == i) && (value[pos].dst[j] == g1))
					{
						weight += value[pos].weight[j];
					}
				}
			}
		}
	}
	if (type == 0)
	{
		unsigned int k1 = (h1 << f) + g1;
		map<unsigned int, int>::iterator it = index.find(k1);
		if (it != index.end())
		{
			int tag = it->second;
			linknode* node = buffer[tag];
			while (node != NULL)
			{
				weight += node->weight;
				node = node->next;
			}
		}
	}
	else if (type==1)
	{
		unsigned int k1 = (h1 << f) + g1;
		for (map<unsigned int, int>::iterator it = index.begin(); it != index.end(); ++it)
		{
			int tag = it->second;
			linknode* node = buffer[tag];
			while (node != NULL)
			{
				if(node->key == k1)
					weight += node->weight;
				node = node->next;
			}
		}
	}
	return weight;
}
/*type 0 is for successor query, type 1 is for precusor query*/
int GSS::nodeDegreeQuery(string s1, int type)
{
	int degree = 0;
	unsigned int hash1 = (*hfunc[0])((unsigned char*)(s1.c_str()), s1.length());
	int tmp = pow(2, f) - 1;
	unsigned short g1 = hash1 & tmp;
	if (g1 == 0) g1 += 1;
	unsigned int h1 = (hash1 >> f) % w;
	int* tmp1 = new int[r];
	tmp1[0] = g1;
	for (int i = 1; i < r; i++)
	{
		tmp1[i] = (tmp1[i - 1] * timer + prime) % bigger_p;
	}
	for (int i = 0; i < r; i++)
	{
		int p1 = (h1 + tmp1[i]) % w;
		for (int k = 0; k < w; k++)
		{
			if (type == 0)/*successor query*/
			{
				int pos = p1*w + k;
				for (int j = 0; j < s; ++j)
				{
					if ((((value[pos].idx >> ((j << 3)))&((1 << 4) - 1)) == i) && (value[pos].src[j] == g1))
					{
						 int tmp_g = value[pos].dst[j];
						 int tmp_s = ((value[pos].idx >> ((j << 3) + 4))&((1 << 4) - 1));
				
						 int shifter = tmp_g;
						 for (int v = 0; v < tmp_s; v++)
							 shifter = (shifter*timer + prime) % bigger_p;
						 int tmp_h = k;
						 while (tmp_h < shifter)
							 tmp_h += w;
						 tmp_h -= shifter;
						 unsigned int val = (tmp_h << f) + tmp_g;
						 degree+=mapTable.countIDnums(val);
					}
				}
			}
			else if (type == 1)/*precursor query*/
			{
				int pos = p1 + k*w;
				for (int j = 0; j < s; ++j)
				{
					if ((((value[pos].idx >> ((j << 3) + 4))&((1 << 4) - 1)) == i) && (value[pos].dst[j] == g1))
					{
						 int tmp_g = value[pos].src[j];
						 int tmp_s = ((value[pos].idx >> (j << 3))&((1 << 4) - 1));
				
						 int shifter = tmp_g;
						 for (int v = 0; v < tmp_s; v++)
							 shifter = (shifter*timer + prime) % bigger_p;
						 int tmp_h = k;
						 while (tmp_h < shifter)
							 tmp_h += w;
						 tmp_h -= shifter;
						 unsigned int val = (tmp_h << f) + tmp_g;
						degree+=mapTable.countIDnums(val);
					}
				}
			}
		}
	}

	if (type == 0)
	{
		unsigned int k1 = (h1 << f) + g1;
		map<unsigned int, int>::iterator it = index.find(k1);
		if (it != index.end())
		{
			int tag = it->second;
			linknode* node = buffer[tag];
			while (node != NULL)
			{
				degree+=mapTable.countIDnums(node->key);
				node = node->next;
			}
		}
	}
	else if (type == 1)
	{
		unsigned int k1 = (h1 << f) + g1;
		for (map<unsigned int, int>::iterator it = index.begin(); it != index.end(); ++it)
		{
			int tag = it->second;
			linknode* node = buffer[tag];
			unsigned int src=node->key;
			while (node != NULL)
			{
				if (node->key == k1)
				{
					degree+=mapTable.countIDnums(src);
					break;
				 } 
				node = node->next;
			}
		}
	}
	return degree;
}
void GSS::Successor_for_Triangle(unsigned int hash1, vector<hashvalue>&HV, bool insequence, unsigned int standard)
{
	int tmp=pow(2,f)-1;
	unsigned short g1=hash1 & tmp;
	unsigned int h1 = (hash1 >> f) % w;
	unsigned int remain=standard%tablesize;
	int* tmp1 = new int[r];
	tmp1[0] = g1;
	for (int i = 1; i < r; i++)
	{
		tmp1[i] = (tmp1[i - 1] * timer + prime) % bigger_p;
	}
	for (int i = 0; i < r; i++)
	{
		int p1 = (h1 + tmp1[i]) % w;
		for (int k = 0; k < w; k++)
		{
			int pos = p1*w + k;
			for (int j = 0; j < s; ++j)
			{
				if ((((value[pos].idx >> ((j << 3)))&((1 << 4) - 1)) == i) && (value[pos].src[j] == g1))
				{
					     int tmp_g = value[pos].dst[j];
						 int tmp_s = ((value[pos].idx >> ((j << 3) + 4))&((1 << 4) - 1));
				
						 int shifter = tmp_g;
						 for (int v = 0; v < tmp_s; v++)
							 shifter = (shifter*timer + prime) % bigger_p;
						 int tmp_h = k;
						 while (tmp_h < shifter)
							 tmp_h += w;
						 tmp_h -= shifter;
						 unsigned int val = (tmp_h << f) + tmp_g;
						 unsigned int tmp_remain=val%tablesize;
						 if(!insequence||tmp_remain>remain||(tmp_remain==remain&&val>standard))
						 {
						 	hashvalue hv;
						 	hv.key=val;
						 	hv.IDnum=mapTable.countIDnums(val);
						 	/*if(hv.IDnum==0)
						 	{
						 		cout<<val<<" IDerror"<<endl;
							}*/
						 	HV.push_back(hv);	
						 }
				}
			}
		}	
	}
		map<unsigned int, int>::iterator it = index.find(hash1);
		if (it != index.end())
		{
			int tag = it->second;
			linknode* node = buffer[tag];
			node = node->next;
			while (node != NULL)
			{
				unsigned int val=node->key;
				unsigned int tmp_remain=val%tablesize;
				if(!insequence||tmp_remain>remain||(tmp_remain==remain&&val>standard))
				{
					hashvalue hv;
					hv.key=val;
					hv.IDnum=mapTable.countIDnums(hv.key);
				/*	if(hv.IDnum==0)
						 	{
						 		cout<<val<<" IDerror"<<endl;
							}*/
					HV.push_back(hv);	
				}
			}
		}
		return; 
}
void GSS::Precursor_for_Triangle(unsigned int hash1, vector<hashvalue>&HV, bool insequence, unsigned int standard)
{
	int tmp=pow(2,f)-1;
	int g1=hash1 & tmp;
	unsigned int h1 = (hash1 >> f) % w;
	unsigned int remain=standard%tablesize;
	int* tmp1 = new int[r];
	tmp1[0] = g1;
	for (int i = 1; i < r; i++)
	{
		tmp1[i] = (tmp1[i - 1] * timer + prime) % bigger_p;
	}
	for (int i = 0; i < r; i++)
	{
		int p1 = (h1 + tmp1[i]) % w;
		for (int k = 0; k < w; k++)
		{
			int pos = p1 + k*w;
			for (int j = 0; j < s; ++j)
			{
				if ((((value[pos].idx >> ((j << 3)+4))&((1 << 4) - 1)) == i) && (value[pos].dst[j] == g1))
				{
					     int tmp_g = value[pos].src[j];
						 int tmp_s = ((value[pos].idx >> (j << 3))&((1 << 4) - 1));
				
						 int shifter = tmp_g;
						 for (int v = 0; v < tmp_s; v++)
							 shifter = (shifter*timer + prime) % bigger_p;
						 int tmp_h = k;
						 while (tmp_h < shifter)
							 tmp_h += w;
						 tmp_h -= shifter;
						 
						 unsigned int val = (tmp_h << f) + tmp_g;
						 unsigned int tmp_remain = val % tablesize;
							if(!insequence||tmp_remain>remain||(tmp_remain==remain&&val>standard))
						 {
						 	hashvalue hv;
						 	hv.key=val;
						 	hv.IDnum=mapTable.countIDnums(val);
						 	/*if(hv.IDnum==0)
						 	{
						 		cout<<value[pos].src[j]<<endl;
						 		cout<<tmp_g<<' '<<tmp_h<<' '<<tmp_s<<' '<<k<<endl;
						 		cout<<val<<" IDerror"<<endl;
							}*/
						 	HV.push_back(hv);	
						 }
				}
			}
		}	
	}
			for (map<unsigned int, int>::iterator it = index.begin(); it != index.end(); ++it)
		{
			int tag = it->second;
			unsigned int val = it->first;
			linknode* node = buffer[tag];
			node = node->next;
			while (node != NULL)
			{
				if(node->key == hash1)
				{
					unsigned int tmp_remain = val % tablesize;
					if(!insequence||tmp_remain>remain||(tmp_remain==remain&&val>standard))
					{
					hashvalue hv;
					hv.key=val;
					hv.IDnum=mapTable.countIDnums(hv.key);
					/*if(hv.IDnum==0)
						 	{
						 		cout<<val<<" IDerror"<<endl;
							}*/
					HV.push_back(hv);
				    }
					break;	
				}
			}
		}
		return;
}
void hvinsert(unsigned int hash, std::vector<hashvalue> &v)
{
	bool find=false;
	for(int i=0;i<v.size();i++)
	{
		if (hash==v[i].key)
		{
			v[i].IDnum++;
			find=true;
			break;
		}
	}
	if(!find)
	{
		hashvalue hv;
		hv.key=hash;
		hv.IDnum=1;
		v.push_back(hv);
	}
	return;
}
int GSS::TriangleCounting_loop()
{
	int count=0;
	for(int i=0;i<tablesize;i++)
	{
		std::vector<hashvalue> node1_set;
		hashTableNode<string> *np;
		np=mapTable.table[i];
		for(;np!=NULL;np=np->next)
		{	
		unsigned int hash=np->key;
	//	cout<<hash<<endl;
		hvinsert(hash, node1_set);
		}
		sort(node1_set.begin(), node1_set.end(), mycomp);
		for(int j=0;j<node1_set.size();j++)
		{
			unsigned int hash1=node1_set[j].key;
			int num1=node1_set[j].IDnum;
			//cout<<num1<<' ';
			std::vector<hashvalue> node2_set;
			Successor_for_Triangle(hash1,node2_set, true,hash1);
		//	cout<<node2_set.size()<<endl;
			for(int k=0;k<node2_set.size();k++)
			{
				unsigned hash2=node2_set[k].key;
				int num2=node2_set[k].IDnum;
				//cout<<num2<<' ';
				std::vector<hashvalue> node3_set, precursor_set;
				Successor_for_Triangle(hash2,node3_set,true,hash1);
				Precursor_for_Triangle(hash1,precursor_set,true,hash1);
			//  cout<<node3_set.size()<<' '<<precursor_set.size()<<endl;
				int num3=countjoin(node3_set,precursor_set);
				//cout<<"join "<<num3<<endl;
				count+=num3*num2*num1;
			}

		}
	}
	return count;
}
int GSS::TriangleCounting()
{
		int count=0;
	for(int i=0;i<tablesize;i++)
	{
		std::vector<hashvalue> node1_set;
		hashTableNode<string> *np;
		np=mapTable.table[i];
		for(;np!=NULL;np=np->next)
		{	
		unsigned int hash=np->key;
		hvinsert(hash, node1_set);
		}
		sort(node1_set.begin(), node1_set.end(), mycomp);
		for(int j=0;j<node1_set.size();j++)
		{
			unsigned int hash1=node1_set[j].key;
			int num1=node1_set[j].IDnum;
			//cout<<num1<<" first ";
			/*if (num1>1)
			{
				cout<<"collide!"<<endl;
				//cout<<"error";
			//	return -1;
			}*/ 
			std::vector<hashvalue> node2_set;
			Successor_for_Triangle(hash1,node2_set, true,hash1);
			Precursor_for_Triangle(hash1, node2_set, true, hash1);
			for(int k=0;k<node2_set.size();k++)
			{
				unsigned hash2=node2_set[k].key;
				int num2=node2_set[k].IDnum;
				//cout<<num2<<" second ";
			/*	if(num2>1)
				{
					cout<<"collide!"<<endl;
					cout<<"error";
					return -1;
				}*/ 
				std::vector<hashvalue> node3_set, precursor_set;
				Successor_for_Triangle(hash2,node3_set,true,hash2);
				Precursor_for_Triangle(hash2,node3_set,true,hash2);
				int num3=countjoin(node3_set,node2_set);
				//cout<<num3<<endl;
				if(num3<0)
				{
					cout<<"error "<<node3_set.size()<<' '<<node2_set.size()<<endl;
					return -1;
				}
				count+=num3;//*num2*num1;
			}

		}
	}
	return count;
}
