//ECE303 Project
//Original Author: Darwin Huang

//importing libraries
#include <iostream>
#include <cstdio>
#include <string>
using namespace std;
#include "link.h"
#include "node.h"
extern void pushEvent(string e, int elapseTime);

// The packet constructor initializes the packet with set information of data and destination. 
link::link(int maxT, int id, node* source, node* destination){
	maxThroughput = maxT;
	src = source;
	dest = destination;
	//char buffer[50];
	//int n = sprintf(buffer,"%d..%d",src->ip_addr,dest->ip_addr);
	//printf("The id of this link is [%s], which is %d characters long",buffer,n);
	this->id = id;
	//buffsiz = buff;
}

void link::propagate(packet* pptr){
	currentPkt = pptr;
	pushEvent("LINK_0_TRANSMIT_PACKET", maxThroughput / pptr->data);
	//q.push(pptr, maxThroughput/pptr->data);
	//buffsiz = buff;
}

void link::tpropagate() {
	return;
}
/*
	packet* link::popPacket(){
		buffer.pop_back();
		return buffer.front();
	}

	int link::pushPacket(packet* pkt){
		//TODO: FIX- the buffer is based on the size of the packets, not # of packets
		if (buffer.size()<buffsiz) {
			buffer.push_back(pkt);
		}
		else if (buffer.size()==buffsiz) {
			delete pkt;
		}
		return 0;
	}
*/

// Displays the packet for testing purposes
string link::toString() {
	string s = "";
	s += "Link display:";
	//s += "\nThe maximum throughput is:\t" + maxThroughput + endl;
	s += "\nThe link goes from " + src->toString() + " to " + dest->toString();
	/*
	s += "\nTransmitting from: ";
	if (from == a) {
		s += "a";
	}
	else if (from == b) {
		s += "b";
	}
	else if (from == nullptr) {
		s += "NOT TRANSMITTING";
	}
	*/
	s += "\nTransmission rate: " + maxThroughput;


	//s += "\nThe link's buffer going into the router contains the following packets: \n";
	//for (int i = 0; i < buffer.size(); i++) {
	//	s += buffer[i];
	//}
	return s;
}