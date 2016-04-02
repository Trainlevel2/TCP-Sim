using namespace std;
#include "link.h"

// The packet constructor initializes the packet with set information of data and destination. 
node::node(int ip) {
	ip_addr = ip;
}

packet* recievePacket(link* link){ 	
	return NULL;
}

int sendPacket(link* link, packet* pkt, int dest_ip){
	return 0;
}
