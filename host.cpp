using namespace std;
#include "host.h"
#include "link.h"
#include "packet.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale> 
#include <cstdlib>
#include <queue>
#include <new>



// The packet constructor initializes the packet with set information of data and destination. 
host::host(int ip, vector<link*>* link_vector){
	ip_addr = ip;
	qVec.resize(link_vector->size());
	for(int i=0;i<qVec.size();i++){
		qVec[i].lptr = (*link_vector)[i];
	}
}

//Receives packet
packet* host::receivePacket(link* lptr){
	return lptr->currentPkt;
}

queue<packet*>* host::getQueue(link* lptr){
	for(int i=0;i<qVec.size();i++){
		if(qVec[i].lptr == lptr){
			return &(qVec[i].outQueue);
		}
	}
}

//Sends packet
void host::pushPacket(packet* pptr, link* lptr){
	getQueue(lptr)->push(pptr);
}
void host::transmitPacket(link* lptr){
	getQueue(lptr)->pop();
}

/*
packet* host::obtainPacket(){
	return NULL;
}

link* chooseLink(){
	
}
*/
