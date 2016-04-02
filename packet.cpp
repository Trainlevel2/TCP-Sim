//ECE303 Project
//Original Author: Darwin Huang

//importing libraries
#include <iostream>
#include <string>
using namespace std;
#include "packet.h"
#include "node.h"

// The packet constructor initializes the packet with set information of data and destination. 
packet::packet(int dat, node* source, node* destination) {
	data = dat;
	src = source;
	dest = destination;
	//TODO: time system
	//timestamp = getTime();	//HOW DO I GET THE TIME OF THE SIMULATION???
}


// Displays the packet for testing purposes
void packet::display(){
	cout << "Packet display:" << endl;
	cout << "data: \t" << data << endl;
	cout << "destination: \t" << dest->toString() << endl;
	cout << "source: \t" << src->toString() << endl;
	//cout << "timestamp: \t" << timestamp << endl;
}