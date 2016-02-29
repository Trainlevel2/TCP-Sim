//ECE303 Project
//Original Author: Darwin Huang

#ifndef _LINK_H
#define _LINK_H

class link {

 public:

  // The link constructor initializes the link with set information of data and destination.
  link(int f, int maxT, Router* src, Router* dst);
  
  // Displays the link for testing purposes
  display();

 private:
  vector<packet> buffer();
  int flow = 0; //negative for backwards, positive for forwards, magnitude indicates current throughput
  int maxThroughput = -1; // maximum data that can go through the packet
  Router* source = nullptr; //start of link
  Router* destination = nullptr; //end of link

};

#endif //_LINK_H