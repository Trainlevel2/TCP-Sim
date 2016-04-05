//ECE303 Project
//Original Author: Darwin Huang

//importing libraries
#include <iostream>
#include <cstdio>
using namespace std;
#include "link.h"
#include "node.h"


// The packet constructor initializes the packet with set information of data and destination. 
link::link(int maxT, int buff, node* source, node* destination){
	maxThroughput = maxT;
	src = source;
	dest = destination;
	char buffer[50];
	int n = sprintf(buffer,"%d..%d",src->ip_addr,dest->ip_addr);
	printf("The id of this link is [%s], which is %d characters long",buffer,n);
	id = buffer;
	//buffsiz = buff;
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

// Displays the link for testing purposes
void link::display(){
	cout << "Link id: "<< id << endl;
	cout << "Max throughput:\t" << maxThroughput << endl;
	cout << "The link goes from " << src->toString() << " to " << dest->toString() << endl;
	cout << "The link's buffer going into the router contains the following packets: " << endl;
	for (int i = 0; i < buffer.size(); i++) {
		buffer[i]->display();
	}
}