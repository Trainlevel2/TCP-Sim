#ifndef _ROUTER_H
#define _ROUTER_H

#include <vector>
using namespace std;
#include "node.h"


class router: public node {

	public:
		int ip_addr;
		//specify host ip
		router(int ip, vector<link*>* link_vector);
		
		//which link are we recieving the packet from?
		void recievePacket(packet* pptr);
		
		//which link are we sending through?
		//which packet are we sending?
		//what is the destination ip?
		//int sendPacket(link* link, packet* pkt, int dest_ip);
		void sendPacket(host* dest, int size);

		link* chooseLink();

	private:
		vector<link*>* links;

};

#endif //_ROUTER_H