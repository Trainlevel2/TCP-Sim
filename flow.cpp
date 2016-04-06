using namespace std;
#include "flow.h"
#include "host.h"

// The packet constructor initializes the packet with set information of data and destination. 
flow::flow(host* source, host* dest, int data) {
	this->source = source;
	this->dest = dest;
	this->data = data;
	state = 0;
	lastSent = 16;
	searchMax(lastSent);
	
}

void flow::searchMax(int size) {
	packet p(100, NULL, NULL); //TODO: GET HOSTS
	source->pushPacket(&p);
	state = 1;
}

void flow::receiveAck(packet* p) {
	data -= p->data; 
	lastSent *= 3 / 2;
	lastSent = min(lastSent,data);
	searchMax(lastSent);
}

void flow::timeoutAck() {
	lastSent /= 2;
	searchMax(lastSent);
}
