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

//specify bitrate 
host::host(string name, int ip)
:node::node(name,ip){
	link_id=-1;
	STATE=0;
	defaultGateway=-1;
}

host::init(){
	if(STATE==0){
		link* link_ptr = &linkVector[link_id];
		int size= 1;
		int num= -1; //connectionReq packet
		packet p(size, num, this, this);
		p.isRIP = true;
		packetVector.push_back(p);
		pushPacket(packetVector.size()-1,link_ptr);		
		STATE++;	
	}
	else{
		cout<< this->name <<" not in zero-state, can't initize"<<endl;
	}
}





//Receives packet
void host::receivePacket(link* link_ptr){
	link* link_ptr = &linkVector[link_id];
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





















	int snum;
	if (!p->isRIP){
		if (!p->isAck) {
		cout <<this->name<< " RECEIVED DATA, SENDING ACK" << endl;
		packet pSend(0, p->num, this, p->src);
		pSend.f = p->f;
		pSend.isAck = true;
		packetVector.push_back(pSend);
		snum = packetVector.size() - 1;
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