#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

// Display an ASCII View of how the network should look
void printNetwork()
{
	//
}

// Add a link from a given host/router to a new Host
void addHost()
{
	//
}

// Add a link from a given host/router to a new Router
void addRouter()
{
	//
}

// Add a link between two existing hosts/routers
void linkRouters()
{
	//
}


int main(int argc, char *argv[])
{
	cout<<"Welcome to the Network Simulator"<<endl;
	string s = "s";
	cout<<"Enter the name of the first host:"<<endl;
	string h1;
	cin>>h1;
	while(s!="train"){
		printNetwork();
		cout<<"What would you like to do?"<<endl;
		cout<<"a. Add a host."<<endl;
		cout<<"b. Add a router."<<endl;
		cout<<"c. Link existing hosts/routers."<<endl;
		cout<<"q. Quit."<<endl;
		cin>>s;
		if(s=="a")
			addHost();
		else if(s=="b")
			addRouter();
		else if(s=="c")
			linkRouters();
		else if(s=="d")
			s="train";
		else
			cout<<"Invalid Response. Enter 'a', 'b', 'c', or 'd'."<<endl;
	}
	return 0;
}