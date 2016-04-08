//ECE303 Project
//Original Author: Darwin Huang

#ifndef _PACKET_H
#define _PACKET_H

#include "host.h"  
class host;	//why is a node declared here? -kirk

class packet {

	public:

		// The packet constructor initializes the packet with set information of data and destination. 
		// Must also find the time...
		packet(int dat, host* src, host* dst);

		// Displays the packet for testing purposes
		string toString();
		int data = -1;

	private:
		int timestamp = -1; // Time of birth for the packet
		host* src = nullptr; // Packet birthplace
		host* dest = nullptr; // Destiny determines that the packet must go here to rest

};

#endif //_PACKET_H
