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
#include "dVec.h"
using namespace std;
class node;

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
				void bford();


		};

		rtable rt;

		//vector<entry> map; //maps ip's to routing table indicies.

		class field{
			int link_id;
			int type; //What's on the other end? 0 = host, 1 = router.
			int ip; 
		};

		vector<field> lVector; //connected link info
		

		int host_ip;
		
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
