//ECE303 Project
//Original Author: Darwin Huang

//importing libraries
using namespace std;
#include "link.h"


  // The link constructor initializes the link with set information of data and destination.
  link::link(int initTransDir, int currentT, int maxT, Router* src, Router* dst){
	int transmitDirection = initTransDir;
	int currentThroughput = currentT;
	int maxThroughput = maxT; 
	Router* source = src;
	Router* destination = dst; 
  }  
  
  // Displays the packet for testing purposes
  link::display(){
	cout << "Link display:" << endl;
	cout << "The transmit direction is:\t" << transmitDirection << endl;
	cout << "The current throughput is:\t" << currentThroughput << endl;
	cout << "The maximum throughput is:\t" << maxThroughput << endl;
	cout << "The link goes from " << *src << " to " << *dst << endl;
	cout << "The link's buffer going into the router contains the following packets: " << endl;
	for(int i = 0; i < buffer.size(); i++){
		buffer[i].display();
	}
  }