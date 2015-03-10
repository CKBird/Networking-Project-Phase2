// Networking Project Phase 2.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "packets.h"
#include "host.h"
#include "time.h"
#include "stdio.h"
#include <iostream>
#include <fstream>

#define totalTime 100000																					//100 seconds, time in milliseconds 
#define timeStep .01																						//Each clock cycle is one hundreth of a millisecond, 1/5th of SIFS
#define SIFS .05																							//Time between packet receive and ACK sent
#define DIFS .1																								//Time after channel is marked idle that new packets cannot be sent
#define maxSpeed 13.75																						//If 100% traffic, no lost packets, this is max speed

double lambda = 0.9;
double mu = 2685;																							//Mu value such that average packet size is 512		
int numNetworks = 20;																						//N value from prompt
double simClock;

double lamdArr[7] = { .01, .05, .1, .3, .6, .8, .9 };														//Lambda and #Networks values for simulations
int numNArr[2] = { 10, 20 };

using namespace std;

int main(int argc, char* argv[])
{
	ofstream FILE;																							//Creating and filling a file with the simulation results for easy graphing
	FILE.open("output.csv");

	for (int j = 0; j < 14; j++)
	{
		lambda = lamdArr[j % 7] * 1000;
		numNetworks = numNArr[j / 7];
		cout << "Test " << j << " - Lambda: " << lambda << " - Number of Networks: " << numNetworks << endl;

		while (!network.empty())																			//Clearing list of networks between simulations
			network.pop_front();

		srand((unsigned int)time(NULL));																	//New Seed for Rand
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
			if (isSending && (busyTill <= simClock))														//If still marked as sending, but done sending, unmark
			{
				//cout << "Current simClock: " << simClock << endl;
				//cout << "Done Sending" << endl;
				isSending = false;
			}
			if ((busyTill <= simClock) && DIFSCounter)														//If not sending, and DIFS counter > 0, count down
			{
				DIFSCounter--;
			}

			if (SIFSCounter && (busyTill <= simClock))														//If SIFS counter is > 0 and not busy, send ACK
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

			if (!SIFSCounter && !DIFSCounter && (busyTill <= simClock))										//If done sending and both DIFS and SIFS are done, can check for next packet
			{
				list<host*> sending;
				for (list<host>::iterator it = network.begin(); it != network.end(); ++it)					//Iterate through list of hosts everytime
				{
					(*it).decSIFSTime();
					(*it).runNormalOp(simClock);

					if (!(*it).returnBackOff() && (*it).wantsToSend())										//If a packet is to be sent from given host THIS MOMENT, add to sending list
						sending.push_back(&*it);
					else
						(*it).decBackOff();
				}

				if (sending.size() == 1)																	//If only ONE person wants to send, send their packet
				{
					isSending = true;
					sending.front()->resetBOCounter();														//Reset BO Counter to 0
					SIFSCounter = (int)(SIFS / timeStep);													//Reset DIFS because something has sent
					sent = sending.front()->popOutgoing();
					sent.returnDest()->receivedPacket(sent);												//Sending packet to host
					bytesSent += sent.returnSize();
					totalDelay += (simClock - sent.returnTimeOC() + sent.returnTime());
					//cout << "Sending From: " << (*(sent.returnSend())).returnHostNumber() << " Sending To: " << (*(sent.returnDest())).returnHostNumber() << " Packet Number: " << sent.returnNumber() << "Packet Time: " << sent.returnTime() << endl;
					busyTill = sent.returnTime() + simClock;												//Set next busyTill to end of next packet transmission
					ACKSent = false;
				}
				else if (sending.size() > 1)																//If more than one person wants to send, tell them all to backoff
				{
					for (list<host*>::iterator it = sending.begin(); it != sending.end(); ++it)
						(*it)->setBackOff();
				}
			}
		}

		double through = ((double)bytesSent / totalTime) * timeStep;										//This block displays proper information based on simulations
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
		FILE << lambda << "," << numNetworks << "," << through << "," << avgDelay << endl;					//Write to and close file written
	}
	cout << "Simulations complete" << endl;
	FILE.close();
	char input;																								//Wait for user input to keep item open
	cin >> input;
	
	return 0;
}

