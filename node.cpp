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

	return "";
}

node::node(string name, int ip,int br){
	ip_addr = ip;
	this->name = name;
}






//general packet transmission function
//pushes to the queue of the link
void pushPacket(int pnum,link* link_ptr) {
	link_ptr->qn.push(this);
	link_ptr->qp.push(pnum);
	link_ptr->propagate();
	br=100;
}