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
		cout<<ip<<" :";
		for(int j=0;j<(int)e.size();j++){
			cout<<"{"<<e[j].ip<<","<<e[j].cost<<"}";
			
		}
		cout << endl << " hosts: ";
		for (int j = 0; j < (int)h.size(); j++) {
			cout << h[j] << ", ";
		}
		cout << endl;
}
