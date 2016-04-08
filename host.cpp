using namespace std;
#include "host.h"
#include "link.h"
#include "packet.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale> 
#include <cstdlib>
#include <queue>
#include <new>



// The packet constructor initializes the packet with set information of data and destination. 
host::host( string name, int ip){
	ip_addr = ip;
	this->name = name;
}

//Receives packet
packet* host::receivePacket(){
	return link_ptr->currentPkt;
}

queue<packet*>* host::getQueue(){
	return &outQueue;
}

void host::setLink(link* link_ptr) {
	this->link_ptr = link_ptr;
}

//Sends packet
void host::pushPacket(packet* pptr){
	getQueue()->push(pptr);
}
void host::transmitPacket(){
	getQueue()->pop();
}