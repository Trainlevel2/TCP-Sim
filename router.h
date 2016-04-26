//ECE303 Project
//Original Author: Kirk Cameron


#ifndef _ROUTER_H
#define _ROUTER_H


//fwd declared dependencies
class link;


//included dependencies
#include <vector>
#include <queue>
#include "node.h"
#include "dVec.h"
//#include "packet.h"
//#include "link.h"
//#include "host.h"




using namespace std;


class router: public node {
	
	//make router inherit some behavior from host
	//make link have a propagate packet function
	//implement max size of

	//routing table gives costs from  

	public:
		
		class rtable{
			public:
				vector<dVec> dvv;
				int update(dVec* dv);
				dVec* getdv(int ip);
				void add(int ip);
				bool isPop(); //is table populated
				void bford();
		};

		rtable rt;

		//vector<entry> map; //maps ip's to routing table indicies.

		class field{
		public:
			int link_id;
			int type; //What's on the other end? 0 = host, 1 = router.
			int ip; 
		};



		//0 = unknown neighbors, empty routing table.
		//1 = broadcast sent, waiting on replies.
		//2 = neighbors known. setting up routing table.
		//3 = routing table done. listening for dvec updates.
		int STATE; 

		vector<field> lVector; //connected link info
		bool fulltable; //no maxints
		
		router(string name, int ip);
		
		void addLink(int lnum);

		//get the packet present on a connected link.
		void receivePacket(link* link_ptr);

		//use routing table to determine packet's outbound link
		//packets forwarded using destination host address

		int chooseLink(int pnum);

		void broadcast();
		int scanLinks();


};

#endif //_ROUTER_H
