//ECE303 Project
//Original Author: Darwin Huang

//importing libraries
using namespace std;
#include "link.h"

// The packet constructor initializes the packet with set information of data and destination. 
link::link(flow f, int maxT, node* src, node* dest){
	flow = f;
	maxThroughput = maxT; 
	source = source;
	destination = dest; 
}  

packet* link::getPacket(){
	buffer.pop();
	return buffer.front();
}

void sendPacket(packet* pkt){
	buffer.push(pkt);

}

// Displays the packet for testing purposes
link::display(){
cout << "Link display:" << endl;
cout << "Flow:\t" << flow << endl;
cout << "The maximum throughput is:\t" << maxThroughput << endl;
cout << "The link goes from " << *src << " to " << *dst << endl;
cout << "The link's buffer going into the router contains the following packets: " << endl;
for(int i = 0; i < buffer.size(); i++){
	buffer[i].display();
}
