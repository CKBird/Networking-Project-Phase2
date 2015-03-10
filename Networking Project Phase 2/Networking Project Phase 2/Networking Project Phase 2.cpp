// Networking Project Phase 2.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "packets.h"
#include "host.h"
#include "time.h"
#include "stdio.h"
#include <iostream>
#include <fstream>

#define totalTime 100000																					//100 seconds, time in msec
#define timeStep .01
#define SIFS .05
#define DIFS .1
#define maxSpeed 13.75

double lambda = 0.9;
double mu = 2685;																						//Mu value such that average packet size is 512		
int numNetworks = 20;																					//N value from prompt
double simClock;

double lamdArr[7] = { .01, .05, .1, .3, .6, .8, .9 };
int numNArr[2] = { 10, 20 };

using namespace std;

int main(int argc, char* argv[])
{
	ofstream FILE;
	FILE.open("output.csv");

	for (int j = 0; j < 14; j++)
	{
		lambda = lamdArr[j % 7] * 1000;
		numNetworks = numNArr[j / 7];
		cout << "Test " << j << " - Lambda: " << lambda << " - Number of Networks: " << numNetworks << endl;

		while (!network.empty())
			network.pop_front();

		srand((unsigned int)time(NULL));
		for (int i = 0; i < numNetworks; i++)
		{
			network.push_back(host(i));																		//Once done, network.begin = host-0 etc
		}
		int DIFSCounter = (int)(DIFS / timeStep);
		int SIFSCounter = 0;
		double busyTill = 0.0;
		bool ACKSent = false;
		bool isSending = false;
		int bytesSent = 0;
		double totalDelay = 0;
		packet sent;

		for (simClock = 0; simClock < totalTime; simClock += timeStep)										//Time is in msec, but increment by hundreths of a msec 
		{
			if (isSending && (busyTill <= simClock))
			{
				//cout << "Current simClock: " << simClock << endl;
				//cout << "Done Sending" << endl;
				isSending = false;
			}
			if ((busyTill <= simClock) && DIFSCounter)
			{
				DIFSCounter--;
			}

			if (SIFSCounter && (busyTill <= simClock))
			{
				if (!ACKSent && !(--SIFSCounter))
				{
					isSending = true;
					ACKSent = true;
					//cout << "Current simClock: " << simClock << endl;
					(sent.returnDest())->receivedPacket(ACK(sent.returnNumber(), sent.returnSend(), NULL));	//Send ACK here
					bytesSent += 64;
					totalDelay += ACKTime;
					DIFSCounter = (int)(DIFS / timeStep);
					busyTill = simClock + ACKTime;
				}
			}

			if (!SIFSCounter && !DIFSCounter && (busyTill <= simClock))
			{
				list<host*> sending;
				for (list<host>::iterator it = network.begin(); it != network.end(); ++it)					//Iterate through list of hosts everytime
				{
					(*it).decSIFSTime();
					(*it).runNormalOp(simClock);

					if (!(*it).returnBackOff() && (*it).wantsToSend())
						sending.push_back(&*it);
					else
						(*it).decBackOff();
				}

				//cout << "Sending Size: " << sending.size() << endl;

				if (sending.size() == 1)
				{
					isSending = true;
					sending.front()->resetBOCounter();														//Reset BO Counter to 0
					SIFSCounter = (int)(SIFS / timeStep);													//Reset DIFS because something has sent
					sent = sending.front()->popOutgoing();
					sent.returnDest()->receivedPacket(sent);												//Sending packet to host
					bytesSent += sent.returnSize();
					totalDelay += (simClock - sent.returnTimeOC() + sent.returnTime());
					//cout << "Current simClock: " << simClock << endl;
					//cout << "Sending From: " << (*(sent.returnSend())).returnHostNumber() << " Sending To: " << (*(sent.returnDest())).returnHostNumber() << " Packet Number: " << sent.returnNumber() << "Packet Time: " << sent.returnTime() << endl;
					busyTill = sent.returnTime() + simClock;												//Set next busyTill to end of next packet transmission
					ACKSent = false;
				}
				else if (sending.size() > 1)
				{
					for (list<host*>::iterator it = sending.begin(); it != sending.end(); ++it)
						(*it)->setBackOff();
				}
			}
		}

		double through = ((double)bytesSent / totalTime) * timeStep;
		//cout << "Always Busy Speed: " << maxSpeed << " - Bytes per hundreths of a msec" << endl;
		//cout << "Bytes Sent: " << bytesSent << " - Bytes" << endl;
		cout << "Throughput: " << through << " - Bytes per hundreth of a msec" << endl;
		//cout << "Total Delay: " << totalDelay << " - msec" << endl;
		double avgDelay;
		if (through != 0)
		{
			avgDelay = totalDelay / bytesSent; //(through * 100);
			cout << "Average Network Delay: " << avgDelay << " - msec" << endl;
		}
		else
			cout << "Average Network Delay cannot be determined as throughput is 0" << endl;
		cout << endl << endl;
		FILE << lambda << "," << numNetworks << "," << through << "," << avgDelay << endl;
	}
	cout << "Simulations complete" << endl;
	FILE.close();
	char input;
	cin >> input;
	
	//Have ending bracket for values here
	return 0;
}

