using namespace std;

#include <iostream>
#include <fstream>
#include <sstream>
#include <locale> 
#include <cstdlib>
#include <queue>
#include <new>
#include <limits>
#include <cstdint>
#include <climits>
#include "rtable.h"
#define INF INT_MAX



void dVec::print(){
		cout<<dvv[i].ip<<" :";
		for(int j=0;j<(int)dvv[i].e.size();j++){
			cout<<"{"<<dvv[i].e[j].ip<<","<<dvv[i].e[j].cost<<"}";
			
		}
		cout << endl << " hosts: ";
		for (int j = 0; j < (int)dvv[i].h.size(); j++) {
			cout << dvv[i].h[j] << ", ";
		}
		cout << endl;
}
