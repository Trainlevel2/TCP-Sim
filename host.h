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

		host(string name, int ip);
		string name;
		packet* receivePacket();	  
		queue<packet*>* getQueue(); 
			
		void pushPacket(packet* pptr);
		void transmitPacket();
		void setLink(link* link_ptr);

	private:
		link* link_ptr;
		queue<packet*> outQueue;
		vector<packet*> qVec;

	
};

#endif //_HOST_H
