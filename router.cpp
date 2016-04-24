using namespace std;
#include "host.h"
#include "node.h"
#include "link.h"
#include "packet.h"
#include "router.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale> 
#include <cstdlib>
#include <queue>
#include <new>
#include <limits>
extern vector<link> linkVector;
extern vector<packet> packetVector;
extern void popTimeout(int timeoutIndex);



router::router(string name, int ip)
:node(string name, int ip){
	host_id=-1;
}


void router::addLink(int id) {
	field f;
	f.link_id = id;
	f.type = -1;
	f.ip = -1;
	lVector.push(f);
}

//get, packet on connected link. start processing.
void router::receivePacket(link* link_ptr) {
	packet* p = &packetVector[link_ptr->pnum];
	link_ptr->pnum = NULL;
	if(p->isRIP){
		for(int i=0;i<lVector.size();i++){
			if(lVector[i].id == link_ptr->id){
				if(p->src->name[0] == 'R'){
					lVector[i].type = 1;
					lVector[i].ip = p->src->ip;
				}else if(p->src->name[0] == 'H'){
					lVector[i].type = 0;
					lVector[i].ip = p->src->ip;
				}
			} 
		}
		if(!p->dv.empty()){
			if(rt.update(&(p->dv))==1){
				broadcast();
			}
		}		
		else{
			cout<<"error: empty RIP packet"<<endl;
		}
	}
	else{
		pushPacket(p->)
	}
}

//
int chooseLink(int pnum){
//pick least-cost next step, given current packet destination

}

void router::rtable::add(int ip){
	int inf = std::numeric_limits<int>::max();
	dVec ndv;
	ndv.ip = dv->ip;
	for(int i=0;i<rt.dvv.size();i++){
		entry nu;
		nu.cost = inf;
		nu.ip = rt.dvv[i].ip;
		ndv.e.push_back(nu);
	}
	rt.dvv.push_back(ndv);
	for(int i=0;i<rt.dvv.size();i++){
		rt.dvv[i].e.resize(rt.dvv.size(),inf);
	}
}

void router::rtable::bford(){

}

int router::rtable::getcost(int ip_from,int ip_to){
	bool f1 = false;
	for(int i=0;i<dvv.size();i++){
		if(ip_from == dvv[i].ip){
			f1=true;
			bool f2 = false;
			for(int j=0;j<dvv[i].e.size();j++){
				if(ip_to == dvv[i].e[j].ip){
					f2=true;
					return dvv[i].e[j].cost;
				}
			}
			if (!f2){
				cout<<"IP"<<ip_to<<"doesnt exist in routing table"<<endl;
				return -1;
			}
		}
	}
	if (!f1){
		cout<<"IP"<<ip_from<<"doesnt exist in routing table"<<endl;
		return -1;
	}
}

//update given distance vector.
//return 1 if broadcast is needed (this rtable contains an ip that RIP source doesn't)
//return 0 if no broadcast is needed
int router::rtable::update(dVec* dv){
	vector<int> toAdd;
	int bcast=0;
	for(int i=0;i<dvv.size();i++){
		bool match = false;
		if(dv->ip == dvv[i].ip){
			match = true;
			vector<int> common;
			//if ip match, update cost
			for(int j=0;j<dvv[i].e.size();j++){
				for(int k=0;k<dv->e.size();k++){
					if(dvv[i].e[j].ip == dv->e[k].ip){
						common.push_back(dv->e[k].ip);
						if(dvv[i].e[j].cost > dv->e[k].cost){
							dvv[i].e[j].cost = dv->e[k].cost;
						}
					}
				}
			}
			for(int j=0;j<common.size();j++){
				for(int k=0;k<dvv[i].e.size();k++){
					if(dvv[i].e[k].ip != common[j]){
						bcast=1;
						break;
					}
				}
				for(int k=0;k<dv->e.size();k++){
					if(dv->e[k].ip != common[j]){
						toAdd.push_back(dv->e[k].ip);
					}
				}
			}
		}
		break;
	}
	if (match == false){
		add(dv->ip);
	}
	if(!toAdd.empty()){
		for(int i=0;i<toAdd.size();i++){
			add(toAdd[i]);
		}
	}
	return bcast;
}

dVec* router::rtable::getdv(int ip){
	for(int i=0;i<dvv.size();i++){
		if(dvv[i].ip == ip){
			return &dvv[i];
		}
	}
}



//if empty routing table, broadcast connectivity request.
//broadcast if there is any change in a link distance vector.
//0 discovery
//1 dVec update 
void router::broadcast(){
	for(int i=0;i<lVector.size();i++){
		link* link_ptr = &linkVector[lVector[i].id];
		int size= 1;
		int num= -1;
		packet p(size, num, this, this);
		p->isRIP = true;
		p->dVec = rt.getdv(this->ip);
		packetVector.push_back(p);
		pushPacket(packetVector.size()-1,link_ptr);
	}
}

//scan links and update routing table accordingly.
//return 1 if broadcast is necessary
int router::scanLinks(){
	dVec* dv = new dVec();
	dv->ip = this->ip;
	int bcast=0;
	for(int i=0;i<lVector.size();i++){
		if(lVector[i].type == 1){
			link* link_ptr = &linkVector[lVector[i].id];
			entry e;
			e.cost = link_ptr->cost;
			e.ip = lVector[i].ip;
			dv.e.push_back(e);
			rt.update(dv);
		}
		else if(lVector[i].type == 0){
			//host
		}else if(lVector[i].type == -1){
			bcast = 1;
		}
	}

	delete dv;
	return bcast;
}
