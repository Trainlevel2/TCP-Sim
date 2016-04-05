//ECE303 Project
//Original Author: Darwin Huang

#ifndef _PACKET_H
#define _PACKET_H

#include "node.h"  
class node;	//why is a node declared here? -kirk

class packet {

	public:

		// The packet constructor initializes the packet with set information of data and destination. 
		// Must also find the time...
		packet(int dat, node* src, node* dst);

		// Displays the packet for testing purposes
		void display();

	private:
		int data = -1;
		int timestamp = -1; // Time of birth for the packet
		node* src = nullptr; // Packet birthplace
		node* dest = nullptr; // Destiny determines that the packet must go here to rest

};

#endif //_PACKET_H