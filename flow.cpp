#include <iostream>
#include <sstream>
using namespace std;
#include "flow.h"
#include "host.h"
#include "link.h"

extern vector<packet> packetVector;
extern vector<link> linkVector;
extern void pushEvent(string e, int elapseTime);
extern int t;
extern string guapLog;
extern string packetLossLog;

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
	lastSent = 1;
	slowStartState = 1;
	ssthresh = -2;
	curTime = t;
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
	if(source->STATE==2)
		cout << "FLOW: "<<this->id<< "SENDING PACKET FROM " << source->name << " TO " << dest->name << ", SIZE " << size << endl;
	curTime = t;
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
	pushEvent(pevent, RTT*lastSent);
}

void flow::receiveAck(int pnum) {
	packet* p = &packetVector[pnum];
	if (data == 0) {
		cout << "FLOW END" << endl;
		done = true;
	}
	if (data == 0 || p->num < packetnum) {
		return;
	}
	else {
		if (ssthresh == -2)
			ssthresh = -1;
		cout << "FLOW: "<<this->id<< "RECEIVED ACK\tData: " << lastSent << "\tSource: " << p->src->name << "\tDest: " << p->dest->name <<"\tSrc: " << p->src->name<< endl;
		RTT = (alpha * RTT) + (1 - alpha)*(t - curTime) / lastSent + RTT_EXTRA;
		stringstream ss;
		ss << this->id;
		guapLog += "FLOW_" + ss.str();
		ss.str("");
		ss << t;
		guapLog += "," + ss.str();
		ss.str("");
		ss << curTime;
		guapLog += "," + ss.str();
		ss.str("");
		ss << lastSent;
		guapLog += "," + ss.str() + "\n";
		cout << "RTT: " << RTT << ", LAST_TIME: " << (t - curTime) / lastSent << endl;
		data -= lastSent;
		cout << "========================SSTHRESH" << ssthresh << endl;
		if(lastSent>=ssthresh&&ssthresh!=-1)
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
	if(this->source->STATE==2)
		cout << "FLOW: "<<this->id<< " TIMED OUT" << endl;
	if (pnum > 0) {
		stringstream ss;
		ss << this->id;
		packetLossLog += "FLOW_" + ss.str();
		ss.str("");
		ss << pnum;
		packetLossLog += "," + ss.str();
		ss.str("");
		ss << lastSent;
		packetLossLog += "," + ss.str() + "\n";
	}
	RTT *= 16;
	//cin.ignore();
	//change cwnd to 1
	cout << "===========TIMEOUT=============SSTHRESH" << ssthresh << endl;
	if(ssthresh == -1)
		ssthresh = lastSent;
	if(lastSent != 1)
		ssthresh = lastSent/2;
	lastSent = 1;
	slowStartState = 1;
	searchMax(lastSent);
}
