#ifndef _HOST_H
#define _HOST_H
#include "packet.h"
#include "link.h"


//host is very different from router -> not a node
class host{
	
	public:
		int ip_addr;

		host(int ip, link* lptr);

		//which link are we recieving the packet from?
		void recievePacket(packet* pptr);

		//which link are we sending through?
		//which packet are we sending?
		//what is the destination ip?

		//let's abstract the IP to just a host pointer
		//only one link per host is allowed, and let the function create the packet given data size.
		//int sendPacket(link* link, packet* pkt, int dest_ip);
		void sendPacket(host* dest, int size);

		//what does this mean?
		//get data from a file
		packet* obtainPacket();

		//only one link in hosts
		//pick link
		//link* chooseLink();

	private:
		link* link_ptr;
};

#endif //_HOST_H