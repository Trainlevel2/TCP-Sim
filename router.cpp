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
	this->ip = 1000 + ip;
	//host_ip=-1;		
	field f;
	f.link_id = -1;	//whatever
	f.type = 2;		//self
	f.ip = this->ip;
	lVector.push_back(f);
	//discovered = false;
	rt.rname = this->name;
	isDefaultGateway = false;
	isCTS = false;
	//test code
	//rtHardCode();

}

void router::addLink(int id) {
	cout<<"adding link to router"<<endl;
		field f;
		f.link_id = id;
		f.type = -1; 	//unknown
		f.ip = -1;		//unknown
		f.isCTS = false;
		lVector.push_back(f);
}
/*
void router::addLink(int id) {
	cout<<"adding link to router"<<endl;
	field f;
	f.link_id = id;
	link* link_ptr = &linkVector[id];
	if (link_ptr->src->name == this->name){
		f.ip = link_ptr->dest->ip;
		if(link_ptr->dest->name[0] == 'H'){
			f.type = 0;
		}
		else if(link_ptr->dest->name[0] == 'R'){
			f.type = 1;
		}	
	}
	else if(link_ptr->dest->name == this->name){
		f.ip = link_ptr->src->ip;
		if(link_ptr->src->name[0] == 'H'){
			f.type = 0;
		}
		else if(link_ptr->src->name[0] == 'R'){
			f.type = 1;
		}	
	}
	lVector.push_back(f);
	printLinks();
}

*/

void router::printLinks(){
	cout<<this->name<<" lvector: "<<endl;
	cout << "{link_id, type, ip}" << endl;
	for(int i=0;i<lVector.size();i++){
		cout<<"{"<<lVector[i].link_id<<","<<lVector[i].type<<","<<lVector[i].ip<<"},"<<endl;
	}
}

/*
//for all links in the network. addCost.
void router::rtHardCode(){

	for(int i=0;i<(int)linkVector.size();i++){
		if((linkVector[i].src->name[0]=='H')&&(linkVector[i].dest->name[0]=='R')){
			rt.addHost(linkVector[i].dest->ip,linkVector[i].src->ip);
			cout << "added host " << linkVector[i].src->name << endl;
			rt.print();
		}
		else if((linkVector[i].dest->name[0]=='H')&&(linkVector[i].src->name[0]=='R')){
			rt.addHost(linkVector[i].src->ip,linkVector[i].dest->ip);
			cout << "added host " << linkVector[i].dest->name << endl;
			rt.print();
			cout << "getCost from "<<1000<<" to "<<1002<<" demo: " <<rt.getCost(1000,1002)<< endl;

		}
		else if ((linkVector[i].dest->name[0] == 'R') && (linkVector[i].src->name[0] == 'R')) {
			rt.addCost(linkVector[i].src->ip, linkVector[i].dest->ip, linkVector[i].cost);
			rt.addCost(linkVector[i].dest->ip, linkVector[i].src->ip, linkVector[i].cost);
			rt.print();
		}
	}
	for (int i = 0; i < (int)rt.dvv.size(); i++) {
		rt.bford(rt.dvv[i].ip);
	}
	//rt.addCost(ip, ip, 0);
	STATE=3;

	cout<<"after hardcoding"<<endl;
	rt.print();
	cin.ignore();
}

*/







/*
	Router States:

	0 = Links Unknown
	Initial state.
	
	If a router in this state receives a CRO, it sets its lVector, 
	sets STATE to discoveryComplete, and replies on that link with a CR1 packet.

	If a router in this state receives a CR1, it sets its lVector,
	and sets STATE to discoveryComplete.
	
	If a router in this state receives a RIP, it will reply with a CR0.

	if the state changes to 1, run an initialization of the routing table.

	

	1 = Links Known



*/

void router::inform(int n,packet* p){
	if((n==1)||(n==2)){
		//if its new
		//broadcast the dvec sent to us on ALL LInks
		for(int i=0;i<lVector.size();i++){
			link* myLink_ptr = &linkVector[lVector[i].link_ptr];
			if (myLink_ptr->id != link_ptr->id){
				//send the dvec SENT TO US
				packet pSend(0, 0, this, this);
				pSend.isRIP = true;
				pSend.dv = *(rt.getDv(p->src->ip));
				packetVector.push_back(pSend);
				snum = (int)packetVector.size() - 1;
				pushPacket(snum,link_ptr);	
			}
		}
		if(n==2){	
			for(int i=0;i<lVector.size();i++){
				//send our dvec on all inks
				link* myLink_ptr = &linkVector[lVector[i].link_ptr];
				packet pSend(0, p->num, this, p->src);
				pSend.isRIP = true;
				pSend.dv = *(rt.getDv(this->ip));
				packetVector.push_back(pSend);
				snum = (int)packetVector.size() - 1;
				pushPacket(snum,myLink_ptr);	
			}
		}
	}
	else if(n==3){
		for(int i=0;i<lVector.size();i++){
			//send CTS to all routers
			if(lVector[i].type==1){
				link* myLink_ptr = &linkVector[lVector[i].link_ptr];
				packet pSend(0, p->num, this, p->src);
				pSend.isCTS = true;
				packetVector.push_back(pSend);
				snum = (int)packetVector.size() - 1;
				pushPacket(snum,myLink_ptr);	
			}
		}
	}
	else if(n==4){
		for(int i=0;i<lVector.size();i++){
			//send CTS to all hosts
			if(lVector[i].type==0){
				link* myLink_ptr = &linkVector[lVector[i].link_ptr];
				packet pSend(0, p->num, this, p->src);
				pSend.isCTS = true;
				packetVector.push_back(pSend);
				snum = (int)packetVector.size() - 1;
				cout<<"informing host connected that "<<this->ip<<" is clearToSend"
				pushPacket(snum,myLink_ptr);	
			}
		}
	}
}


