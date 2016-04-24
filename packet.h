
//ECE303 Project
//Original Author: Darwin Huang

#ifndef _PACKET_H
#define _PACKET_H

#include "host.h"  
#include "flow.h"
class flow;
class host;	//why is a node declared here? -kirk

class packet {

	public:

		// The packet constructor initializes the packet with set information of data and destination. 
		// Must also find the time...
		packet(int dat, int num, node* src, node* dst); //node* should not be carried in the packet.  ideally, node ip  instead. --kirk
		
		class entry{
			public:
				int cost;
				int ip;
		};

		vector<entry> dVec;
		
		bool isAck;
		bool isRIP;

		// Displays the packet for testing purposes
		string toString();
		int data = -1;
		int timestamp = -1; // Time of birth for the packet
		node* src = nullptr; 
		node* dest = nullptr; 
		flow* f = nullptr;
		int num;
};

#endif //_PACKET_H