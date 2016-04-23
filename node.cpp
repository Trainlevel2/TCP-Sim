#include <sstream>
#include <string>
using namespace std;
#include "host.h"
#include "node.h"
#include "link.h"

// The packet constructor initializes the packet with set information of data and destination. 
//node::node(int ip) {
//	ip_addr = ip;
//}

//void node::sendPacket(node* dest, int size){
//	return;
//}

string node::toString() {
	stringstream ss;
	
//	ss << ip_addr;
//	string outString = "IP: " + ss.str();
//	return outString;
	return "";
}

void pushPack(link* lptr){
	lptr->



}
