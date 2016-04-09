#include <iostream>
using namespace std;
#include "flow.h"
#include "host.h"
extern vector<packet> packetVector;

// The packet constructor initializes the packet with set information of data and destination. 
flow::flow(host* source, host* dest, int data) {
	this->source = source;
	this->dest = dest;
	this->data = data;
	state = 0;
}

void flow::startFlow() {
	cout << "FLOW START" << endl;
	lastSent = 10;
	searchMax(lastSent);
}

void flow::searchMax(int size) {
	cout << "SENDING PACKET FROM " << source->name << " TO " << dest->name << ", SIZE " << size << endl;
	packet p(size, source, dest);
	p.f = this;
	packetVector.push_back(p);
	source->pushPacket(&packetVector[packetVector.size()-1]);
	state = 1;
}

void flow::receiveAck(packet* p) {
	cout << "RECEIVED ACK, pSource: " << p->src->name << ", pDest: " << p->dest->name << endl;
	data -= lastSent;
	if (data == 0)
		return;
	else {
		lastSent = lastSent * 3 / 2;
		lastSent = min(lastSent, data);
		searchMax(lastSent);
	}
}

void flow::timeoutAck() {
	cout << "TIMED OUT" << endl;
	lastSent /= 2;
	searchMax(lastSent);
}
