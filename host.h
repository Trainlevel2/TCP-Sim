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
		

		host(string name, int ip,int br);
		void receivePacket(link* l);
			
		void pushPacket(int pnum);
		void addLink(link* l);
		

	private:
		link* link_ptr;
	
};

#endif // _HOST_H
