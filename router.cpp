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
	if (STATE==0){ 		//0 = unknown neighbors, empty routing table.
		if(p->isRIP){
			cout<<"CONNECTION packet recieved by router "<<this->name<<" ,replying"<<endl;
			//update lVector info
			for(int i=0;i<(int)lVector.size();i++){
				if(lVector[i].link_id == link_ptr->id){
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
			}
			//broadcast connectionReq on ALL links
			broadcast();
			STATE=1;
		}else{
			//we can't forward
		}
	}
	else if(STATE==1){ 	//1 = broadcast sent, waiting on replies.
		//update lVector info, based on packet sent.
		if(p->isRIP){
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
		}
		else{
			//we can't forward
		}
		
		if(discoveryComplete()){
			STATE=2;
		}
		//make distance vector from lVector
		//generate preliminary routing table
		rt.init();
	}
	else if(STATE==2){ 	//2 = setting up routing table.
		if(p->isRIP){
			if(!p->dv.e.empty()){
				cout<<"DVEC packet recieved by router "<<this->name<<" ,replying"<<endl;
				int n1 = rt.update(&(p->dv));
				if(n==1){		//broadcast dvec sent to us.
					sendDVec(p->dv.ip);
				}else if(n==2){	//broadcast dvec sent to us AND our dvec.
					sendDVec(p->dv.ip);
					sendDVec(this->ip);
				}
			}
		}
		else{
			//we still can't forward
		}
		if(rt.isComplete()){
			STATE++;

		}
	}
	else if (STATE==3){
		if(p->isRIP){
			if(!p->dv.e.empty()){
				cout<<"DVEC packet recieved by router "<<this->name<<" ,replying"<<endl;
				int n2 = rt.update(&(p->dv));
				if(n==1){		//broadcast dvec sent to us.
					sendDVec(p->dv.ip);
				}else if(n==2){	//broadcast dvec sent to us AND our dvec.
					sendDVec(p->dv.ip);
					sendDVec(this->ip);
				}
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


//return true if dvec for ip_from needs to be broadcasted
//return false if no braodcast required
bool router::rtable::bford(int ip_from){
	vector<int>* cvec = new vector<int>;
	int ip_to;
	int bcast=false;
	for(int i=0;i<dvv.size();i++){
		if (!cvec.empty()){
			cvec->erase(cvec->begin(),cvec->end());
		}
		ip_to = dvv[i].ip;
		for(int j=0;j<dvv.size();j++){
			int cost = getcost(ip_from,dvv[j].ip) + getcost(dvv[j].ip,ip_to);
			cvec->push_back();	
		}
		int m=INF;
		for(int j=0;j<cvec->size();j++){
			if((*cvec)[j]<m){
				m = (*cvec)[j];
			}
		}
		if (getcost(ip_from,ip_to)!=m){
			rt.setcost(ip_from,ip_to,m);
			bcast=true;
		}	
	}
	return bcast;
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

bool router::rtable::isComplete(){
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
//return 1 if we need to broadcast the dvec sent to us.
//return 2 if we need to broadcast the dvec sent to us AND our distance vector(updated by bellman ford).
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
			if(getcost(dv->ip,dv->e[i].ip) != dv->e[i].cost){
				if(bcast==0){
					bcast=1;
				}
				setcost(dv->ip,dv->e[i].ip,dv->e[i].cost);
			}
		}
		if (isComplete()){
			if(bford(dv->ip)){
				bcast = 2;
			}
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






void router::sendDVec(int ip_from){
	for(int i=0;i<lVector.size();i++){
		if(lVector[i].type != 2){
			link* link_ptr = &linkVector[lVector[i].link_id];
			int size= 1;
			int num= -1;
			packet p(size, num, this, this);
			p.isRIP = true;
			p.dVec = rt.getdv(ip_from);
			packetVector.push_back(p);
			pushPacket(packetVector.size()-1,link_ptr);
		}
	}
}

void router::broadcast(){
	for(int i=0;i<lVector.size();i++){
		if(lVector[i].type != 2){
			link* link_ptr = &linkVector[lVector[i].link_id];
			int size= 1;
			int num= -1; //connectionReq packet
			packet p(size, num, this, this);
			p.isRIP = true;
			//p.dVec = rt.getdv(this->ip);
			packetVector.push_back(p);
			pushPacket(packetVector.size()-1,link_ptr);
		}
	}	
}

//if there's are hosts connected 2u, broadcast cleartosend.
void router::clearToSend(){
	for(int i=0;i<(int)lVector.size();i++){
		if(lVector[i].type==0){
			link* link_ptr = &linkVector[lVector[i].link_id];
			int size= 1;
			int num= -2; //clearToSend packet
			packet p(size, num, this, this);
			p.isRIP = true;
			//p.dVec = rt.getdv(this->ip);
			packetVector.push_back(p);
			pushPacket(packetVector.size()-1,link_ptr);		
		}
	}
}

//scan links and update routing table accordingly.
//return 1 if broadcast is necessary



//assumes LinkVector is fully updated.
void router::rtable::init(){
	if(dvv.empty()){
	dVec* dv = new dVec();
	dv->ip = ip;
	for(int i=0;i<lVector.size();i++){
		if(lVector[i].type == 1){ //router
			link* link_ptr = &linkVector[lVector[i].link_id];
			entry ent; 
			ent.cost = link_ptr->cost;
			ent.ip = lVector[i].ip;
			dv.e.push_back(ent);
		}
		else if(lVector[i].type == 0){ //host
			//don't add
		}else if(lVector[i].type == -1){ //unknown
			//shouldn't occur
		}else if(lVector[i].type == 2){ //self
			entry s;
			s.ip = this->ip;
			s.cost = 0;
			dv.e.push_back(s);
		}
	}
	rt.update(dv);
	delete dv;
	}else{
		cout<<"aint empty not updating"<<endl;
	}
}