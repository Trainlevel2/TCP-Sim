#ifndef _NODE_H
#define _NODE_H

#include <string>
#include "packet.h"
#include "link.h"

//class link;
//class packet;

class node {

	public:
		int br; //bitrate
		string name;
		//no constructor, since only subclasses constructors are called
		//specify host ip
		//node(int ip);
		
		//which link are we recieving the packet from?
		virtual void receivePacket(link* link) {};
		
		virtual void addLink(link* l) {};
		
		void pushPack(link* lptr);

		//which link are we sending through?
		//which packet are we sending?
		//what is the destination ip?
		//virtual int sendPacket(link* link, packet* pkt, int dest_ip);
		//virtual void sendPacket(node* dest, int size);

		string toString();

};

#endif // _NODE_H
