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

		void pushPacket(int pnum,link* link_ptr);
		int STATE;
		/*
			Host States:

			0 = Router Unknown
			Initial state.

			If flow_start is called on a host in this state, it sends a CRO packet.
			Its WantsToSend flag is updated to 1.
			
			If a host in this state receives a CRO, it sets its router, sets its state to 1, and replies with a CR1 packet.
			If a host in this state receives a CR1, it sets its router, and sets its state to 1.
			

			A host in this state will not rreceive a CTS because its router will be unknown.


			1 = Router Known
			
			If a host in this state receives a CTS, it sets its state to 2.
			If its WantsToSend flag is equal to 1, flow::start is called again on this host.


			2 = ClearToSend.
			Flow begins.

		*/
		
		int defaultGateway;

};

#endif // _HOST_H
