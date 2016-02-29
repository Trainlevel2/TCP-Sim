//ECE303 Project
//Original Author: Darwin Huang

//importing libraries
using namespace std;
#include "link.h"

// The packet constructor initializes the packet with set information of data and destination. 
link::link(int src,int dest,int br){
	src_ip = src;
	dest_ip = dest;
	bitrate = br;
}

packet* link::getPacket(){
	buffer.pop();
	return buffer.front();
}

void sendPacket(packet* pkt){
	buffer.push(pkt);

}