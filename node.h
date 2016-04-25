#ifndef _NODE_H
#define _NODE_H

//fwd declared dependencies
class link;
class packet;

//included dependencies
#include <string>


class node {
	public:
		int br; //transmission bitrate....Separate from propagation!
		
		node(string name, int ip);
		
		string name;
		int ip;

		virtual void receivePacket(link* link_ptr) {};
		
		//virtual void addLink(link* l) {};
		virtual void addLink(int lnum);

		void pushPacket(int pnum,link* link_ptr);

		//which link are we sending through?
		//which packet are we sending?
		//what is the destination ip?
		//virtual int sendPacket(link* link, packet* pkt, int dest_ip);
		//virtual void sendPacket(node* dest, int size);

		string toString();

};

#endif // _NODE_H
