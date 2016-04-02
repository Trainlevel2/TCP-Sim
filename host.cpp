using namespace std;
#include "link.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale> 
#include <cstdlib>

// The packet constructor initializes the packet with set information of data and destination. 
host::host(int ip,link* link_ptr) {
	ip_addr = ip;
	link = link_ptr;
}

packet* host::recievePacket(link* link);
{ 	
	return link.getPacket();

}

int host::sendPacket(link* link, packet* pkt, int dest_ip)
{

	//choose link
	//push packet to buffer of chosen link
	chooseLink().sendPacket(obtainPacket());
	
}

packet* obtainPacket()
{
	
}

link* chooseLink()
{
	
}










