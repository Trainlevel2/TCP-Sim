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
#include <string>
extern vector<packet> packetVector;
extern vector<link> linkVector;
extern void popTimeout(int timeoutIndex);

//specify bitrate 
host::host(string name, int ip)
:node::node(name,ip){
	link_id=-1;
}

//Receives packet
void host::receivePacket(link* link_ptr){
	link* link_ptr = &linkVector[link_id];
	packet* p = &packetVector[link_ptr->pnum];
	int tnum = link_ptr->pnum;
	link_ptr->pnum = -1;
	int snum;
	if (!p->isRIP){
		if (!p->isAck) {
		cout <<this->name<< " RECEIVED DATA, SENDING ACK" << endl;
		packet pSend(0, p->num, this, p->src);
		pSend.f = p->f;
		pSend.isAck = true;
		packetVector.push_back(pSend);
		snum = packetVector.size() - 1;
		pushPacket(snum,link_ptr);
		}
		else {
			cout <<this->name<< " RECEIVED ACK" << endl;
			int pnum = p->num;
			p->f->receiveAck(tnum);
			//Delete associated timeout
			popTimeout(pnum); //the 0 is a stand-in for the timeout index, since there will only be one timeout at a time for now
		}
	}
	else{
		cout<<this->name<< " RECEIVED RIP,SENDING RIPly"<<endl;
		packet pSend(0, p->num, this, this);
		pSend.isRIP = true;
		pSend.isAck = false;
		packetVector.push_back(pSend);
		snum = packetVector.size() - 1;
		pushPacket(snum,link_ptr);
	}
}

void host::addLink(int lnum) {
	this->link_id = lnum;
}

void node::pushPacket(int pnum,link* link_ptr) {
	link_ptr->qn.push(this);
	link_ptr->qp.push(pnum);
	link_ptr->propagate();
}