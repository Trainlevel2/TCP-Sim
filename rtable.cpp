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

bool rtable::bford(int ip_from){
	vector<int>* cvec = new vector<int>;
	int ip_to;
	int bcast=false;
	//loop through distance vector
	for(int i=0;i<(int)dvv.size();i++){
		if (!cvec->empty()){
			cvec->erase(cvec->begin(),cvec->end());
		}
		ip_to = dvv[i].ip;
		////cout << "cvec "<<ip_from<<","<<ip_to<<" reset" << endl;
		//loop through distance vector
		for(int j=0;j<(int)dvv.size();j++){
			
			int cost = getCost(ip_from,dvv[j].ip) + getCost(dvv[j].ip,ip_to);
			if (cost < 0) {
				cost = INF; 
			}
			////cout << "pushing back to cvec: " << cost << endl;
			cvec->push_back(cost);	
		}
		int m=INF;
		for(int j=0;j<(int)cvec->size();j++){
			if((*cvec)[j]<m){
				m = (*cvec)[j];
			}
		}
		if (getCost(ip_from,ip_to)!=m){
			////cout << "bford setcost to " << m << endl;
			setCost(ip_from,ip_to,m);
			bcast=true;
		}	
	}
	delete cvec;
	return bcast;
}


int rtable::getCost(int ip_from,int ip_to){
	//cout<<this->rname<<" getting cost from: "<< ip_from<<" to:"<<ip_to <<endl;
	bool f1 = false;
	for(int i=0;i<(int)dvv.size();i++){
		if(ip_from == dvv[i].ip){
			f1=true;
			bool f2 = false;
			for(int j=0;j<(int)dvv[i].e.size();j++){
				if(ip_to == dvv[i].e[j].ip){
					f2=true;
					return dvv[i].e[j].cost;
				}
			}
			if (!f2){
				//cout<<"IP"<<ip_to<<"doesnt exist in routing table"<<endl;
				return -1;
			}
		}

	}
	if (!f1){
		//cout<<"IP"<<ip_from<<"doesnt exist in routing table"<<endl;
		return -1;
	}
	return -1;
}

//assuming no duplicate entries in each vector
//assume AmB,BmA empty
void rtable::compare(vector<int>& A, vector<int>& B, vector<int>& AmB, vector<int>& BmA){
	vector<int> common;
	for(int i=0;i<(int)A.size();i++){
		for(int j=0;j<(int)B.size();j++){
			if(A[i]==B[j]){
				common.push_back(A[i]);
			}
		}
	}

	for(int i=0;i<(int)common.size();i++){
		
		for(int j=0;j<(int)A.size();j++){
			if(common[i] != A[j]){
				AmB.push_back(A[j]);
			}
		}

		for(int j=0;j<(int)B.size();j++){
			if(common[i] != B[j]){
				BmA.push_back(B[j]);
			}
		}
	}
}

bool rtable::containsIp(int ip){
	for(int i=0;i<(int)dvv.size();i++){
		if(ip==dvv[i].ip){
			return true;
		}
	}
	return false;
}


int rtable::addCost(int ip_from,int ip_to,int cost){
	//cout<<this->rname<<"adding Cost "<< cost <<" from "<<ip_from<<" to "<<ip_to<<endl;
	if(!containsIp(ip_from)){
		//cout<<"does not contain "<<ip_from<<endl;
		addip(ip_from);
	}
	if (!containsIp(ip_to)){
		//cout<<"does not contain "<<ip_to<<endl;
		addip(ip_to);
	}
	setCost(ip_from,ip_to,cost);
	return 0;
}

void rtable::print(){
	cout << endl << this->rname << " ROUTING TABLE" << endl;
	for(int i=0;i<(int)dvv.size();i++){
		dvv[i].print();
	}
	cout << endl;
}


void rtable::addip(int ip){
	//int inf = std::numeric_limits<int>::max();
	dVec ndv;
	ndv.ip = ip;
	//create a new row w/ heading ip, usual length
	if(dvv.empty()){
		//cout<<"ADDING THE FIRST IP " <<ip<<endl;
		dVec::entry nu;
		nu.cost = 0;
		nu.ip = ip;
		ndv.e.push_back(nu);
		ndv.ip = ip;
		dvv.push_back(ndv);
	}
	else{
		for(int i=0;i<(int)dvv.size();i++){
			dVec::entry nu;
			nu.cost = INF;
			nu.ip = dvv[i].ip;
			ndv.e.push_back(nu);
		}
		//append row
		dvv.push_back(ndv);
		//grow all of the rows in the table to the proper length: length+1.
		for(int i=0;i<(int)dvv.size();i++){
			if (dvv[i].ip == ip) {
				dVec::entry e;
				e.cost = 0; //cost from ip to self.
				e.ip = ip;
				dvv[i].e.push_back(e);
			}
			else {
				dVec::entry e;
				e.cost = INF; //cost from ip to others.
				e.ip = ip;
				dvv[i].e.push_back(e);
			}

		}
	}

	
}





void rtable::addHost(int ip,int host_ip){
	if (!containsIp(ip)) {
		//cout << "does not contain " << ip << endl;
		addip(ip);
	}	
	dVec* dv = getDv(ip);
	bool found=false;	
	for(int i=0;i<(int)dv->h.size();i++){
		if (dv->h[i]==host_ip){
			found=true;
		}
	}
	if (!found){
		dv->h.push_back(host_ip);		
	}
}

