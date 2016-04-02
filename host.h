#ifndef _HOST_H
#define _HOST_H



class host: public node {

	public:
		host(int ip_addr, link* lptr);

		//which link are we recieving the packet from?
		packet* recievePacket(link* link);

		//which link are we sending through?
		//which packet are we sending?
		//what is the destination ip?
		int sendPacket(link* link, packet* pkt, int dest_ip);

		//get data from a file
		packet* obtainPacket();

		//pick link
		link* chooseLink();

	private:
		link* link_ptr;
};

#endif //_HOST_H