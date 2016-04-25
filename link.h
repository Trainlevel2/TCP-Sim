#ifndef _LINK_H
#define _LINK_H

#include "packet.h"
#include "node.h"
#include "flow.h"
#include <string>
#include <queue>
class flow;
class node;
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
		//link(int maxT, int buff_max, node* src, node* dest);
		link(int maxT, int id, node* src, node* dest);
		//int pushPacket(packet* pkt);
		//packet* popPacket();
		string toString();
		int id;

		//propagation of current packet
		//this function should modify the current packet value
		//after propagation delay
		int getBufferSize();
		void propagate();
		void forcepropagate();
		void tpropagate();
		int pnum; //CURRENT packet being transmitted on the link
		node* src = nullptr;
		node* dest = nullptr;

		//qp is a queue of the packets in the link buffer.
		//get the packet at the top by using packetVector[qp.front()]
		queue<int> qp;
		//qn is a queue of the source nodes corresponding to the packets in the packet queue.
		//every time a packet is pushed or removed, so is a node
		queue<node*> qn;

	private:

		int maxThroughput = -1; 

		//packet buffer
		//vector<packet*> buffer;
		//int buffsiz = 0;
		//vector<packet*>* buffer = nullptr;
};

#endif //_LINK_H
