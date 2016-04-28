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
	//isDefaultGateway = false;
	isCTS = false;
	routersConnected = false;
	STATE=0;
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

void router::printLinks(){
	cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" lvector: "<<endl;
	cout << "{link_id, type, ip}" << endl;
	for(int i=0;i<lVector.size();i++){
		cout<<"{"<<lVector[i].link_id<<","<<lVector[i].type<<","<<lVector[i].ip<<"},"<<endl;
	}
}


void router::inform(int n,int ip,link* link_ptr){
	cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" informing "<< n <<endl;
	if((n==1)||(n==2)){
		//if its new
		//broadcast the dvec sent to us on ALL LInks
		for(int i=0;i<lVector.size();i++){
			link* myLink_ptr = &linkVector[lVector[i].link_id];
			if ((myLink_ptr->id != link_ptr->id)&&(lVector[i].type == 1)){
				cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" propagating new dVec"<<endl;
				packet pSend(0, 0, this, this);
				pSend.isRIP = true;
				pSend.dv = *(rt.getDv(ip));
				packetVector.push_back(pSend);
				pushPacket((int)packetVector.size() - 1,link_ptr);	
			}
		}
		if(n==2){	
			for(int i=0;i<lVector.size();i++){
				cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" propagating modified dVec"<<endl;
				link* myLink_ptr = &linkVector[lVector[i].link_id];
				packet pSend(0, 0, this, this);
				pSend.isRIP = true;
				pSend.dv = *(rt.getDv(this->ip));
				packetVector.push_back(pSend);
				pushPacket((int)packetVector.size() - 1,myLink_ptr);	
			}
		}
	}
	else if(n==3){
		for(int i=0;i<lVector.size();i++){
			cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" propagating CTS"<<endl;
			if(lVector[i].type==1){
				link* myLink_ptr = &linkVector[lVector[i].link_id];
				packet pSend(0, 0, this, this);
				pSend.isCTS = true;
				packetVector.push_back(pSend);
				pushPacket((int)packetVector.size() - 1,myLink_ptr);	
			}
		}
	}
	else if(n==4){
		for(int i=0;i<lVector.size();i++){
			cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" is ready to send: propagating CTS to its HOSTS"<<endl;
			if(lVector[i].type==0){
				link* myLink_ptr = &linkVector[lVector[i].link_id];
				packet pSend(0, 0, this, this);
				pSend.isCTS = true;
				packetVector.push_back(pSend);
				cout<<"informing host connected that "<<this->ip<<" is clearToSend"<<endl;
				pushPacket((int)packetVector.size() - 1,myLink_ptr);	
			}
		}
	}
}


void router::sendCR(int i,link* link_ptr){
		packet pSend(0, 0, this, this);
		pSend.isCR = true;
		pSend.t = i;
		packetVector.push_back(pSend);
		pushPacket((int)packetVector.size() - 1,link_ptr);
}

void router::crResp(link* link_ptr,packet* p){
	cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" responding to: ";
	cout<<"CR"<<p->t<<endl;
	if(p->t==0){
		//update lVector
		lVectorUpdate(link_ptr,p);
		printLinks();
		cin.ignore();
		for(int i=0;i<lVector.size();i++){
			if(lVector[i].type != 2){
				link* lptr = &linkVector[lVector[i].link_id];
				if(lptr != link_ptr){
					sendCR(0,lptr);
				}
				else{
					sendCR(1,lptr);
				}	
			}	
		}
	}else{
		lVectorUpdate(link_ptr,p);
		STATE=1;
	}
	printLinks();
}

void router::receivePacket(link* link_ptr) {
	cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" has recieved packet:"<<endl;
	packet* p = &packetVector[link_ptr->pnum];
	int tnum = link_ptr->pnum;
	link_ptr->pnum = -1;
	if(STATE==0){ //Links Unknown
		if(p->isCR){
			crResp(link_ptr,p);
			if(discoveryComplete()){
				cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" has completed discovery:"<<endl;
				printLinks();
				cin.ignore();
				STATE=1;
				cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" initial routing table:"<<endl;
				rtInit();
				rt.print();
				cin.ignore();
				if(RIPbuf.empty()){	
					cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" empty RIPbuf:"<<endl;
					inform(1,this->ip,link_ptr);
				}
				else{
					cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" nonEmpty RIPbuf:"<<endl;
				
					int k=0;
					while(!RIPbuf.empty()){
						int n = rt.update(&RIPbuf.front());
						//get ip from RIPbuf
						int ipRet = RIPbuf.front().ip;
						RIPbuf.pop();
						if (n>k){
							k = n;
						}
						//IDEALLY: inform less
						//inform all routers
						//treat lVector[]
						for(int i=0;i<lVector.size();i++){
							link* lptr = &linkVector[lVector[i].link_id];
							if(lVector[i].type == 1){
								inform(k,ipRet,lptr);
							}
						}
					}
					
				}				
				STATE=1;
				//if no routers connected to this router
				if(!routersConnected){
					cout<<"no routers connected!"<<endl;
					if(rt.isComplete()){
						cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" routing table is complete:"<<endl;
						cout<<"final routing table:"<<endl;
						rt.print();
						STATE=2;
						inform(4,p->src->ip,link_ptr);
					}
				}
			}
		}
		else if (p->isRIP){
			RIPbuf.push(p->dv);
		}
	}
	else if(STATE==1){ //building routing table
		if(p->isRIP){
			inform(rt.update(&(p->dv)),p->src->ip,link_ptr);
		}
		else if(p->isCR){
			//we sent to a router that wasn't ready for RIP yet
			crResp(link_ptr,p);
		}
		else if(p->isCTS){
			for(int i=0;i<lVector.size();i++){
				if(lVector[i].type == 1){
					lVector[i].isCTS = true;
				}
			}
		}
		if(rt.isComplete()){
			cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" routing table is complete:"<<endl;
			cout<<"final routing table:"<<endl;
			rt.print();
			STATE=2;
			inform(3,p->src->ip,link_ptr);
		}
	}
	else if(STATE==2){ //routing table complete
		if(p->isRIP){
			inform(rt.update(&(p->dv)),p->src->ip,link_ptr);
			cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" routing table updated:"<<endl;
			rt.print();
		}
		else if(p->isCR){
			crResp(link_ptr,p);
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
		else if(p->f != nullptr){
			cout<<"data packet recieved on "<< this->name <<" choosing link"<<endl;
			link* l = chooseLink(p);
			cout<<"link "<<l->id<<" chosen"<<endl;
			if(l){
				pushPacket(tnum,l);
			}else{
				cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" chooseLink failure"<<endl;
			}
		}
		if(this->isCTS){
			inform(4,p->src->ip,link_ptr);
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
				routersConnected = true;
				cout<<"router connected!"<<endl;
			}
			else if(p->src->name[0] == 'H'){
				lVector[i].type = 0;
				lVector[i].ip = p->src->ip;
				//isDefaultGateway = true;
			}
		}
	}
}

//generate distance vector from 
dVec* router::genDv(dVec& dv){
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
	return &dv;
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

//assumes LinkVector is fully updated.
void router::rtInit(){
	dVec dv;
	cout<<"creating routing table "<<endl;
	rt.update(genDv(dv));
}