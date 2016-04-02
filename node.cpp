#include <sstream>
#include <string>
using namespace std;
#include "node.h"
#include "link.h"

// The packet constructor initializes the packet with set information of data and destination. 
//node::node(int ip) {
//	ip_addr = ip;
//}

packet* node::recievePacket(link* link){ 	
	return NULL;
}

void node::sendPacket(host* dest, int size){
	return;
}

string node::toString() {
	stringstream ss;
	ss << ip_addr;
	string outString = "IP: " + ss.str();
	return outString;
}
