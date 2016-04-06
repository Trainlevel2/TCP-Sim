#include <cstdlib>
#include <iostream>
#include <string>
#include <queue> //for std::priority_queue
using namespace std;
#include "link.h"

//GLOBAL VARIABLES for time management
std::priority_queue<string> q;
int time = 0;
string eventlog = "";
vector<link> linkVector;

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

// Run through and record data on the Network
// Implement flows
void SimulateNetwork(){
	
}

//Expects an event in the form of a string with standardized format, as well as time to elapse for the event
//Adds the "added time" and "time to execute" onto the start of string for use as the comparable to order events
//An example of a working format for e is LINK_0_SEND_PACKET or HOST_1_RECEIVE_PACKET
void pushEvent(string e, int elapseTime){
	int currentTime = time;
	int executeTime = time + elapseTime;
	string event = executeTime + "," + currentTime;
	event += "," + e; //csv for simplicity
	q.push(event);
}

//Pops an event from the event queue and executes the appropriate command, as contained in e and assuming correct format of e
//Pushes data onto a log for output
void popEvent(){
	if(q.empty()){
		cout << "ERROR: The event queue is empty! Cannot pop event!";
		return;
	}
	string event = q.top();
	q.pop();
	
	//Extract the original event message from the expanded event message
	int find = event.find_last_of(","); //right after the last comma lies the original event message
	string e = event.substr(find+1);
	
	//Extract the original event message's meaning
	find = e.find("_");
	string objectType = e.substr(0,find);
	
	e = e.substr(find+1); //gets the rest of the message
	find = e.find("_"); 
	string objectIndex = e.substr(0,find);
	
	string function = e.substr(find+1);
	
	//Execute the event in the event e that was initially input into pushEvent
	if(objectType == "LINK"){
		int index = stoi(objectIndex);
		if (function == "TRANSMIT_PACKET"){
			//linkVector[index].recievePacket();	
			linkVector[index].tpropagate();
		}
	}
	
	eventlog += "\n" + event; //add the event to the log
}

int main(int argc, char *argv[])
{
	cout<<"Welcome to the Network Simulator!"<<endl;
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
		else if(s=="q")
			s="train";
		else
			cout<<"Invalid Response. Enter 'a', 'b', 'c', or 'd'."<<endl;
	}
	SimulateNetwork();
	return 0;
}
