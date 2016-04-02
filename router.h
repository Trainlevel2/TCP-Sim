
#ifndef _ROUTER_H
#define _ROUTER_H



class router: public node {

	public:
		//specify host ip
		router(int ip, vector<link*> link_vector);
		
		//which link are we recieving the packet from?
		packet* recievePacket(link* link);
		
		//which link are we sending through?
		//which packet are we sending?
		//what is the destination ip?
		int sendPacket(link* link, packet* pkt, int dest_ip);

		link* chooseLink();

	private:
		vector<link*>* links;

};

#endif //_ROUTER_H