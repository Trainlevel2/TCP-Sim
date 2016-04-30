//ECE303 Project
//Original Author: Darwin Huang

//importing libraries
#include <iostream>
#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;
#include "link.h"
#include "node.h"
extern void pushEvent(string e, int elapseTime);
extern vector<packet> packetVector;
extern int t;
extern string linkRateLog;

// The packet constructor initializes the packet with set information of data and destination. 
link::link(int maxT, int id, node* source, node* destination){
	maxThroughput = maxT;
	src = source;
	dest = destination;
	cost=1;
	//char buffer[50];
	//int n = sprintf(buffer,"%d..%d",src->ip_addr,dest->ip_addr);
	//printf("The id of this link is [%s], which is %d characters long",buffer,n);
	this->id = id;
	//buffsiz = buff;
	curTime = t;
}

int link::getBufferFill() {
	queue<int> temp = qp;
	int sum = 0;
	while (temp.size() != 0) {
		int packetSize = packetVector[temp.front()].data;
		sum += packetSize;
		temp.pop();
	}
	return sum;
}

void link::prepPropagate(node* n, int packet_in) {
	int sum = getBufferFill();
	int packetSize = packetVector[packet_in].data;
	int bufLeft = bufferSize - sum;
	sum += packetSize;
	if (sum < bufferSize) {
		qn.push(n);
		qp.push(packet_in);
		propagate();
	}
	else {
		cout << "DROPPED PACKET KEK" << endl;
	}
	cost = maxThroughput*bufLeft / delay;
}

void link::propagate(){
	//node* nn = qn.front();
	//packet* pp = &packetVector[qp.front()];
	//cout << "Queuing Packet\tLink: " << this->id << "\tPacket: " << pp->num << endl;
	if (qp.size() == 1) {
		pnum = qp.front();
		stringstream ss;
		ss << this->id;
		if (qn.front() != src) {
			node* s = src;
			src = qn.front();
			dest = s;
		}
		curTime = t;
		pushEvent("LINK_" + ss.str() + "_TRANSMIT_PACKET", ((100 * packetVector[qp.front()].data) / maxThroughput) + (delay * 100));
	}
}

void link::forcepropagate() {
	//node* nn = qn.front();
	//packet* pp = &packetVector[qp.front()];
	//cout << "Queuing Packet\tLink: " << this->id << "\tPacket: " << pp->num << endl;
	pnum = qp.front();
	stringstream ss;
	ss << this->id;
	if (qn.front() != src) {
		node* s = src;
		src = qn.front();
		dest = s;
	}
	curTime = t;
	pushEvent("LINK_" + ss.str() + "_TRANSMIT_PACKET", ((100 * packetVector[qp.front()].data) / maxThroughput) + (delay*100));
}

void link::tpropagate() {
	//cout << "T-Propagating packet\tLink: " << this->id << "\tPacket: " << packetVector[pnum].num << endl;
	stringstream ss;
	ss << this->id;
	linkRateLog += "LINK_" + ss.str();
	ss.str("");
	ss << t;
	linkRateLog += "," + ss.str();
	ss.str("");
	ss << curTime;
	linkRateLog += "," + ss.str();
	ss.str("");
	ss << packetVector[qp.front()].data;
	linkRateLog += "," + ss.str() + "\n";
	dest->receivePacket(this);
	qn.pop();
	qp.pop();
	if (qp.size() > 0) {
		//cout << "link " << this->id << " force propagate" << endl;
		forcepropagate();
	}
	//cin.ignore();
	return;
}

int link::getBufferSize(){
	return qp.size();
}

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