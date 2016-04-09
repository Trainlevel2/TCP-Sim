//ECE303 Project
//Original Author: Kirk Cameron


#ifndef _ROUTER_H
#define _ROUTER_H

#include <vector>
#include <queue>
#include "host.h"
#include "node.h"
#include "packet.h"
#include "link.h"
using namespace std;
class node;

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
		string name;
		int maxSize; //of output packet queue
		router(string name);

		void addLink(link* l);
		
		//get the output queue which corresponds to a connected link.
		queue<packet*>* getQueue(link* lptr);

		//get the packet present on a connected link.
		void receivePacket(link* lptr);

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
