//ECE303 Project
//Original Author: Darwin Huang

#ifndef _PACKET_H
#define _PACKET_H


//fwd declared dependencies
class node;
class flow;


//included dependencies
#include <string>
#include "dVec.h"

class packet {

	public:

		// The packet constructor initializes the packet with set information of data and destination. 
		// Must also find the time...
		packet(int data, int num, node* src, node* dst); //node* should not be carried in the packet.  ideally, node ip  instead. --kirk
		
		dVec dv;
		bool isCR;
		bool isAck;
		bool isRIP;
		bool isCTS;
		int t;

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
