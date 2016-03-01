//ECE303 Project
//Original Author: Darwin Huang

//importing libraries
using namespace std;
#include "packet.h"

// The packet constructor initializes the packet with set information of data and destination. 
packet::packet(int dat, node* source, node* destination) {
	data = dat;
	src = source;
	dest = destination;
	timestamp = getTime();	//HOW DO I GET THE TIME OF THE SIMULATION???
}


// Displays the packet for testing purposes
packet::display(){
	cout << "Packet display:" << endl;
	cout << "data: \t" << data << endl;
	cout << "destination: \t" << *destination << endl;
	cout << "source: \t" << *source << endl;
	cout << "timestamp: \t" << timestamp << endl;
}