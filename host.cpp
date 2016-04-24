using namespace std;
#include "host.h"
#include "node.h"
#include "link.h"
#include "packet.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale> 
#include <cstdlib>
#include <queue>
#include <new>
extern vector<packet> packetVector;
extern vector<link> linkVector;
extern void popTimeout(int timeoutIndex);

//specify bitrate 
host::host(string name, int ip)
:node(string name, int ip){}

//Receives packet
void host::receivePacket(link* l){
	//return link_ptr->currentPkt;
	packet* p = &packetVector[link_ptr->pnum];
	int tnum = link_ptr->pnum;
	link_ptr->pnum = NULL;
	if (!p->isAck) {
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
}

void host::addLink(int lnum) {
	this->link_ptr = &linkVector[lnum];
}



//inherited function
/*
//Sends packet and creates timeout event
void host::pushPacket(int pnum){
	link_ptr->qn.push(this);
	link_ptr->qp.push(pnum);
	link_ptr->propagate();
}
*/