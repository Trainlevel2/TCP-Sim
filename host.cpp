using namespace std;
#include "host.h"
#include "node.h"
#include "link.h"
#include "packet.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <locale> 
#include <cstdlib>
#include <queue>
#include <new>
#include <string>
extern vector<packet> packetVector;
extern vector<link> linkVector;
extern void popTimeout(int timeoutIndex);

/*
	//specify bitrate 
	host::host(string name, int ip)
	:node::node(name,ip){
		link_id=-1;
		STATE=0;
		//defaultGateway=-1;

	}
*/

//specify bitrate 
host::host(string name, int ip)
:node::node(name,ip){
	this->ip = ip;
	link_id=-1;
	STATE=0;
	defaultGateway=-1;

}

void host::pushPacket(int pnum,link* link_ptr) {
	packet* p = &packetVector[pnum];
	if(p->f != nullptr){
		if(this->STATE == 0){
			this->init();
		}else if(this->STATE ==2){
			link_ptr->qn.push(this);
			link_ptr->qp.push(pnum);
			link_ptr->propagate();
		}
	}else{
		link_ptr->qn.push(this);
		link_ptr->qp.push(pnum);
		link_ptr->propagate();
	}
	cin.ignore();
}

void host::init(){
	if(STATE==0){
		cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" SENT CR0 "<<endl;
		link* link_ptr = &linkVector[this->link_id];
		packet pSend(0, 0, this, this);
		pSend.isCR = true;
		pSend.t = 0;
		packetVector.push_back(pSend);
		pushPacket((int)packetVector.size() - 1,link_ptr);	
	}
	else{
		cout<< this->name <<" not in zero-state, can't initize"<<endl;
	}
}


/*
	Host States:

	0 = Router Unknown
	Initial state.

	If flow_start is called on a host in this state, it sends a CRO packet.
	Its WantsToSend flag is updated to 1.
	
	If a host in this state receives a CRO, it sets its router, sets its state to 1, and replies with a CR1 packet.
	If a host in this state receives a CR1, it sets its router, and sets its state to 1.
	

	A host in this state will not rreceive a CTS because its router will be unknown.


	1 = Router Known
	
	If a host in this state receives a CTS, it sets its state to 2.
	If its WantsToSend flag is equal to 1, flow::start is called again on this host.


	2 = ClearToSend.
	Flow begins.

*/


void host::receivePacket(link* link_ptr){
	packet* p = &packetVector[link_ptr->pnum];
	int tnum = link_ptr->pnum;
	link_ptr->pnum = -1;
	int snum=-1;

	if (STATE==0){ //router Unknown
		if(p->isCR){
			if(p->t==0){
				cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" RECEIVED CR0 "<<endl;
				this->defaultGateway = p->src->ip;
				packet pSend(0, p->num, this, p->src);
				pSend.isCR = true;
				pSend.t = 1;
				packetVector.push_back(pSend);
				snum = (int)packetVector.size() - 1;
				pushPacket(snum,link_ptr);
				STATE=1;
			}else if(p->t==1){
				STATE=1;
				cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" RECEIVED CR1, WAITING ON CTS "<<endl;
				this->defaultGateway = p->src->ip;
			}
		}
	}else if(STATE==1){ //router known
		if(p->isCTS){
			cout<<this->name<<" STATE: "<<this->STATE<<" :"<<" RECEIVED CTS, SENDING "<<endl;

			STATE=2;
		}
	}else if(STATE==2){ //clear to send
		if(p->f != nullptr){
			if (!p->isAck) {
				cout <<this->name<< " RECEIVED DATA, SENDING ACK" << endl;
				packet pSend(0, p->num, this, p->src);
				pSend.f = p->f;
				pSend.isAck = true;
				packetVector.push_back(pSend);
				snum = (int)packetVector.size() - 1;
				pushPacket(snum,link_ptr);
			}
			else {
				cout <<this->name<< " RECEIVED ACK" << endl;
				int pnum = p->num;
				p->f->receiveAck(tnum);
				//Delete associated timeout
				popTimeout(pnum); //the 0 is a stand-in for the timeout index, since there will only be one timeout at a time for now
			}
		}
	}
}
/*
//Receives packet
void host::receivePacket(link* link_ptr){
	//link* link_ptr = &linkVector[link_id];
	packet* p = &packetVector[link_ptr->pnum];
	int tnum = link_ptr->pnum;
	link_ptr->pnum = -1;


	//if there are no flows going out of a host.
	//

	
	if(STATE==0){ //0 = Initial state, no connectionReq packets yet sent
		if(p->isRIP){
			
		}

	}else if(STATE==1){//1 = listening for a RIP connectionReq REPLY 
		



	}else if(STATE==2){//2 = listening for a RIP clearToSend 
	

	}

		
}
*/


/*
void host::receivePacket(link* link_ptr){
	//link* link_ptr = &linkVector[link_id];
	packet* p = &packetVector[link_ptr->pnum];
	int tnum = link_ptr->pnum;
	link_ptr->pnum = -1;
	if (!p->isRIP){
		if (!p->isAck) {
			cout <<this->name<< " RECEIVED DATA, SENDING ACK" << endl;
			packet pSend(0, p->num, this, p->src);
			pSend.f = p->f;
			pSend.isAck = true;
			//pSend.isRIP = false;
			
			packetVector.push_back(pSend);
	
			snum = (int)packetVector.size() - 1;
		
			pushPacket(snum,link_ptr);
		}
		else {
			cout <<this->name<< " RECEIVED ACK" << endl;
			int pnum = p->num;
			p->f->receiveAck(tnum);
			//Delete associated timeout
			popTimeout(pnum); //the 0 is a stand-in for the timeout index, since there will only be one timeout at a time for now
		}
	}
	else{
		cout<<this->name<< " RECEIVED RIP,SENDING RIPly"<<endl;
		packet pSend(0, p->num, this, this);
		pSend.isRIP = true;
		pSend.isAck = false;
		packetVector.push_back(pSend);
		snum = packetVector.size() - 1;
		pushPacket(snum,link_ptr);
	}
}


*/



void host::addLink(int lnum) {
	this->link_id = lnum;
}

void host::broadcast(){
	link* link_ptr = &linkVector[link_id];
	int size= 1;
	int num= -1;
	packet p(size, num, this, this);
	p.isRIP = true;
	packetVector.push_back(p);
	pushPacket(packetVector.size()-1,link_ptr);
}