//ECE303 Project
//Original Author: Darwin Huang

#ifndef _LINK_H
#define _LINK_H

class link {

 public:

  // The link constructor initializes the link with set information of data and destination.
  link(int initTransDir, int currentT, int maxT, Router* src, Router* dst);
  
  // Displays the link for testing purposes
  display();

 private:
  vector<packet> buffer();
  int transmitDirection = -1; //-1 for not set, 0 for reverse, 1 for forward
  int currentThroughput = -1; //the amount of packets going through
  int maxThroughput = -1; // maximum data that can go through the packet
  Router* source = nullptr; //start of link
  Router* destination = nullptr; //end of link

};

#endif //_LINK_H