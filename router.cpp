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
extern vector<packet> packetVector;
extern void popTimeout(int timeoutIndex);

//constructor 
router::router( string name, int ip,int br){
	ip_addr = ip;
	this->name = name;
	this->br = br;
}


void router::addLink(link* l) {
	qVec.resize(qVec.size()+1);
	qVec[qVec.size() - 1].lptr = l;
}

//get the packet present on a connected link.
void router::receivePacket(link* lptr) {
	for(int i=0;i<lptr->{

	}




	//return lptr->currentPkt;
	/*
	packet* p = &packetVector[link_ptr->pnum];
	int tnum = link_ptr->pnum;
	link_ptr->pnum = NULL;
	if (p->data > 0) {
		//cout << "RECEIVED DATA, SENDING ACK" << endl;
		packet pSend(0, p->num, this, p->src);
		pSend.f = p->f;
		packetVector.push_back(pSend);
		link_ptr->qp.push(packetVector.size() - 1);
		link_ptr->qn.push(this);
		link_ptr->propagate();
		//link_ptr->propagate(&packetVector[packetVector.size() - 1]);
	}
	else {
		//cout << "RECEIVED ACK" << endl;
		int pnum = p->num;
		p->f->receiveAck(tnum);
		//Delete associated timeout
		popTimeout(pnum); //the 0 is a stand-in for the timeout index, since there will only be one timeout at a time for now
	}
	*/
	
}

//get the output queue which corresponds to a connected link.
queue<packet*>* router::getQueue(link* lptr){
	for(int i=0;i<qVec.size();i++){
		if(qVec[i].lptr == lptr){
			return &(qVec[i].outQQ);
		}
	}
}


//push packet to router output queue.
//if the queue is full, the packet is lost.
//returns 0 if successful,
//returns 1 on packet loss.
int router::pushPacket(packet* pptr, link* lptr){
	queue<packet*>* Q = getQueue(lptr);
	if(Q->size() < maxSize){
		Q->push(pptr);
		return 0;
	}
	else{
		delete pptr;
		return 1;
	}
}

/*
void router::pushPacket(int pnum){
	link_ptr->qn.push(this);
	link_ptr->qp.push(pnum);
	link_ptr->propagate();
}
*/

//transmit packet to a certain outbound link.
void router::transmitPacket(link* lptr){
	queue<packet*>* Q = getQueue(lptr);
	//lptr->currentPkt = Q->front();
	
	//makes lptr's current pkt = Q->front() AFTER propagation delay
	//lptr->propagate(Q->front()); 
	Q->pop();
}

//use routing table to determine packet's outbound link
link* router::chooseLink(){
	//ROUTING TABLE 
	//match pptr->dest
	//exctract appropriate link
	return NULL;
}

router::outQueue::outQueue()
{
}
