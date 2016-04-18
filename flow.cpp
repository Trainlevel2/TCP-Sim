#include <iostream>
#include <sstream>
using namespace std;
#include "flow.h"
#include "host.h"
extern vector<packet> packetVector;
extern void pushEvent(string e, int elapseTime);

// The packet constructor initializes the packet with set information of data and destination. 
flow::flow(host* source, host* dest, int data, int id) {
	this->source = source;
	this->dest = dest;
	this->data = data;
	this->id = id;
	state = 0;
}

void flow::startFlow() {
	cout << "FLOW START" << endl;
	lastSent = 10;
	searchMax(lastSent);
}

void flow::searchMax(int size) {
	//cout << "SENDING PACKET FROM " << source->name << " TO " << dest->name << ", SIZE " << size << endl;
	packetnum++;
	packet p(size, packetnum, source, dest);
	p.f = this;
	packetVector.push_back(p);
	source->pushPacket(packetVector.size()-1);
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
	if (data == 0 || p->num < packetnum) {
		//cout << "RECEIVED DUPLICATE ACK\tData: " << lastSent << "\tSource: " << p->src->name << "\tDest: " << p->dest->name << endl;
		return;
	}
	else {
		cout << "RECEIVED ACK\tData: " << lastSent << "\tSource: " << p->src->name << "\tDest: " << p->dest->name << endl;
		data -= lastSent;
		lastSent = lastSent * 3 / 2;
		lastSent = min(lastSent, data);
		searchMax(lastSent);
	}
}

void flow::timeoutAck(int pnum) {
	//packet* p = &packetVector[pnum];
	cout << "TIMED OUT" << endl;
	lastSent /= 2;
	searchMax(lastSent);
}
