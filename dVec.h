		

		



//ECE303 Project
//Original Author: Darwin Huang

#ifndef _DVEC_H
#define _DVEC_H

#include <vector>

class dVec{
	public:	
		class entry{
			public:
				int cost;
				int ip;
		};
		vector<entry> e; 
		int ip;
		vector<int> h;
		void print();
};

#endif //_DVEC_H