#ifndef _HOST_H
#define _HOST_H

#include <vector>
#include <queue>
#include "node.h"
#include "packet.h"
#include "link.h"
using namespace std;

class host{
	
	public:
		int ip_addr;

		host(int ip, vector<link*>* link_vector);
			
		packet* receivePacket(link* lptr);	  
		queue<packet*>* getQueue(link* lptr); 
			
		void pushPacket(packet* pptr, link* lptr);
		void transmitPacket(link* lptr);

	private:
	
		link* link_ptr;

		class outQueue {
			public:
				queue<packet*> outQueue;
				link* lptr;
		};

		vector<outQueue> qVec;

	
};

#endif //_HOST_H
