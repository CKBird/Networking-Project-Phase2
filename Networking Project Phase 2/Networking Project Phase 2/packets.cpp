/*
 * Networking Project
 * Phase 1 - Server Emulation
 * Chris Bird, Blake Tacklind
 * 
 * Definition of event class and sub-class
 */

#include "stdafx.h"
#include "stdio.h"
#include "iostream"
#include "stdlib.h"
#include "math.h"
#include <queue>
#include <list>
#include "packets.h"

#define maxTime 1.12
#define maxSize 1544

using namespace std;
double speedOfMedium = (11 * (1000000));																		//This is the speed of the network (11 Mbps)
double ACKTime = ((8.0 * sizeOfACK) / speedOfMedium);															//Time it takes to send an ACK
float lambda;
static int timeToNext;																							//Time until next packet is to be sent
extern double simClock;
extern double mu;

sentData::sentData(int pNum, host* dest, host* send)
{
	size = returnSize(serviceTime = generatePacketServiceTime());												//Creates time and size of packet
	if (size > maxSize)																							//If randomly generated size is greater than max size, set to max size
		size = maxSize;
	ack = false;
	packetNumber = pNum;
	destination = dest;
	sender = send;
	timeOC = simClock;
};

sentData::~sentData()
{
  //Destructor
};

double sentData::generatePacketServiceTime()																	//Generates the time to the next arriving packet
{  
    double u; //w
    u = (rand() / (RAND_MAX + 1.0));
    double temp = ((-1 / mu) * log(1 - u));																		//Make sure that we convert from double to int before returning
	return temp;
};

ACK::ACK(int pNum, host* dest, host* send)
{
	size = 64; 
	serviceTime = returnTime(); 
	ack = true;
	packetNumber = pNum;
	destination = dest;
	sender = send;
	timeQueued = simClock;
	SIFStime = 5;
}

ACK::~ACK()
{
  //Destructor
};
