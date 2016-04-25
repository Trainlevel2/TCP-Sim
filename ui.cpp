#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <queue> //for std::priority_queue
#include <functional>
using namespace std;
#include "link.h"
#include "host.h"
#include "router.h"
#include "flow.h"
#define WINDOWS 0 //1=true, 0=false

void pushEvent(string e, int elapseTime);
void popEvent();

//GLOBAL VARIABLES for time management
std::priority_queue<string, vector<string>, std::greater<string> > q;
int t = 0;
string eventlog = "";
vector<link> linkVector;
vector<host> hostVector;
vector<router> routerVector;
vector<flow> flowVector;
vector<packet> packetVector;

//logs for graphing
string linkRateLog = "end time (ms), start time (ms), action"; //TODO: convert to get rate
string bufferLog = "time(s), link index, buffer occupancy";
string packetLossLog = "time(s), loss"; //TODO: how?
string flowRateLog = "time (s), flow index, flow rate (Mb/s)"; //TODO: how?
string cwndLog = "time (s) , flow index , cwnd";
string packetDelayLog = "time(s), packetDelay"; //TODO: how?


router* findRouter(string rname) {
	for (int i = 0; i < (int)routerVector.size(); i++)
		if (routerVector[i].name == rname)
			return &routerVector[i];
	return NULL;
}

host* findHost(string hname) {
	for (int i = 0; i < (int)hostVector.size(); i++)
		if (hostVector[i].name == hname)
			return &hostVector[i];
	return NULL;
}

// Display an ASCII View of how the network should look
void printNetwork()
{
	cout << "HOSTS" << endl;
	for (int i = 0; i < (int)hostVector.size(); i++) {
		cout << hostVector[i].name << ", " << hostVector[i].ip_addr << endl;
	}

	cout << "ROUTERS" << endl;
	for (int i = 0; i < (int)routerVector.size(); i++) {
		cout << routerVector[i].name << endl;
	}

	cout << "LINKS" << endl;
	for (int i = 0; i < (int)linkVector.size(); i++) {
		cout << linkVector[i].id << ", " << ((host*)linkVector[i].src)->name << ", " << ((host*)linkVector[i].dest)->name << endl;
	}
}

// Add a host
void createHost(string hostName){
	//implement later
	cout << "HOST: " << hostName << endl;
	host h(hostName, (int)hostVector.size());
	hostVector.push_back(h);
}

// Add a router
void createRouter(string routerName){
	//implement later
	cout << "ROUTER: " << routerName << endl;
	router r(routerName);
	routerVector.push_back(r);
}

// Add a link from a given host/router to a new Router
void createLink(string linkName, string nodeA, string nodeB, int a, int b, int c){
	//implement later
	cout << "LINK: " << linkName << " FROM " << nodeA << " TO " << nodeB << ", PARAMETER " << a << endl;
	node* nA;
	nA = findHost(nodeA);
	if (!nA)
		nA = findRouter(nodeA);
	node* nB;
	nB = findHost(nodeB);
	if (!nB)
		nB = findRouter(nodeB);

	link l(a, (int)linkVector.size(), nA, nB);
	linkVector.push_back(l);

	nA->addLink(&linkVector[(int)linkVector.size() - 1]);
	nB->addLink(&linkVector[(int)linkVector.size() - 1]);
}

// Create flows
void createFlow(string flowName, string hostA, string hostB, int a, int b){
	//implement later
	cout << "FLOW: " << flowName << " FROM " << hostA << " TO " << hostB << ", PARAMETER " << a << " " << b << endl;
	flow l(findHost(hostA), findHost(hostB), a, (int)flowVector.size());
	flowVector.push_back(l);
	stringstream ss;
	ss << (int)flowVector.size() - 1;
	string event = "FLOW_" + ss.str() + "_START";
	pushEvent(event, 0);
}

// Run through and record data on the Network
void SimulateNetwork(){
	string sss = q.top();
	while (q.size() > 0) {
		popEvent();
	}
}

//Expects an event in the form of a string with standardized format, as well as time to elapse for the event
//Adds the "added time" and "to execute" onto the start of string for use as the comparable to order events
//An example of a working format for e is LINK_0_SEND_PACKET or HOST_1_RECEIVE_PACKET
void pushEvent(string e, int elapseTime){
	int currentTime = t;
	int executeTime = t + elapseTime;
	stringstream ss;
	ss << executeTime;
	int max_int_len = 8;
	string event = string(max_int_len - ss.str().size(),'0') + ss.str() + ",";
	ss.str("");
	ss << currentTime;
	event += string(max_int_len - ss.str().size(), '0') + ss.str() + "," + e; //csv for simplicity
	q.push(event);

	//cout << "NEW EVENT\t" << e << "\tCURTIME: " << currentTime << "\tDONE: " << executeTime << endl;
}

