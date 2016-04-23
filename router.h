//ECE303 Project
//Original Author: Kirk Cameron


#ifndef _ROUTER_H
#define _ROUTER_H

#include <vector>
#include <queue>
#include "host.h"
#include "node.h"
#include "packet.h"
#include "link.h"
using namespace std;
class node;

class router: public node {
	
	//make router inherit some behavior from host
	//make link have a propagate packet function
	//implement max size of 




	public:

		vector<vector<int>> rtable;

		vector<int> lVector;

		router(string name, int ip);
		
		void addLink(int lnum);

		//get the packet present on a connected link.
		int receivePacket(int lnum);

		//use routing table to determine packet's outbound link
		//packets forwarded using destination host address

		int chooseLink(int pnum);


		
		link* chooseLink();

		//push packet to router output queue.
		//if the queue is full, the packet is lost.
		//returns 0 if successful,
		//returns 1 on packet loss.
		//int pushPacket(packet* pptr, link* lptr);
		//void pushPacket(int pnum)

		//transmit packet to a certain outbound link.
		void transmitPacket(link* lptr);

};

#endif //_ROUTER_H
