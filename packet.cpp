
//ECE303 Project
//Original Author: Darwin Huang

//importing libraries
#include <iostream>
#include <string>
using namespace std;
#include "packet.h"
#include "host.h"

// The packet constructor initializes the packet with set information of data and destination. 
packet::packet(int dat, int num, host* source, host* destination) {
	data = dat;
	src = source;
	dest = destination;
	this->num = num;
	//TODO: time system
	//timestamp = getTime();	//HOW DO I GET THE TIME OF THE SIMULATION???
}


// Displays the packet for testing purposes
string packet::toString() {
	string s = "";
	s += "Packet display:";
	s += "\ndata: \t" + data;
	s += "\ndestination: \t" + dest->name;
	s += "\nsource: \t" + src->name;
	s += "\n";
	//s += "timestamp: \t"; s += timestamp; s += endl;
	return s;
}