//Pops an event from the event queue and executes the appropriate command, as contained in e and assuming correct format of e
//Pushes data onto a log for output
void popEvent(){
	if(q.empty()){
		cout << "ERROR: The event queue is empty! Cannot pop event!";
		return;
	}
	string event = q.top();
	//cout << "POP\t\t" << event << endl;
	q.pop();
	
	//Extract the time at which the message is executed. This becomes the current time in the time manager.
	int find = event.find(",");
	string stimeNow = event.substr(0,find); //time after event is done
	istringstream iss(stimeNow);
	int timeNow; iss>>timeNow;
	if(timeNow >= t){
		t = timeNow;
	}
	else{
		cout << "ERROR: current event time (" << timeNow << ") ends before current time (" << t <<")!" << endl;
	}
	
	//Extract the original event message from the expanded event message
	find = event.find_last_of(","); //right after the last comma lies the original event message
	string e = event.substr(find+1);
	
	//Extract the original event message's meaning
	find = e.find("_");
	string objectType = e.substr(0,find);
	
	e = e.substr(find+1); //gets the rest of the message
	find = e.find("_"); 
	string objectIndex = e.substr(0,find);
	string function = e.substr(find+1);
	
	find = function.find("_"); //in case a function has 1 argument (only for timeouts right now)
	string functionn = function.substr(0,find);
	string arg = function.substr(find+1);
	
	
	//Execute the event in the event e that was initially input into pushEvent
	if(objectType == "LINK"){
		int index = stoi(objectIndex);
		if (function == "TRANSMIT_PACKET"){
			//linkVector[index].recievePacket();	
			linkVector[index].tpropagate();
		}
	}
	else if (objectType == "FLOW") {
		int index = stoi(objectIndex);
		if (function == "START") {
			flowVector[index].startFlow();
		}
		else if (functionn == "TIMEOUT") {
			int pptr = stoi(arg);
			flowVector[index].timeoutAck(pptr);
		}
	}
	eventlog += "\n" + event; //add the event to the log
	linkRateLog += "\n" + event;
	//log all flow cwnd's
	for(int i = 0; i < (int)flowVector.size(); i++){
		stringstream ss;
		ss << t / 1000;
		cwndLog += "\n" + ss.str();
		ss.str("");
		ss << i;
		cwndLog += "," + ss.str();
		ss.str("");
		ss << flowVector[i].getCwnd();
		cwndLog += "," + ss.str();
	}
	for(int i = 0; i < (int)linkVector.size(); i++){
		bufferLog += "\n" + t/1000;
		bufferLog += ",";
		bufferLog += i + "," + linkVector[i].getBufferSize().
	}
}

void popTimeout(int timeoutIndex){
	//TODO: implement this
	//how do you look through a priority_queue to find the timeout? STL priority queue doesn't seem to let us find stuff :(
	queue<string> qq;
	while (q.size() > 0) {
		string event = q.top();
		q.pop();
		
		//Extract the original event message from the expanded event message
		int find = event.find_last_of(","); //right after the last comma lies the original event message
		string e = event.substr(find + 1);

		//Extract the original event message's meaning
		find = e.find("_");
		string objectType = e.substr(0, find);

		e = e.substr(find + 1); //gets the rest of the message
		find = e.find("_");
		string objectIndex = e.substr(0, find);
		string function = e.substr(find + 1);

		find = function.find("_"); //in case a function has 1 argument (only for timeouts right now)
		string functionn = function.substr(0, find);
		string arg = function.substr(find + 1);
		istringstream iss(arg);
		int intarg; iss >> intarg;

		if (!(objectType == "FLOW" && functionn == "TIMEOUT" && intarg ==timeoutIndex)) {
			qq.push(event);
		}
		
	}
	while (qq.size() > 0) {
		string event = qq.front();
		q.push(event);
		qq.pop();
	}
}

int main(int argc, char *argv[])
{
	cout<<"Welcome to the Network Simulator!"<<endl;
	string ln, file, temp;
	ifstream read;
	//cout<<"Input the filename: ";
	//cin>>file;

	if(WINDOWS){
		file = ".\\TestCases\\testcase0.txt";	
	}else{
		file = "./TestCases/testcase0.txt";	
	}
	read.open(file.c_str());
	while(!read.eof())
	{
		getline(read,ln);
		//cout << "READING: " << ln << endl;
		// if(ln!="")		
			// cout<<"reading "<<ln<<endl;
		if(ln=="Hosts:"&&!read.eof()){
			getline(read,ln);
			istringstream iss(ln);
			int numHost; iss>>numHost;
			for(int i = 0; i<numHost&&iss; i++){
				iss>>temp;
				createHost(temp);
				// cout<<"Added Host"<<endl;
			}
		}
		else if(ln=="Routers:"&&!read.eof()){
			getline(read,ln);
			istringstream iss(ln);
			int numRout; iss>>numRout;
			for(int i = 0; i<numRout&&iss; i++){
				iss>>temp;
				createRouter(temp);
				// cout<<"Added Router"<<endl;
			}
		}
		else if(ln=="Links:"&&!read.eof()){
			getline(read,ln);
			istringstream iss(ln);
			int numLink; iss>>numLink;
			for(int i = 0; i<numLink&&iss; i++){
				iss>>temp;
				string h1; iss>>h1;
				string h2; iss>>h2;
				int a; int b; int c; iss>>a; iss>>b; iss>>c;
				createLink(temp,h1,h2,a,b,c);
				// cout<<"Added Link"<<endl;
			}
		}
		else if(ln=="Flows:"&&!read.eof()){
			getline(read,ln);
			istringstream iss(ln);
			int numFlow; iss>>numFlow;
			for(int i = 0; i<numFlow&&iss; i++){
				iss>>temp;
				string h1; iss>>h1;
				string h2; iss>>h2;
				int a; int b; iss>>a; iss>>b;
				createFlow(temp,h1,h2,a,b);				
				// cout<<"Added Flow"<<endl;
			}
		}
	}
	cout << endl;
	SimulateNetwork();
	cout<<eventlog<<endl;
	cout << cwndLog << endl;
	//cin.ignore();
	//printNetwork();
	cin.ignore();
	return 0;
}