int rtable::update(dVec* dv){
	//int inf = std::numeric_limits<int>::max();
	//cout<<"\n\ninput dv size: "<<(int)dv->e.size()<<endl;
	if((int)dv->e.size()==0){
		cerr<<"can't update: null dvec passed into rtable::update()"<<endl;
		exit(1);
	}

	int bcast=0;
	if(dvv.empty()){
		for(int i=0;i<dv->e.size();i++){
			addCost(dv->ip,dv->e[i].ip,dv->e[i].cost);
		}	
		for(int i=0;i<dv->h.size();i++){
			addHost(dv->ip,dv->h[i]);
		}
		bcast = 1;
		
	}
	else{
		cout<<"INPUT DV"<<endl;
		dv->print();

		cout<<"INITIAL NONEMPTY"<<endl;
		print();

		vector<int> tA; //to Add
		vector<int> tB; //to Broadcast

		vector<int>* comp = new vector<int>;

		for(int i=0;i<(int)dv->e.size();i++){
			comp->push_back(dv->e[i].ip);
		}

		vector<int>* comp2 = new vector<int>;

		for(int i=0;i<(int)dvv.size();i++){
			comp2->push_back(dvv[i].ip);
		}

		compare(*comp2,*comp,tB,tA);

		
		if((tB.empty())&&(tA.empty())){
			cout<<"new router ip's not found"<<endl;
			for(int i=0;i<(int)dv->e.size();i++){
				//copy right over.
				if(getCost(dv->ip,dv->e[i].ip) != dv->e[i].cost){
					if(bcast==0){
						bcast=1;
					}
					setCost(dv->ip,dv->e[i].ip,dv->e[i].cost);
				}
			}
		}
		else{
			if (!tA.empty()){
				cout<<"routers to add found"<<endl;
				for(int i=0;i<(int)tA.size();i++){
					for(int j=0;j<(int)dv->e.size();j++){
						if (dv->e[j].ip == tA[i]){
							addCost(dv->ip,dv->e[j].ip,dv->e[j].cost);
						}
					}
				}
			}
			else if(!tB.empty()){
				cout<<"need to Broadcast knowledge of routers"<<endl;
				
			}
			bcast=1; //just propagate the thing we received
		}


		//cout<<"new router ip's not found"<<endl;
		for(int i=0;i<(int)dv->e.size();i++){
			//copy right over.
			if(getCost(dv->ip,dv->e[i].ip) != dv->e[i].cost){
				if(bcast==0){
					bcast=1;
				}
				setCost(dv->ip,dv->e[i].ip,dv->e[i].cost);
			}
		}


		cout<<"NEW ROUTERS ADDED"<<endl;
		print();
		//cout<<endl;

		
		//cout<<endl;

		
		dVec* mdv = getDv(dv->ip);

		if(mdv->h.empty()){
			for(int i=0;i<dv->h.size();i++){
				mdv->h.push_back(dv->h[i]);	
			}
			cout<<"HOSTS ADDED"<<endl;
			print();
		}
		else{

			vector<int> tB2,tA2;
			compare(mdv->h,dv->h,tB2,tA2);
			if((tB2.empty())&&(tA2.empty())){
				//they are the same, do nothing
			}
			else{
				if (!tA2.empty()){
					cout<<"hosts to add found"<<endl;
					for(int i=0;i<(int)tA2.size();i++){
						for(int j=0;j<(int)dv->h.size();j++){
							if(dv->h[j]==tA2[i]){
								addHost(dv->ip,dv->h[j]);
							}
						}
					}
				}
				if (!tB2.empty()){
					cout<<"need to broadcast knowledge of hosts"<<endl;
				}
				bcast=1;
			}
		
		}
		
		

		
		//if (isComplete()){
			if(bford(dv->ip)){
				bcast = 2; //send our dVec
				//cout<<"bellman fording"<<endl;
				print();
			}
		//}
		delete comp;
		delete comp2;

	}

	return bcast;
	
}

dVec* rtable::getDv(int ip){
	cout<<"getting dv"<<endl;
	for(int i=0;i<(int)dvv.size();i++){
		if(dvv[i].ip == ip){
			return &dvv[i];
		}
	}
	return NULL;
}


bool rtable::isComplete(){
	for(int i=0;i<(int)dvv.size();i++){
		for(int j=0;j<(int)dvv.size();j++){
			if(INF==getCost(dvv[i].ip,dvv[j].ip)){
				return false;
			}
		}
	}
	return true;
}


int rtable::setCost(int ip_from,int ip_to,int cost){
	bool f1 = false;
	for(int i=0;i<(int)dvv.size();i++){
		if(ip_from == dvv[i].ip){
			f1=true;
			bool f2 = false;
			for(int j=0;j<(int)dvv[i].e.size();j++){
				if(ip_to == dvv[i].e[j].ip){
					f2=true;
					//cout << ip_from << " to " << ip_to << " cost update: " << cost << endl;
					dvv[i].e[j].cost = cost;
					return 0;
				}
			}
			if (!f2){
				//cout<<"IP"<<ip_to<<"doesnt exist in routing table"<<endl;
				return -1;
			}
		}
	}
	if (!f1){
		//cout<<"IP"<<ip_from<<"doesnt exist in routing table"<<endl;
		return -1;
	}
	return -1;

}