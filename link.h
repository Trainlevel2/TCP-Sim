#ifndef _LINK_H
#define _LINK_H

#include "packet.h"
#include "flow.h"
#include <queue>

class link {

	public:
		// The link constructor initializes the link with set information of data and destination.
		link::link(flow f, int maxT, node* src, node* dest);
		int sendPacket(packet* pkt);
		packet* getPacket();
		void display();
	private:
		node* src = nullptr;
		node* dest = nullptr;
		int bitrate;
		int maxThroughput = -1; // maximum data that can go through the packet
		//packet buffer
		vector<packet*> buffer;
};

#endif //_PACKET_H
