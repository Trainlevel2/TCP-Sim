#ifndef _HOST_H
#define _HOST_H

#include <vector>
#include <queue>
#include "node.h"
using namespace std;
class link;
class packet;

class host : public node{
	
	public:
		int ip_addr;

		host(string name, int ip);
		void receivePacket(link* l);	  
		queue<packet*>* getQueue(); 
			
		void pushPacket(packet* pptr);
		void transmitPacket();
		void addLink(link* l);
		string name;

	private:
		link* link_ptr;
		queue<packet*> outQueue;
		vector<packet*> qVec;

	
};

#endif _HOST_H
