#include <iostream>
#include <sstream>
using namespace std;
#include "flow.h"
#include "host.h"
#include "link.h"

extern vector<packet> packetVector;
extern vector<link> linkVector;
extern void pushEvent(string e, int elapseTime);

// The packet constructor initializes the packet with set information of data and destination. 
flow::flow(host* source, host* dest, int data, int id) {
	this->source = source;
	this->dest = dest;
	this->data = data;
	this->id = id;
	state = 0;
}

//return 0 if flow startedd successfully.
//return 1 if flow uninitialized.
int flow::startFlow() {
	cout << "FLOW: "<<this->id<<" START" << endl;
	lastSent = 10;
	slowStartState = 1;
	ssthresh = 160;
	/*
	if(source->STATE != 2){
		source->init();
		return 1;
	}*/
	
	searchMax(lastSent);
	return 0;
}

int flow::getCwnd(){
	return lastSent;
}


//searchMax should send 'size' packets as oppsed to a big packet of size 'size'.

void flow::searchMax(int size) {
	cout << "FLOW: "<<this->id<< "SENDING PACKET FROM " << source->name << " TO " << dest->name << ", SIZE " << size << endl;
	packetnum++;
	packet p(size, packetnum, source, dest);
	p.f = this;
	packetVector.push_back(p);
	source->pushPacket(packetVector.size()-1,&linkVector[source->link_id]);
	state = 1;
	stringstream ss;
	ss << this->id;
	string pevent = "FLOW_" + ss.str();
	ss.str("");
	ss << packetnum;
	pevent += "_TIMEOUT_" + ss.str();
	pushEvent(pevent, P_TIMEOUT);
}

void flow::receiveAck(int pnum) {
	packet* p = &packetVector[pnum];
	if(data == 0)
		cout << "FLOW END" << endl;
	if (data == 0 || p->num < packetnum) {
		return;
	}
	else {
		cout << "FLOW: "<<this->id<< "RECEIVED ACK\tData: " << lastSent << "\tSource: " << p->src->name << "\tDest: " << p->dest->name <<"\tSrc: " << p->src->name<< endl;
		data -= lastSent;
		if(lastSent>=ssthresh)
			slowStartState=0;
		if(slowStartState==1)
			lastSent = lastSent * 2;
		else
			lastSent++;
		lastSent = min(lastSent, data);
		searchMax(lastSent);
	}
}

void flow::timeoutAck(int pnum) {
	//packet* p = &packetVector[pnum];
	cout << "FLOW: "<<this->id<< " TIMED OUT" << endl;
	//cin.ignore();
	//change cwnd to 1
	lastSent = 1;
	slowStartState = 1;
	ssthresh/=2;
	searchMax(lastSent);
}
