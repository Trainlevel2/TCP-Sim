//Original Author: Darwin Huang
//Takes in Mark's guapguap.csv and outputs some new guap...
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
using namespace std;

//IMPORTANT!!!
//CHECK THESE BEFORE IMPORTING!!
int numberOfFlows = 3;
int numberOfLinks = 9;
int maxTime = 3000000;
int dt = 10000;
string packetDelayLog = "";
string flowRateLog = "";

vector < vector < double > > flowRate(numberOfFlows, vector<double>(maxTime/dt+1, 0)); //rows are flows and columns are times.
vector < vector < double > > linkRate(numberOfLinks, vector<double>(maxTime/dt+1, 0));

int stoi(string s){
	istringstream iss(s);
	int i; iss>>i;
	return i;
}

void processGuapGuap(){
    cout << "Processing Guap Guap..." << endl;
    //import file, initialize variables
    ifstream infile("guapguap.txt");
    string line = "";
    string word = "";

    string sFlowIndex = "";
    int flowIndex = -1;
    int endTime = -1;
    int startTime = -1;
    int packetSize = -1;
    int f = -1;

    //prep for output files
    ofstream flowRateFile;
    flowRateFile.open("flowRateLog.csv");
    ofstream packetDelayFile;
    packetDelayFile.open("packetDelayLog.csv");

    //read every line
    while (getline(infile, line)){
        stringstream strstr(line);

        //read every word in the line
        getline(strstr,word,',');
        sFlowIndex = word;

        getline(strstr,word,',');
        endTime = stoi(word);
        getline(strstr,word,',');
        startTime = stoi(word);
        getline(strstr,word,',');
        packetSize = stoi(word)*1024;

        flowIndex = stoi(sFlowIndex.substr(5));
        if(startTime < endTime){
            //FLOW RATE CALCULATIONS
            double packetRate = (double)packetSize / (endTime - startTime);
            //cout << "packetSize: " << packetSize << "startTime: " << startTime << "endTime: " << endTime << "packetRate: " << packetRate << endl;

            //normalize startTime and endTime in respect to dt
            int normStartTime = startTime / dt;
            int normEndTime = endTime / dt;

            for(int t = normStartTime; t <= normEndTime; t ++){ //Add packet 's rate to all time intervals, normalized by dt
                //add packet's rate
                flowRate[flowIndex][t] += packetRate;
                //cout << "flow rate at (" << flowIndex << "," << t*dt << ") is now: " << flowRate[flowIndex][t] << endl;
            }

            int avgTime = ((startTime+endTime)/2000);
            double delay = 1.0*(endTime-startTime)/packetSize;

            packetDelayFile << flowIndex << "," << avgTime << "," << delay << endl;
        }
    }

    //post-processing link/flow rates from vectors to .csv
    //cout << "FlowRate i = " << flowRate.size() << ", j = " << flowRate[0].size() << endl;
    /*for(int i = 0; i < flowRate.size(); i++){
        for(int j = 0; j < flowRate.at(0).size(); j++){
            flowRateFile << i << "," << (j*dt/1000) << "," << flowRate[i][j] << endl;
        }
    }
	*/
	for(int j = 0; j < flowRate.at(0).size(); j++){
        flowRateFile << (j*dt/1000);
        for(int i = 0; i < flowRate.size(); i++){
            flowRateFile << "," << flowRate[i][j];
        }
        flowRateFile << endl;
    }

    //output to file

    flowRateFile.close();
    packetDelayFile.close();
}

void processLinkGuap(){
    cout << "Processing Link Guap..." << endl;
    //import file, initialize variables
    ifstream infile("linkRateGuap.txt");
    string line = "";
    string word = "";

    string sLinkIndex = "";
    int linkIndex = -1;
    int endTime = -1;
    int startTime = -1;
    int packetSize = -1;
    int f = -1;

    //prep for output files
    ofstream linkRateFile;
    linkRateFile.open("linkRateLog.csv");


    //read every line
    while(getline(infile, line)){
        stringstream strstr(line);

        //read every word in the line
        getline(strstr,word,',');
        sLinkIndex = word;
        getline(strstr,word,',');
        endTime = stoi(word);
        getline(strstr,word,',');
        startTime = stoi(word);
        getline(strstr,word,',');
        packetSize = stoi(word)*1024;

        linkIndex = stoi(sLinkIndex.substr(5));
        if(startTime < endTime){
            //LINK RATE CALCULATIONS
            double packetRate = (double)packetSize / (endTime - startTime);
            //cout << "packetSize: " << packetSize << "startTime: " << startTime << "endTime: " << endTime << "packetRate: " << packetRate << endl;

            //normalize startTime and endTime in respect to dt
            int normStartTime = startTime / dt;
            int normEndTime = endTime / dt;

            for(int t = normStartTime; t <= normEndTime; t ++){ //Add packet 's rate to all time intervals, normalized by dt
                //add packet's rate
                linkRate[linkIndex][t] += packetRate;
                //cout << "link rate at (" << linkIndex << "," << t*dt << ") is now: " << linkRate[linkIndex][t] << endl;
            }
        }
    }

    //post-processing link/link rates from vectors to .csv
    /*
    for(int i = 0; i < linkRate.size(); i++){
        for(int j = 0; j < linkRate.at(0).size(); j++){
            linkRateFile << i << "," << (j*dt/1000) << "," << linkRate[i][j] << endl;
        }
    }
    */

    for(int j = 0; j < linkRate.at(0).size(); j++){
        linkRateFile << (j*dt/1000);
        for(int i = 0; i < linkRate.size(); i++){
            linkRateFile << "," << linkRate[i][j];
        }
        linkRateFile << endl;
    }

    //output to file
    linkRateFile.close();

}

main(){
    cout << "Welcome to this fun post-processing program" << endl;
    processGuapGuap();
	processLinkGuap();
    cout << "Completed run." << endl;
}
