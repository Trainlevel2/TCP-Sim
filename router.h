//ECE303 Project
//Original Author: Kirk Cameron


#ifndef _ROUTER_H
#define _ROUTER_H

#include <vector>
#include <queue>
#include "node.h"
#include "packet.h"
#include "link.h"
using namespace std;


class router: public node {
	private:

		class outQueue {
			public:
				outQueue();
				queue<packet*> outQQ;
				link* lptr;
		};

		vector<outQueue> qVec;


	public:
		int ip_addr;
		int maxSize; //of output packet queue
		router(int ip, vector<link*>* link_vector);
		
		//get the output queue which corresponds to a connected link.
		queue<packet*>* getQueue(link* lptr);

		//get the packet present on a connected link.
		packet* recievePacket(link* lptr);

		//use routing table to determine packet's outbound link
		link* chooseLink();

		//push packet to router output queue.
		//if the queue is full, the packet is lost.
		//returns 0 if successful,
		//returns 1 on packet loss.
		int pushPacket(packet* pptr, link* lptr);

		//transmit packet to a certain outbound link.
		void transmitPacket(link* lptr);

};

#endif //_ROUTER_H
