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
#include "rtable.h"
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
		
		

		//int discovered;
		rtable rt;
		//int host_ip;
		//vector<entry> map; //maps ip's to routing table indicies.

		class field{
		public:
			int link_id;
			
			//-1 = unknown
			//0 = host, 
			//1 = router
			//2 = self
			int type; //What's on the other end? 
			bool isCTS;
			int ip; 
		};
		bool routersConnected;
		queue<dVec> RIPbuf; 
		bool isCTS;
		bool isDefaultRouter;
		//0 = unknown neighbors, empty routing table.
		//1 = broadcast sent, waiting on replies.
		//2 = neighbors known. setting up routing table.
		//3 = routing table done. listening for dvec updates.
		int STATE; 
		void lVectorUpdate(link* link_ptr,packet* p);
		void testing();
		bool clearToSend();
		void crResp(link* link_ptr,packet* p);
		void inform(int n,int ip,link* link_ptr);
		vector<field> lVector; //connected link info
		bool fulltable; //no maxints
		void printLinks();
		router(string name, int ip);
		void rtHardCode();
		void addLink(int id);
		void receivePacket(link* link_ptr);
		
		void sendCR(int i,link* link_ptr);
		bool discoveryComplete();
		link* chooseLink(packet* p);
		void sendDVec(int ip_from);
		//void broadcast();
		void rtInit();
		dVec* genDv(dVec& dv);
};

#endif //_ROUTER_H