void router::crResp(int i){
	if(i==0){
		//update lVector
		lVectorUpdate(link_ptr,p);
		packet pSend(0, 0, this, this);
		pSend.isCR = true;
		pSend.t = 1;
		packetVector.push_back(pSend);
		snum = (int)packetVector.size() - 1;
		pushPacket(snum,link_ptr);
	}else{
		lVectorUpdate(link_ptr,p);
		STATE=1;
	}
	printLinks();
}

void router::receivePacket(link* link_ptr) {
	packet* p = &packetVector[link_ptr->pnum];
	int tnum = link_ptr->pnum;
	link_ptr->pnum = -1;
	if(STATE==0){ //Links Unknown
		if(p->isCR){
			crResp(p->t);
			if(discoveryComplete()){
				STATE=1;
				rtInit();
				int k=0;
				for(int i=0;i<RIPbuf.size();i++){
					int n = rt.update(RIPbuf.front());
					if (n>k){
						k = n;
					}
				}
				inform(k,p);
			}
		}
		else if (p->isRIP){
			RIPbuf.push(p->dv);
		}
	}
	else if(STATE==1){ //building routing table
		if(p->isRIP){
			inform(rt.update(&(p->dv)),p);
		}
		else if(p->isCR){
			//we sent to a router that wasn't ready for RIP yet
			crResp(p->t);
		}
		else if(p->isCTS){
			for(int i=0;i<lVector.size();i++){
				if(lVector[i].type == 1){
					lVector[i].isCTS = true;
				}
			}
		}
		if(rt.isComplete()){
			STATE=2;
			inform(3,p);
		}
	}
	else if(STATE==2){ //routing table complete
		if(p->isRIP){
			inform(rt.update(&(p->dv)),p);
		}
		else if(p->isCR){
			crResp(p->t);
		}
		else if(p->isCTS){
			cout<<"clearToSend packet recieved on"<<this->name<<endl;
			for(int i=0;i<lVector.size();i++){
				if(lVector[i].type == 1){
					lVector[i].isCTS = true;
				}
			}
			if(!this->isCTS){
				isCTS = clearToSend();
			}
		}
		else if((!p->isRIP)&&(!p->isCR)&&(!p->isCTS)){
			cout<<"data packet recieved on "<< this->name <<" choosing link"<<endl;
			link* l = chooseLink(p);
			cout<<"link "<<l->id<<" chosen"<<endl;
			if(l){
				pushPacket(tnum,l);
			}else{
				cout<<this->name<<" chooseLink failure"<<endl;
			}
		}
		if(this->isCTS){
			inform(4,p);
		}
	}
}

bool router::clearToSend(){
	for(int i=0;i<lVector.size();i++){
		if((lVector[i].type==1)&&(!lVector[i].isCTS)){
			return false;
		}
	}
	return true;
}

void router::lVectorUpdate(link* link_ptr,packet* p){
	for(int i=0;i<lVector.size();i++){
		if(lVector[i].link_id == link_ptr->id){
			if(p->src->name[0] == 'R'){
				lVector[i].type = 1;
				lVector[i].ip = p->src->ip;
			}
			else if(p->src->name[0] == 'H'){
				lVector[i].type = 0;
				lVector[i].ip = p->src->ip;
				isDefaultGateway = true;
			}
		}
	}
}

//generate distance vector from 
dVec* genDv(dVec& dv){
	dv.ip = -1;
	for(int i=0;i<(int)lVector.size();i++){
		if(lVector[i].type == 1){ //router
			link* link_ptr = &linkVector[lVector[i].link_id];
			dVec::entry ent; 
			ent.cost = link_ptr->cost;
			ent.ip = lVector[i].ip;
			dv.e.push_back(ent);
		}
		else if(lVector[i].type == 0){ //host
			dv.h.push_back(lVector[i].ip);
		}else if(lVector[i].type == -1){ //unknown
			//shouldn't occur
		}else if(lVector[i].type == 2){ //self
			dVec::entry s;
			s.ip = lVector[i].ip;
			s.cost = 0;
			dv.ip=lVector[i].ip;
			dv.e.push_back(s);
		}
	}
}








/*
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
			cout<<"data packet recieved on "<< this->name <<" choosing link"<<endl;
			link* l = chooseLink(p);
			cout<<"link "<<l->id<<" chosen"<<endl;
			if(l){
				pushPacket(tnum,l);
			}else{
				cout<<this->name<<"chooseLink failure"<<endl;
			}
		}
	}
}

*/


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
				for(int k=0; k < (int)lVector.size();k++){
					if(lVector[k].type == 1){
						int step_cost = rt.getCost(this->ip,lVector[k].ip) + rt.getCost(lVector[k].ip,rt.dvv[i].ip);
						if (step_cost < 0) {
							step_cost = INF;
						}
						if(step_cost<min_step){
							min_link= &linkVector[lVector[k].link_id];
							min_step = step_cost;
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
			dVec* dv = rt.getDv(ip_from);
			packetVector.push_back(p);
			pushPacket(packetVector.size()-1,link_ptr);
		}
	}
}
/*
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
*/

//if there's are hosts connected 2u, broadcast cleartosend.
/*
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
*/
//scan links and update routing table accordingly.
//return 1 if broadcast is necessary



//assumes LinkVector is fully updated.
void router::rtInit(){
	dVec* dv = new dVec();
	genDv(*dv);
	rt.update(dv);
	delete dv;
}