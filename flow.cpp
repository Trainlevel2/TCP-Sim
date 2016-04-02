using namespace std;
#include "link.h"

// The packet constructor initializes the packet with set information of data and destination. 
flow::flow(bool dir, link* lnk, int br) {
	direction = dir;
	link = lnk;
	bitrate = br;
}

