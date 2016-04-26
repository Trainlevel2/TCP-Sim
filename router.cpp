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
extern vector<router> routerVector;
//extern vector<node> nodeVector;
extern void popTimeout(int timeoutIndex);

router::router(string name, int ip)
:node::node(name,ip){
	//host_ip=-1;		
	field f;
	f.link_id = -1;	//whatever
	f.type = 2;		//self
	f.ip = this->ip;
	lVector.push_back(f);
	//discovered = false;
	rt.rname = this->name;
	//test code
	rtHardCode();


}


void router::addLink(int id) {
	cout<<"adding link to router"<<endl;
	/*
		field f;
		f.link_id = id;
		f.type = -1; 	//unknown
		f.ip = -1;		//unknown
		lVector.push_back(f);

	*/

	field f;
	f.link_id = id;
	link* link_ptr = &linkVector[id];
	if (link_ptr->src->ip == this->ip){
		f.ip = link_ptr->dest->ip;
		if(link_ptr->dest->name[0] == 'H'){
			f.type = 1;
		}
		else if(link_ptr->dest->name[0] == 'R'){
			f.type = 0;
		}	
	}
	else if(link_ptr->dest->ip == this->ip){
		f.ip = link_ptr->src->ip;
		if(link_ptr->src->name[0] == 'H'){
			f.type = 1;
		}
		else if(link_ptr->src->name[0] == 'R'){
			f.type = 0;
		}	
	}
	lVector.push_back(f);
	printLinks();
}

void router::printLinks(){
	cout<<this->name<<" lvector: "<<endl;
	for(int i=0;i<lVector.size();i++){
		cout<<"{"<<lVector[i].link_id<<","<<lVector[i].type<<","<<lVector[i].ip<<"},"<<endl;
	}
}


//for all links in the network. addCost.
void router::rtHardCode(){

	for(int i=0;i<(int)linkVector.size();i++){
		rt.addCost(linkVector[i].src->ip,linkVector[i].dest->ip,linkVector[i].cost);
		if((linkVector[i].src->name[0]=='H')&&(linkVector[i].dest->name[0]=='R')){
			rt.addHost(linkVector[i].dest->ip,linkVector[i].src->ip);
		}
		else if((linkVector[i].dest->name[0]=='H')&&(linkVector[i].src->name[0]=='R')){
			rt.addHost(linkVector[i].src->ip,linkVector[i].dest->ip);
		}
	}
	STATE=3;

	cout<<"after hardcoding"<<endl;
	rt.print();
	cin.ignore();
}




//get, packet on connected link. start processing.
void router::receivePacket(link* link_ptr) {
	packet* p = &packetVector[link_ptr->pnum];
	int tnum = link_ptr->pnum;
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
		rtInit();
	}
	else if(STATE==2){ //2 = setting up routing table.
		if(p->isRIP){
			if(!p->dv.e.empty()){
				cout<<"DVEC packet recieved by router "<<this->name<<" ,replying"<<endl;
				int n = rt.update(&(p->dv));
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
	else if (STATE==3){	//3 = routing table done. listening for dvec updates.
		if(p->isRIP){
			if(!p->dv.e.empty()){
				cout<<"DVEC packet recieved by router "<<this->name<<" ,replying"<<endl;
				int n = rt.update(&(p->dv));
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
				cout<<this->name<<"chooseLink failure"<<endl;
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
	//IF DEST CORRESPONDS TO CONNECTED HOST
	for(int i=0; i<(int)lVector.size();i++){
		if((lVector[i].type == 0)&&(lVector[i].ip == p->dest->ip)){
			return &linkVector[lVector[i].link_id];

		}
	}
	//ELSE PICK LEAST-COST NEXT STEP, GIVEN DESTINATION IP
	link* min_link=NULL;
	int min_step=INF;
	//if the destination of the packet corresponds to a default gateway router of any router in the table
	//determine the least-cost path to that router


	for(int i=0;i<(int)rt.dvv.size();i++){ 
		for(int j=0;j<(int)rt.dvv[i].h.size();j++){
			if(rt.dvv[i].h[j] == p->dest->ip){
				//therefore we want to get to rt.dvv[i].ip
				for(int j=0;(int)lVector.size();j++){
					if(lVector[j].type == 1){
						int step_cost = rt.getCost(this->ip,lVector[j].ip) + rt.getCost(lVector[j].ip,rt.dvv[i].ip);
						if(step_cost<min_step){
							min_link= &linkVector[lVector[j].link_id];
						}
					}
				}
			} 
		}		
	}
	return min_link;
}


void router::sendDVec(int ip_from){
	for(int i=0;i<(int)lVector.size();i++){
		if(lVector[i].type != 2){
			link* link_ptr = &linkVector[lVector[i].link_id];
			int size= 1;
			int num= -1;
			packet p(size, num, this, this);
			p.isRIP = true;
			rt.getDv(ip_from,p.dv);
			packetVector.push_back(p);
			pushPacket(packetVector.size()-1,link_ptr);
		}
	}
}

void router::broadcast(){
	for(int i=0;i<(int)lVector.size();i++){
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
void router::rtInit(){
	dVec* dv = new dVec();
	dv->ip = -1;
	for(int i=0;i<(int)lVector.size();i++){
		if(lVector[i].type == 1){ //router
			link* link_ptr = &linkVector[lVector[i].link_id];
			dVec::entry ent; 
			ent.cost = link_ptr->cost;
			ent.ip = lVector[i].ip;
			dv->e.push_back(ent);
		}
		else if(lVector[i].type == 0){ //host
			dv->h.push_back(lVector[i].ip);
		}else if(lVector[i].type == -1){ //unknown
			//shouldn't occur
		}else if(lVector[i].type == 2){ //self
			dVec::entry s;
			s.ip = lVector[i].ip;
			s.cost = 0;
			if(dv->ip==-1){
				dv->ip=lVector[i].ip;
			}
			dv->e.push_back(s);
		}
	}
	rt.update(dv);
	delete dv;
	
}
