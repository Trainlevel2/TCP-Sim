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


// The packet constructor initializes the packet with set information of data and destination. 
host::host( string name, int ip){
	ip_addr = ip;
	this->name = name;
}

//Receives packet
void host::receivePacket(link* l){
	//return link_ptr->currentPkt;
	packet* p = link_ptr->currentPkt;
	link_ptr->currentPkt = NULL;
	if (p->data > 0) {
		cout << "RECEIVED DATA, SENDING ACK" << endl;
		host* sendTo = p->src;
		packet pSend(-1, this, sendTo);
		pSend.f = p->f;
		packetVector.push_back(pSend);
		link_ptr->propagate(&packetVector[packetVector.size() - 1]);
	}
	else {
		cout << "RECEIVED ACK" << endl;
		p->f->receiveAck(p);
	}
}

queue<packet*>* host::getQueue(){
	return &outQueue;
}

void host::addLink(link* link_ptr) {
	this->link_ptr = link_ptr;
}

//Sends packet
void host::pushPacket(packet* pptr){
	getQueue()->push(pptr);
	transmitPacket();
}
void host::transmitPacket(){
	packet* p = getQueue()->front();
	getQueue()->pop();
	link_ptr->propagate(p);
}