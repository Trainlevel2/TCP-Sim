using namespace std;
#include "link.h"

// The packet constructor initializes the packet with set information of data and destination. 
router::router(int ip, vector<link*>* link_vector) {
	ip_addr = ip;
	links = link_vector;
}

packet* host::recievePacket(link* link);
{ 	
	return link.getPacket();
}

int host::sendPacket(link* link, packet* pkt, int dest_ip)
{
	//get data from a file
	
	//break file up into packets

	//choose link
	
	//push packet to buffer of chosen link
	
}