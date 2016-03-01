#ifndef _FLOW_H
#define _FLOW_H


/*
	Every flow points to a link, which specifies their source and destination address. 
	
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
		//flow direction
		//1 = link.src -> link.dest
		//-1= link.dest -> link.src
		//0 = no flow
		int direction;
		link* link;
		int bitrate = 0;
		
};

#endif //_FLOW_H