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
vector<flow> flowVector;
vector<int> flowStopTimeVector;
vector<packet> packetVector;
vector<host> hostVector;
vector<router> routerVector;

//logs for graphing
//periodic
string bufferLog = "bufferLog \nlink index, time(s), buffer occupancy";
string cwndLog = "cwndLog \nflow index, time(s) , cwnd";

//event driven
string packetLossLog = "packetLossLog \nflow index, time(s), packets lost";
string flowRateLog = "flowRateLog \nflow index, time(s), flow rate(dataSize/ms)";
string linkRateLog = "linkRateLog \nlink index, time(s), link rate (dataSize/ms)";
string packetDelayLog = "packetDelayLog \nflow index, time(s), packetDelay(ms)";

//extra stuff for logging
//Rate for a given flow/link, from a time t to time t+dt, is the summation of the rates of all packets in the flow/link at that time interval.
int dt = 100; //milliseconds per sample. Increase for gradation, decrease for speed. If this is infinity, you get avg rate.
vector<vector<double> > flowRate(flowVector.size()); //rows are flows and columns are times.
int endTime;
int startTime;
double packetRate;


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
		if (hostVector[i].name[0]=='H'){
			cout << hostVector[i].name << ", " << hostVector[i].ip << endl;
		}
	}

	cout << "ROUTERS" << endl;
	for (int i = 0; i < (int)routerVector.size(); i++) {
		if (routerVector[i].name[0]=='R'){
			cout << routerVector[i].name << ", " << routerVector[i].ip << endl;
		}
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
	router r(routerName, (int)routerVector.size());
	routerVector.push_back(r);
}


// Add a link from a given host/router to a new Router
void createLink(string linkName, string nodeA, string nodeB, int a, int b, int c){
	//implement later
	cout << "LINK: " << linkName << " FROM " << nodeA << " TO " << nodeB << ", PARAMETERS " << a << "," << b << "," << c << endl;
	node* nA;
	nA = findHost(nodeA);
	if (!nA)
		nA = findRouter(nodeA);
	node* nB;
	nB = findHost(nodeB);
	if (!nB)
		nB = findRouter(nodeB);

	link l(a, (int)linkVector.size(), nA, nB);
	l.bufferSize = c;
	l.delay = b;
	l.cost = a*c / b;
	linkVector.push_back(l);
	nA->addLink(l.id);
	nB->addLink(l.id);
}

// Create flows
void createFlow(string flowName, string hostA, string hostB, int a, int b){
	//implement later
	cout << "FLOW: " << flowName << " FROM " << hostA << " TO " << hostB << ", PARAMETER " << a << " " << b << endl;
	flow l(findHost(hostA), findHost(hostB), a, (int)flowVector.size());
	flowVector.push_back(l);
	flowStopTimeVector.push_back(0);
	stringstream ss;
	ss << (int)flowVector.size() - 1;
	string event = "FLOW_" + ss.str() + "_START";
	pushEvent(event, b);
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
	cout << "PUSH_EVENT:\t\t" << event << endl;
	
	//if flow, update when the flow is going to be done sending
	int whereTheFlowAt = e.find("FLOW_");
	if(whereTheFlowAt > 0){ //this is a flow event (-1 if it isn't)
		int find = e.find("_",whereTheFlowAt+5); //5 is the length of "FLOW_"
		string a = e.substr(whereTheFlowAt+5, find-whereTheFlowAt-5);
		int flowIndex = stoi(a);
		
		flowStopTimeVector[flowIndex] = executeTime;
	}

}

