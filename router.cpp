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
#define INF std::numeric_limits<int>::max()
extern vector<link> linkVector;
extern vector<packet> packetVector;
extern vector<node> nodeVector;
extern void popTimeout(int timeoutIndex);

router::router(string name, int ip)
:node::node(name,ip){
	host_ip=-1;
	field f;
	f.link_id = -1;
	f.type = 2;
	f.ip = this->ip;
	lVector.push_back(f);
	discovered = false;
}


void router::addLink(int id) {
	field f;
	f.link_id = id;
	f.type = -1;
	f.ip = -1;
	lVector.push_back(f);
}

//get, packet on connected link. start processing.
void router::receivePacket(link* link_ptr) {
	packet* p = &packetVector[link_ptr->pnum];
	int tnum = pnum;
	link_ptr->pnum = -1;
	if(p->isRIP){
		if(!p->dv.e.empty()){
			if(rt.update(&(p->dv))==1){
				broadcast(1);
			}
		}		
		else{
			cout<<"CONNECTION packet recieved by router "<<this->name<<" ,replying"<<endl;
			if(!discovered){
				for(int i=0;i<(int)lVector.size();i++){
					if(lVector[i].link_id == link_ptr->id){
						if(p->src->name[0] == 'R'){
							lVector[i].type = 1;
							lVector[i].ip = p->src->ip;
						}
						else if(p->src->name[0] == 'H'){
							lVector[i].type = 0;
							lVector[i].ip = p->src->ip;
						}
					} 
				}
				if(discoveryComplete()){
					discovered = true;
				}
			}
			int size= 1;
			int num= -1;
			packet p(size, num, this, this);
			p.isRIP = true;
			//p.dVec = rt.getdv(this->ip);
			packetVector.push_back(p);
			pushPacket(packetVector.size()-1,link_ptr); //router shouldn't knw what link the packet came in on.
		}
	}
	else{
		link* l = chooseLink(p);
		if(l){
			pushPacket(tnum,l);
		}else{
			cout<<"chooseLink failure at"<<this->name<<endl;
		}
	}
}

bool router::discoveryComplete(){
	for(int i=0;i<(int)lVector.size();i++){
		if(lVector[i].type==-1){
			return false;
		}
	}
	return true;
}


link* router::chooseLink(packet* p){
	//int inf = std::numeric_limits<int>::max();

	//pick least-cost next step, given current packet destination
	link* min_link=NULL;
	int min_step=INF;
	for(int i=0; i<(int)rt.dvv.size();i++){
		
		if(nodeVector[rt.dvv[i].ip].name[0] == 'H'){
			if(p->dest->ip == rt.dvv[i].ip){
				//loop through neighbors. if router, cost-check.
				for(int j=0;(int)lVector.size();j++){
					if(lVector[j].type == 1){
						int step_cost = rt.getcost(this->ip,lVector[j].ip) + rt.getcost(lVector[j].ip,p->dest->ip);
						if(step_cost<min_step){
							min_link= &linkVector[lVector[j].link_id];
						}
					}
				}
				break;
			}
			
		}	
	}
	return min_link;
}

void router::bford(int ip){
	
	for(int i=0;i<rt.dvv.size();i++){
		if(rt.dvv[i].ip==ip){

			//set costs in routing table row accordingly.
			for(int j=0;j<rt.dvv[i].e.size();j++){
				




				rt.setcost(ip,dvv[i].e[j].ip,cost)
			}

		}
	}
}


void router::rtable::addip(int ip){
	//int inf = std::numeric_limits<int>::max();
	dVec ndv;
	ndv.ip = dv->ip;
	for(int i=0;i<rt.dvv.size();i++){
		entry nu;
		nu.cost = INF;
		nu.ip = rt.dvv[i].ip;
		ndv.e.push_back(nu);
	}
	rt.dvv.push_back(ndv);
	for(int i=0;i<rt.dvv.size();i++){
		rt.dvv[i].e.resize(rt.dvv.size(),INF);
	}
}



