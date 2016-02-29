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
  packet::display(){
	cout << "Link display:" << endl;
	cout << "The transmit direction is:\t" << transmitDirection << endl;
	cout << "The current throughput is:\t" << currentThroughput << endl;
	cout << "The maximum throughput is:\t" << maxThroughput << endl;
	cout << "The link goes from " << *src << " to " << *dst << endl;
  }