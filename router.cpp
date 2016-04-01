using namespace std;
#include "router.h"

// The packet constructor initializes the packet with set information of data and destination. 
router::router(int ip, vector<link*>* link_vector){
	ip_addr = ip;
	links = link_vector;
}


void router::recievePacket(packet* pptr)
{ 	
	//TODO
	//return link.getPacket();
}

//int host::sendPacket(link* link, packet* pkt, int dest_ip)
void router::sendPacket(host* dest, int size){
	//TODO

	//get data from a file
	
	//break file up into packets

	//choose link
	
	//push packet to buffer of chosen link
	
}