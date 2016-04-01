#ifndef _LINK_H
#define _LINK_H

#include "packet.h"
#include "flow.h"
#include "node.h"
#include <queue>

/*
	Links connect hosts and routers, and carry packets from one end to the other. 
	Every link has a specified capacity in bits per second.
	
	Every host and router can process an infinite amount of 
	incoming data instantaneously, but outgoing data must sit on a link buffer until the link is free.
	Link buffers are first-in, first-out. 
	Packets that try to enter a full buffer will be dropped. 
	
	For the purpose of this project, all links are half-duplex 
	(data can flow in both directions, but only in one direction at a time).
*/



class link {

	public:
		// The link constructor initializes the link with set information of data and destination.
		link(int maxT, int buff_max, node* src, node* dest);
		int pushPacket(packet* pkt);
		packet* popPacket();
		void display();
	private:
		node* src = nullptr;
		node* dest = nullptr;

		int bitrate;
		int maxThroughput = -1; // maximum data that can go through the packet
		//packet buffer
		vector<packet*> buffer;
		int buffsiz = 0;
};

#endif //_PACKET_H
