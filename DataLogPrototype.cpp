//logs for graphing
//CODE NEEDS TO BE ADDED INTO UI.CPP!
//OR WE COULD MAKE EVERYTHING IN UI.CPP GLOBAL AND DO THIS IN A SEPARATE CLASS B/C UI.CPP IS GETTING SO BLOATED WITH CODE

//link/flow indices are in many of these logs because excel can just sort by that variable so that they're separated. And after that graph time vs. the other variable.

//LOGGING LINK RATE AND FLOW RATE
//INITIALIZATION FOR LOGS
//periodic
string bufferLog = "bufferLog \nlink index, time(s), buffer occupancy";
string cwndLog = "cwndLog \nflow index, time(s) , cwnd";

//event driven
string packetLossLog = "packetLossLog \nflow index, time(s), packets lost"; //TODO: Can we figure out how many packets were lost?
string flowRateLog = "flowRateLog \nflow index, time(s), flow rate(dataSize/ms)";
string linkRateLog = "linkRateLog \nlink index, time(s), link rate (dataSize/ms)";
string packetDelayLog = "packetDelayLog \nflow index, time(s), packetDelay(ms)";


//EXTRA INITIALIZATION FOR LOG CALCULATIONS
//Rate for a given flow/link, from a time t to time t+dt, is the summation of the rates of all packets in the flow/link at that time interval.
int dt = 100; //milliseconds per sample. Increase for gradation, decrease for speed. If this is infinity, you get avg rate.
vector<vector<double> > flowRate(flowVector.size()); //rows are flows and columns are times. TODO: initialize flowRate's column dimension to flowVector.size() (check vector constructor syntax)
vector<vector<double> > linkRate(linkVector.size()); //rows are links and columns are times. TODO: initialize linkRate's column dimension to linkVector.size() (check vector constructor syntax)
int endTime;
int startTime;
double packetRate;


POPEVENT(){
	//WITHIN TIME MANAGER:
	//FLOW AND LINK RATE CALCULATIONS
	if(event.find("PACKET") > 0){
		int endTime = t; //remove "endTime"/"startTime" for actual code. It's just for readability now
		int startTime = startTime; //TODO: get startTime
		double packetRate = packet.dataSize / (endTime - startTime); //TODO: get packet's dataSize
		
		//check for if flowRate/linkRate time dimension is large enough. If not, increase it until it is.
		while(flowRate.size() < endTime/dt){
			for(int i = 0; i < flowVector.size(); i++){
				flowRate.at(i).pushBack(0);
				linkRate.at(i).pushBack(0);
			}
		}
		
		if(event.find("flowPacketSent") > 0){
				for(int t = startTime/dt; t <= endTime/dt; t+=dt){ //Add packet 's rate to all time intervals, normalized by dt
					//add packet's rate
					flowRate.at(flowIndex,t/dt) += packetRate;
				}
		}
		else if(event.find("linkPacketSent") > 0){
			for(int t = startTime/dt; t <= endTime/dt; t+=dt){ //Add packet 's rate to all time intervals, normalized by dt
					//add packet's rate
					flowRate.at(flowIndex,t/dt) += packetRate;
				}
		}
	}
	
	if(event.find("FLOW") > 0){
		//PACKETLOSSLOG
		//example event -> FLOW_0_TIMEOUT_5
		if(event.find("TIMEOUT") > 0){
			packetLossLog += "\n" + flowIndex + "," + t/1000 + "," + amtPacketsLost; //TODO: Extract the flowIndex from the event so that the .csv will be easier to parse
			//TODO: get amtPacketsLost how?
		}
		
		//PACKETDELAYLOG
		//TODO: create packet sent event for packets sent across a flow for this and flowRateLog
		else if(event.find("TRANSMIT_PACKET" > 0){ //TODO: check syntax after FLOW_2_TRANSMIT_PACKET event exists/works
			packetDelaylog += "\n" + flowIndex +  "," + t/1000  + "," + (endTime-startTime);
		}	
	}
	
	//PERIODICALLY COLLECT:
	if(event.find("DATA_ACQUISITION")>0){ //TODO: create data acquisition events that spawn other data acquisition events
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
	}
}

//AFTER POPEVENT:
//POST-PROCESSING LINK/FLOW RATES TO .CSV
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

//PRINTING TO OUTPUT
cout << bufferLog << endl;
cout << cwndLog << endl;
cout << packetLossLog << endl;
cout << flowRateLog << endl;
cout << linkRateLog << endl;
cout << packetDelayLog << endl;