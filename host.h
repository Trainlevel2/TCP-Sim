#ifndef _HOST_H
#define _HOST_H

#include <vector>
#include <queue>
#include "node.h"
using namespace std;
class link;
class packet;

class host{
	
	public:
		int ip_addr;

		host(int ip, link* link_ptr);
			
		packet* receivePacket();	  
		queue<packet*>* getQueue(); 
			
		void pushPacket(packet* pptr);
		void transmitPacket();

	private:
	
		link* link_ptr;
		queue<packet*> outQueue;
		vector<packet*> qVec;

	
};

#endif //_HOST_H