//return -1 on failure
//return cost on success
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

//return 0 on success
//return 1 on failure
int router::rtable::setcost(int ip_from,int ip_to,int cost){
	bool f1 = false;
	for(int i=0;i<dvv.size();i++){
		if(ip_from == dvv[i].ip){
			f1=true;
			bool f2 = false;
			for(int j=0;j<dvv[i].e.size();j++){
				if(ip_to == dvv[i].e[j].ip){
					f2=true;
					dvv[i].e[j].cost = cost;
					return 0;
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
bool router::rtable::isPop(){
	for(int i=0;i<dvv.size();i++){
		for(int j=0;j<dvv.size();j++){
			if(INF==getcost(dvv[i].ip,dvv[j].ip)){
				return true;
			}
		}
	}
	return false;
}

//update given distance vector.
//return 1 if broadcast is needed (this rtable contains an ip that RIP source doesn't)
//return 0 if no broadcast is needed
int router::rtable::update(dVec* dv){
	//int inf = std::numeric_limits<int>::max();
	vector<int> toAdd;
	int bcast=0;

	//make sure that dv and rtable have the same set of IPs
	vector<int> common;
	for(int i=0;i<dvv.size();i++){
		for(int j=0;j<dv->e.size();j++){
			if(dvv[i].ip == dv->e[j].ip){
				common.push_back(1);
			}
		}
	}
	if((dvv.size()==dv->e.size())&&(common.size()==dvv.size())){
		//they have the same elements
		for(int i=0;i<dv->e.size();i++){
			setcost(dv->ip,dv->e[i].ip,dv->e[i].cost);
		}
		if (isPop()){
			bford();
		}
	}else{
		for(int i=0;i<common.size();i++){
			if(bcast==0){
				for(int j=0;j<dvv[i].e.size();j++){
					if(dvv[i].e[j].ip != common[i]){
						bcast=1;
						break;
					}
				}	
			}
			for(int j=0;j<dv->e.size();j++){
				if(dv->e[j].ip != common[i]){
					toAdd.push_back(dv->e[j].ip);
				}
			}
		}
		if(!toAdd.empty()){
			for(int i=0;i<toAdd.size();i++){
				addip(toAdd[i]);
			}
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
void router::broadcast(int mode){
	if (mode==0){
		for(int i=0;i<lVector.size();i++){
			if(lVector[i].type != 2){
				link* link_ptr = &linkVector[lVector[i].link_id];
				int size= 1;
				int num= -1;
				packet p(size, num, this, this);
				p.isRIP = true;
				//p.dVec = rt.getdv(this->ip);
				packetVector.push_back(p);
				pushPacket(packetVector.size()-1,link_ptr);
			}
		}
	}else if(mode==1){
		for(int i=0;i<lVector.size();i++){
			if(lVector[i].type != 2){
				link* link_ptr = &linkVector[lVector[i].link_id];
				int size= 1;
				int num= -1;
				packet p(size, num, this, this);
				p.isRIP = true;
				p.dVec = rt.getdv(this->ip);
				packetVector.push_back(p);
				pushPacket(packetVector.size()-1,link_ptr);
			}
		}
	}
	
}

//scan links and update routing table accordingly.
//return 1 if broadcast is necessary



//assumes LinkVector is updated.
int router::scanLinks(){
	dVec* dv = new dVec();
	dv->ip = this->ip;
	int bcast=0;
	for(int i=0;i<lVector.size();i++){
		if(lVector[i].type == 1){
			link* link_ptr = &linkVector[lVector[i].link_id];
			entry e;
			e.cost = link_ptr->cost;
			e.ip = lVector[i].ip;
			dv.e.push_back(e);
		}
		else if(lVector[i].type == 0){
			//host
		}else if(lVector[i].type == -1){
			bcast = 1;
		}else if(lVector[i].type == 2){
			//self
			entry s;
			s.ip = this->ip;
			s.cost = 0;
			dv.e.push_back(s);
		}
	}
	rt.update(dv);

	delete dv;
	return bcast;
}