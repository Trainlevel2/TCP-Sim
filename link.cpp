//ECE303 Project
//Original Author: Darwin Huang

//importing libraries
using namespace std;
#include "link.h"

// The packet constructor initializes the packet with set information of data and destination. 
link::link(int maxT, int buff, node* source, node* destination);{
	maxThroughput = maxT;
	src = source;
	dest = destination; 
	buffsiz = buff;
}  

packet* link::popPacket(){
	buffer.pop();
	return buffer.front();
}

int link::pushPacket(packet* pkt){
	if (buffer.size()<buffsiz) {
		buffer.push(pkt);
	}
	else if (buffer.size()==buffsiz) {
		delete pkt;
	}
}

// Displays the packet for testing purposes
void link::display(){
cout << "Link display:" << endl;
cout << "Flow:\t" << flow << endl;
cout << "The maximum throughput is:\t" << maxThroughput << endl;
cout << "The link goes from " << *src << " to " << *dst << endl;
cout << "The link's buffer going into the router contains the following packets: " << endl;
for(int i = 0; i < buffer.size(); i++){
	buffer[i].display();
}
