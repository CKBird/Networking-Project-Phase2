// Networking Project Phase 2.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "packets.h"
#include "host.h"
#include "time.h"
#include "stdio.h"
#include <iostream>

#define totalTime 10
#define timeStep .01
#define SIFS .05
#define DIFS .1

double lambda = 0.01;
int numNetworks = 2;																				//N value from prompt
double simClock;

using namespace std;

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < numNetworks; i++)
		network.push_back(host(i));															//Once done, network.begin = host-0 etc

	int DIFSCounter = (int)(DIFS / timeStep);
	int SIFSCounter = (int)(SIFS / timeStep);
	double busyTill = 0.0;
	bool ACKSent = false;
	packet sent;
	//Have for loop start here to change values <- this will be for simulations later
	for (simClock = 0; simClock < totalTime; simClock += timeStep)											//Time is in msec, but increment by hundreths of a msec 
	{
		//if (busyTill > simClock)
			//for (list<host>::iterator it = host::network.begin(); it != host::network.end(); ++it)	//Iterate through list of hosts everytime
				//(*it).decSIFSTime();

		if (SIFSCounter && (busyTill <= simClock))
		{
			if (!ACKSent && !(--SIFSCounter))
			{
				ACKSent = true;
				(sent.returnDest())->receivedPacket(ACK(sent.returnNumber(), sent.returnSend(), NULL));	//Send ACK here
				DIFSCounter = (int)(DIFS / timeStep);
				busyTill = simClock + ACKTime;
			}
			else
			{
				DIFSCounter--;
			}
		}

		if (!SIFSCounter && !DIFSCounter && (busyTill <= simClock))
		{
			list<host> sending;
			for (list<host>::iterator it = network.begin(); it != network.end(); ++it)		//Iterate through list of hosts everytime
			{
				(*it).decSIFSTime();
				if (!(*it).returnBackOff())
					sending.push_back(*it);
				else
					(*it).decBackOff();
			}

			if (sending.size() == 1)
			{
				sending.front().resetBOCounter();														//Reset BO Counter to 0
				SIFSCounter = (int)(SIFS / timeStep);															//Reset DIFS because something has sent
				sent = sending.front().popOutgoing();
				(*(sent.returnDest())).receivedPacket(sent);											//Sending packet to host
				busyTill = sent.returnTime() + simClock;													//Set next busyTill to end of next packet transmission
			}
			else if (sending.size() > 1)
			{
				for (list<host>::iterator it = sending.begin(); it != sending.end(); ++it)
					(*it).setBackOff();
			}
		}
	}
	cout << "I'm the best" << endl;
	char input;
	cin >> input;
	
	//Have ending bracket for values here
	return 0;
}

