//logs for graphing


//LOGGING LINK RATE AND FLOW RATE
//INITIALIZATION FOR LOGS
//periodic
string bufferLog = "bufferLog \ntime(s), link index, buffer occupancy";
string cwndLog = "cwndLog \ntime(s) , flow index, cwnd";

//event driven
string packetLossLog = "packetLossLog \ntime(s), flow index, othervariable??"; //TODO: what is the othervariable?
string flowRateLog = "flowRateLog \ntime(s), flow rate(dataSize/ms)";
string linkRateLog = "linkRateLog \ntime(s), link rate (dataSize/ms)";
string packetDelayLog = "packetDelayLog \ntime(s), flow index, packetDelay(ms)";


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
			packetLossLog += "\n" + t/1000 + "," + flowIndex + "," + otherVariable; //TODO: Extract the flowIndex and otherVariable from the event so that the .csv will be easier to parse
		}
		
		//PACKETDELAYLOG
		//TODO: create packet sent event for packets sent across a flow for this and flowRateLog
		else if(event.find("TRANSMIT_PACKET" > 0){ //TODO: check syntax after FLOW_2_TRANSMIT_PACKET works
			packetDelaylog += "\n" + t/1000 + "," + flowIndex + "," + (endTime-startTime);
		}
		
	}

	
	//PERIODICALLY COLLECT:
	if(event.find("DATA_ACQUISITION")>0){ //TODO: create data acquisition events that spawn other data acquisition events
		//BUFFER SIZE
		for(int i = 0; i < (int)linkVector.size(); i++){
			//bufferLog += "\n" + t/1000 + "," + i + "," + linkVector[i].getBufferSize();
			stringstream ss;
			ss << t / 1000;
			bufferLog += "\n" + ss.str();
			ss.str("");
			ss << i;
			bufferLog += "," + ss.str();
			ss.str("");
			ss << linkVector[i].getBufferSize();
			bufferLog += "," + ss.str();
		}
		//CWND
		for(int i = 0; i < (int)flowVector.size(); i++){
			//cwndLog += "\n" + t/1000 + "," + i + "," + flowVector[i].getCwnd();
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
	}
}
//POST-PROCESSING LINK/FLOW RATES TO .CSV
for(int i = 0; i < flowRateVector.size(); i++){
	flowRateLog += "\nFlow " + i;
	for(int j = 0; j < flowRateVector.at(0).size(); j++){
		flowRateLog += "\n" + (j*dt/1000) + "," + flowRateVector.at(i,j);
	}
}

for(int i = 0; i < linkRateVector.size(); i++){
	linkRateLog += "\nFlow " + i;
	for(int j = 0; j < linkRateVector.at(0).size(); j++){
		linkRateLog += "\n" + (j*dt/1000) + "," + linkRateVector.at(i,j);
	}
}

//PRINTING TO OUTPUT
cout << bufferLog << endl;
cout << cwndLog << endl;
cout << packetLossLog << endl;
cout << flowRateLog << endl;
cout << linkRateLog << endl;
cout << packetDelayLog << endl;