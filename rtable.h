#ifndef _RTABLE_H
#define _RTABLE_H
#include "dVec.h"
#include <vector>


using namespace std;
class rtable{
	public:
		string rname;
		vector<dVec> dvv;
		int setCost(int ip_from,int ip_to,int cost);
		bool bford(int ip_from);
		void addip(int ip);
		int getCost(int ip_from,int ip_to);
		bool isComplete();
		int update(dVec* dv);
		dVec* getDv(int ip);
		int addCost(int ip_from,int ip_to,int cost);
		void addHost(int ip,int host_ip);
		void print();
		void compare(vector<int>& A, vector<int>& B, vector<int>& AmB, vector<int>& BmA);
		bool containsIp(int ip);

};

#endif //_ROUTER_H