
#ifndef _LINK_H
#define _LINK_H

#include "packet.h"
#include "flow.h"
#include <queue>

class link {

	public:
		link(int src_ip, int dest_ip, int bitrate);

		int sendPacket(packet* pkt);
		packet* getPacket();

	private:
		int src_ip;
		int dest_ip;
		int bitrate;

		//packet buffer
		queue<packet*> buffer;
};

#endif //_PACKET_H