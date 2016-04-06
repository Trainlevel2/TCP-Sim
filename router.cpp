//ECE303 Project
//Original Author: Kirk Cameron

using namespace std;
#include "router.h"
#include <queue>
#include <new>
//constructor 
router::router(int ip, vector<link*>* link_vector){
	ip_addr = ip;
	qVec.resize(link_vector->size());
	for(int i=0;i<qVec.size();i++){
		qVec[i].lptr = (*link_vector)[i];
	}
	maxSize = 20;
}

//get the packet present on a connected link.
packet* recievePacket(link* lptr){
	return lptr->currentPkt;
}

//get the output queue which corresponds to a connected link.
queue<packet*>* getQueue(link* lptr){
	for(int i=0;i<qVec.size();i++){
		if(qVec[i].lptr == lptr)
		{
			return &(qVec[i].outQueue);
		}
	}
}

//push packet to router output queue.
//if the queue is full, the packet is lost.
//returns 0 if successful,
//returns 1 on packet loss.
int pushPacket(packet* pptr, link* lptr){
	queue<packet*>* Q = getQueue(lptr);
	if(Q->size() < maxSize)
	{
		Q->push(pptr);
		return 0;
	}
	else
	{
		delete pptr;
		return 1;
	}
}

//transmit packet to a certain outbound link.
void transmitPacket(link* lptr){
	queue<packet*>* Q = getQueue(lptr);
	//lptr->currentPkt = Q->front();
	
	//better:
	//makes lptr's current pkt = Q->front() AFTER propagation delay
	lptr->propagate(Q->front()); 
	Q->pop();
}

//use routing table to determine packet's outbound link
link* chooseLink(packet* pptr){
	//loop through routing table
	//match pptr->dest
	//exctract appropriate link
}