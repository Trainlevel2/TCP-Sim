using namespace std;
#include "router.h"
#include <queue>
#include <new>

// The packet constructor initializes the packet with set information of data and destination. 
router::router(int ip, vector<link*>* link_vector){
	ip_addr = ip;
	qVec.resize(link_vector->size());
	for(int i=0;i<qVec.size();i++){
		qVec[i].lptr = (*link_vector)[i];
	}
}


packet* recievePacket(link* lptr){
	return lptr->currentPkt;
}

queue<packet*>* getQueue(link* lptr){
	for(int i=0;i<qVec.size();i++){
		if(qVec[i].lptr == lptr)
		{
			return &(qVec[i].outQueue);
		}
	}
}

void pushPacket(packet* pptr, link* lptr){
	getQueue(lptr)->push(pptr);
}


void transmitPacket(link* lptr){
	getQueue(lptr)->pop();
}


link* chooseLink(packet* pptr){
	//loop through routing table
	//match pptr->dest
	//exctract appropriate link

}