#ifndef _NODE_H
#define _NODE_H

#include "link.h"
#include "packet.h"
class node {

	public:
		//specify host ip
		node(int ip);
		
		//which link are we recieving the packet from?
		virtual packet* recievePacket(link* link);
		

		//which link are we sending through?
		//which packet are we sending?
		//what is the destination ip?
		virtual int sendPacket(link* link, packet* pkt, int dest_ip);

	private:
		int ip_addr;


};

#endif //_NODE_H