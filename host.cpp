using namespace std;
#include "host.h"
#include "link.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale> 
#include <cstdlib>

// The packet constructor initializes the packet with set information of data and destination. 
host::host(int ip, link* link_ptr) {
	ip_addr = ip;
	this->link_ptr = link_ptr;
}

void host::recievePacket(packet* pptr)
{
	//TODO: send ACK to flow
	//return lptr->getPacket();
}

void host::sendPacket(host* dest, int size)
{

	//choose link
	//push packet to buffer of chosen link
	//chooseLink().sendPacket(obtainPacket());
	//only one link for hosts
	
	//TODO: create packet + push to link
}

packet* host::obtainPacket()
{
	return NULL;
}
/*
link* chooseLink()
{
	
}
*/









