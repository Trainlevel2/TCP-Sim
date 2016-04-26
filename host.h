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
		void broadcast();
		//void pushPacket(int pnum);
		void addLink(int lnum);
		//void testing();
		void init();

		//0 = Initial state
		//1 = listening for a RIP connectionReq REPLY 
		//2 = listening for a RIP clearToSend REPLY
		//3 = clear to send.
		int STATE;
		//int defaultGateway;
};

#endif // _HOST_H
