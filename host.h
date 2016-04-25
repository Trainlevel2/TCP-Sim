#ifndef _HOST_H
#define _HOST_H

//fwd declared dependencies
class link;
//class packet;


//included dependencies
#include <vector>
#include <string>
#include <queue>
#include "node.h"

using namespace std;


class host : public node{
	
		
	public:
		int link_id;
		host(string name, int ip);
		void receivePacket(link* link_ptr);
		//void pushPacket(int pnum);
		void addLink(int lnum);
		
};

#endif // _HOST_H
