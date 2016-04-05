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
				queue<packet*> outQueue;
				link* lptr;
		};

		vector<outQueue> qVec;


	public:
		int ip_addr;
		router(int ip, vector<link*>* link_vector);
		//every link connected to a router is associated with an output queue.
		
		queue<packet*>* getQueue(link* lptr);

		packet* recievePacket(packet* pptr,link* lptr);

		link* chooseLink(packet* pptr);
		
		void pushPacket(packet* pptr, link* lptr);
		void transmitPacket(link* lptr);



};

#endif //_ROUTER_H
