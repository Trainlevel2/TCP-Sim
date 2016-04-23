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

//constructor 
router::router(string name, int ip,int br)
:node(string name, int ip,int br){}



void router::addLink(int lnum) {
	lVector.push(lnum);

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

//broadcast iinitiallyu
//broadcast if there is any change in a link distance vector.
void broadcast(){
	

	if(rtable.empty()){
		for(int i=0;i<lVector.size();i++){
			link* link_ptr = &linkVector[lVector[i]];
			int size= 1;
			int num= -1;
			packet p(size,num, this, this);
			packetVector.push_back(p);
			pushPacket(packetVector.size()-1,link_ptr);
		}
	}else{
		//if there is a distance vector change
	}
	

}