//Pops an event from the event queue and executes the appropriate command, as contained in e and assuming correct format of e
//Pushes data onto a log for output
void popEvent(){
	//WITHIN TIME MANAGER:
	if(q.empty()){
		cout << "ERROR: The event queue is empty! Cannot pop event!";
		return;
	}
	string event = q.top();
	cout << "POP\t\t" << event << endl;
	q.pop();
	
	//Extract the time at which the message is executed. This becomes the current time in the time manager.
	int find = event.find(",");
	string stimeNow = event.substr(0,find); //time after event is done
	//istringstream iss(stimeNow);
	//int timeNow; iss>>timeNow;
	int timeNow = stoi(stimeNow);
	if(timeNow >= t){
		t = timeNow;
	}
	else{
		cout << "ERROR: current event time (" << timeNow << ") ends before current time (" << t <<")!" << endl;
	}
	
	//Extract the original event message from the expanded event message
	int find2 = event.find(",", find + 1); //right after the last comma lies the original event message
	string sStartTime = event.substr(find+1, find2-find-1);
	//istringstream iss(stimeNow);
	//int startTime; iss>>startTime;
	int startTime = stoi(sStartTime);
	string e = event.substr(find2+1);
	
	
	//Extract the original event message's meaning
	find = e.find("_");
	string objectType = e.substr(0,find);
	
	e = e.substr(find+1); //gets the rest of the message
	find = e.find("_");
	string sObjectIndex = e.substr(0,find);
	int objectIndex = stoi(sObjectIndex);
	string function = e.substr(find+1);
	
	find = function.find("_"); //in case a function has 1 argument (only for timeouts right now)
	string functionn = function.substr(0,find);
	string arg = function.substr(find+1);
	
	/*
	//testing parsing of the event
	cout << "endTime " << timeNow << endl;
	cout << "startTime: " << startTime << endl;
	cout << "objectType: " << objectType << endl;
	cout << "objectIndex: " << objectIndex << endl;
	cout << "function: " << function << endl;
	cout << "arg: " << arg << endl;
	*/
	
	//Execute the event in the event e that was initially input into pushEvent

	
	//TRANSMIT_PACKET should result in 1 item being popped off the link's transmission queue
	//TRANSMIT_PACKET should push a PROPAGATE_PACKET onto the event queue.


	//PROPAGATE_PACKET should result in the link propagating the latest data popped off the the transmission queue.
	
	if(objectType == "LINK"){
		if (function == "TRANSMIT_PACKET"){
			//linkVector[index].recievePacket();	
			linkVector[objectIndex].tpropagate();
		}
	}
	else if (objectType == "FLOW") {
		if (function == "START") {
			flowVector[objectIndex].startFlow();
		}
		else if (functionn == "TIMEOUT") {
			int pptr = stoi(arg);
			flowVector[objectIndex].timeoutAck(pptr);
		}
	}
	
	//LOGS:
	/*
	//FLOW AND LINK RATE CALCULATIONS
	if(function == "TRANSMIT_PACKET"){
		int endTime = t;
		//double packetRate = packet.dataSize / (endTime - startTime); //TODO ASK: get packet's dataSize
		double packetRate = 1024 / (endTime - startTime); //TODO ASK: get packet's dataSize
		
		//check for if flowRate/linkRate time dimension is large enough. If not, increase it until it is.
		while(flowRate.at(0).size() < endTime/dt){
			for(int i = 0; i < flowVector.size(); i++){
				flowRate.at(i).pushBack(0);
			}
			for(int i = 0; i < linkVector.size(); i++){
				linkRate.at(i).pushBack(0);
			}
		}
		
		if(objectType == "FLOW"){
			for(int t = startTime/dt; t <= endTime/dt; t+=dt){ //Add packet 's rate to all time intervals, normalized by dt
				//add packet's rate
				flowRate.at(objectIndex,t/dt) += packetRate;
			}
		}
		else if(objectType == "LINK"){
			for(int t = startTime/dt; t <= endTime/dt; t+=dt){ //Add packet 's rate to all time intervals, normalized by dt
					//add packet's rate
					linkRate.at(objectIndex,t/dt) += packetRate;
			}
		}
	}
	
	
	if(objectType == "FLOW") > 0){
		//PACKETLOSSLOG
		//example event -> FLOW_0_TIMEOUT_5
		if(event.find("TIMEOUT") > 0){
			//packetLossLog += "\n" + objectIndex + "," + t/1000 + "," + amtPacketsLost; //TODO: get amtPacketsLost
			packetLossLog += "\n" + objectIndex + "," + t/1000 + "," + 5; //TODO: get amtPacketsLost
		}
		
		//PACKETDELAYLOG
		//TODO ASK: create packet sent event for packets sent across a flow for this and flowRateLog
		else if(function == "TRANSMIT_PACKET"){
			packetDelaylog += "\n" + objectIndex +  "," + t/1000  + "," + (endTime-startTime);
		}	
	}
	*/
	//PERIODICALLY COLLECT:
	//if(event.find("DATA_ACQUISITION")>0){ //TODO ASK: create data acquisition events that spawn other data acquisition events?
		//BUFFER SIZE
		for(int i = 0; i < (int)linkVector.size(); i++){
			//bufferLog += "\n" + i + "," + t / 1000 + "," + linkVector[i].getBufferSize();
			stringstream ss;
			ss << i;
			bufferLog += "\n" + ss.str();
			ss.str("");
			ss << t / 1000;
			bufferLog += "," + ss.str();
			ss.str("");
			ss << linkVector[i].getBufferSize();
			bufferLog += "," + ss.str();
		}
		//CWND
		for(int i = 0; i < (int)flowVector.size(); i++){
			//cwndLog += "\n" + i + "," + t / 1000 + "," + flowVector[i].getCwnd();
			stringstream ss;
			ss << i;
			cwndLog += "\n" + ss.str();
			ss.str("");
			ss << t / 1000;
			cwndLog += "," + ss.str();
			ss.str("");
			ss << flowVector[i].getCwnd();
			cwndLog += "," + ss.str();
		}
	//}
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

void outputGuap() {
	/*
	//post-processing link/flow rates from vectors to .csv
	for(int i = 0; i < flowRateVector.size(); i++){
		for(int j = 0; j < flowRateVector.at(0).size(); j++){
			flowRateLog += "\n" + i + "," + (j*dt/1000) + "," + flowRateVector.at(i,j);
		}
	}
	
	for(int i = 0; i < linkRateVector.size(); i++){
		linkRateLog += "\nFlow " + i;
		for(int j = 0; j < linkRateVector.at(0).size(); j++){
			linkRateLog += "\n" + i + ","+ (j*dt/1000) + "," + linkRateVector.at(i,j);
		}
	}
	*/
	//cout << bufferLog << endl;
	//cout << packetLossLog << endl;
	//cout << flowRateLog << endl;
	//cout << cwndLog << endl;
	//cout << packetDelayLog << endl;
	ofstream guapFile1;
	guapFile1.open("BufferGuap.txt");
	guapFile1 << bufferLog;
	guapFile1.close();

	ofstream guapFile2;
	guapFile2.open("packetLossGuap.txt");
	guapFile2 << packetLossLog;
	guapFile2.close();

	ofstream guapFile3;
	guapFile3.open("flowRateGuap.txt");
	guapFile3 << flowRateLog;
	guapFile3.close();

	ofstream guapFile4;
	guapFile4.open("cwndGuap.txt");
	guapFile4 << cwndLog;
	guapFile4.close();

	ofstream guapFile5;
	guapFile5.open("packetDelayGuap.txt");
	guapFile5 << packetDelayLog;
	guapFile5.close();

	ofstream guapFile6;
	guapFile6.open("eventGuap.txt");
	guapFile6 << eventlog;
	guapFile6.close();
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
		file = "./TestCases/testcase1.txt";	
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
			//cout<<"routerline detected"<<endl;
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
			//cout<<"linkline detected"<<endl;
			getline(read,ln);
			istringstream iss(ln);
			int numLink; iss>>numLink;
			for(int i = 0; i<numLink&&iss; i++){
				iss>>temp;
				string h1; iss>>h1;
				string h2; iss>>h2;
				int a; int b; int c; iss>>a; iss>>b; iss>>c;
				//cout<<"calling createLink"<<endl;
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
	/*
for (int i = 0; i < routerVector.size(); i++) {
		routerVector[i].rtHardCode();
	}
	*/
	
	cin.ignore();
	SimulateNetwork();
	cout << linkRateLog <<endl;
	//cin.ignore();
	//printNetwork();
	
	outputGuap();
	cin.ignore();
	return 0;
}
