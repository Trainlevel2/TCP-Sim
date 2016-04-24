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
extern vector<link> linkVector;
extern vector<packet> packetVector;
extern void popTimeout(int timeoutIndex);



//dvec and rvec will be used TOGETHER as the distance vector.

//network discovery and distance vector updates will occur simultaneously.




//constructor 
router::router(string name, int ip)
:node(string name, int ip){
	host=-1;
}

void router::addLink(int id) {
	lVector.push(id);
}

//get the packet present on a connected link.
int router::receivePacket(int lnum) {
	link* lptr = &linkVector[lnum];
	packet* p = &packetVector[link_ptr->pnum];
	link_ptr->pnum = NULL;
	return pnum;
}

int chooseLink(int pnum){
//pick least-cost next step, given current packet destination

}




void router::procPacket(pnum){
	packet* p = &packetVector[link_ptr->pnum];
	if(p->isRIP){
		if(!dVec.empty()){
			
			if(!rVector.empty()){

				for(int i=0;i<p->dVec.size();i++){
					bool found=false;
					for(int j=0;j<rVector.size();j++){
						if(p->dVec[i].ip==rVector[j]){
							found=true;
							break;
						}
					}
					if(found==false){
						rtable.insert(p->src->ip,dVec); /***************************************/
					}
				}
				bool updateReq = false;
				for(int i=0;i<rVector.size();i++){
					bool found=false;
					for(int j=0;j<p->rVec.size();j++){
						if(p->rVec[i]==rVector[j]){
							found=true;
							break;
						}
					}
					if(found==false){
						updateReq=true;
					}
				}
				if(updateReq==true){
					broadcast(0);       //we don't know what link the packet came from once it is in processing. therefore, we have to broadcast to all links.
				}
			}
			else{
				rVector = p->rVec;
			}
			
		}else{
			//dVec update

		}
	}else{
		//determine forward path
	}
}

router::rtable::insert(){
	
}






















//if empty routing table, broadcast connectivity request.
//broadcast if there is any change in a link distance vector.
//0 discovery
//1 dVec update 
void broadcast(int mode){
	
	if (mode==0){
		for(int i=0;i<lVector.size();i++){
			link* link_ptr = &linkVector[lVector[i]];
			int size= 1;
			int num= -1;
			packet p(size, num, this, this);
			p->isRIP = true;
			p->rVec = rVector;
			packetVector.push_back(p);
			pushPacket(packetVector.size()-1,link_ptr);
		}
	}
	else if(mode==1){
		for(int i=0;i<lVector.size();i++){
			link* link_ptr = &linkVector[lVector[i]];
			int size= 1;
			int num= -1;
			packet p(size, num, this, this);
			p->isRIP = true;
			p->dVec = 
			packetVector.push_back(p);
			pushPacket(packetVector.size()-1,link_ptr);
		}
	}
}

//
void updateTable(){
	
	//scan all links
	//if there is a link cost change
	//update it in the routing table

	if(rtable.empty()){ 
		for(int i=0;i<lVector.size();i++){
			link* link_ptr = &linkVector[lVector[i]];
			if(this->ip == link->src->ip){
				if(link->dest->name[0] =='R'){
					rVector.push_back(link->dest->ip);

				}else if(link->dest->name[0]=='H'){
					this->host = link->dest->ip;
				}
			}
			else{
				if(link->src->name[0] =='R'){
					rVector.push_back(link->dest->ip);
				}else if(link->src->name[0]=='H'){
					this->host = link->dest->ip;
				}
			}
		}
		rVector.push_back(this->ip);
		int tSize = rVector.size();
		rtable.resize(tSize);
		for(int i=0;i<rtable.size();i++){
			rtable.at(i).resize(tSize);
		}
		for(int i=0;i<rVector.size();i++){




			rtable[i][]
		}
	}
	else{

	}
}


int getIndex(int ip){
	for(int i=0;i<rVector.size();i++)
}




void getdVec(){
	//map src and dest in the routing table by the order in which they are in the link vector.

	rtable[0][0] = 0;
	for(int i=0;i<lVector.size();i++){
		link* link_ptr = &linkVector[lVector[i]];
		rtable[0][i] = link_ptr->cost;
	}
}




