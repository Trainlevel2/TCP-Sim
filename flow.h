#ifndef _FLOW_H
#define _FLOW_H
#include "host.h"
#include "packet.h"
class host;
class packet;

/*
	Every flow is an abstracted stream of data which only 
	controls what size and when packets are sent
	
	The flow generates packets at a rate controlled by the
	congestion control algorithm defined for that flow. 
	
	At least two different congestion control algorithms, 
	e.g. TCP Reno and FAST-TCP, will be implemented, and we will 
	be able to choose independently between them for each flow.
	
	Flows may send a continuous stream of data, or may send a finite user-specifiable amount of data;
	they may also start immediately or after some user-specifiable delay.
*/
class flow {

	public:
		host* source;
		host* dest;
		int data;
		int state; //0=start, 1=searchMax
		int lastSent;
		int slowStartState;
		int ssthresh;
		flow(host* source, host* dest, int data, int id);
		void startFlow();
		void receiveAck(int pnum);
		int id;
		int getCwnd();
		void timeoutAck(int pnum);
	private:
		const int P_TIMEOUT = 1000;
		int packetnum = 0;
		void searchMax(int size);
		
};

#endif //_FLOW_H
