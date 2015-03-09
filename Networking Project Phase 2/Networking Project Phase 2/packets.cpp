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

using namespace std;
double speedOfMedium = (11 * (10 ^ 6));
double ACKTime = ((8.0 * sizeOfACK) / speedOfMedium);
float lambda;
static int timeToNext;
extern double simClock;

sentData::sentData(int pNum, host* dest, host* send)
{
	size = returnSize(serviceTime = generatePacketServiceTime()); //Creates time and size of packet
	ack = false;
	packetNumber = pNum;
	destination = dest;
	sender = send;
};

sentData::~sentData()
{
  //Destructor
};

//Generates the time to the next arriving packet
double sentData::generatePacketServiceTime()
{  
    double u; //w
    u = (rand() / (RAND_MAX + 1.0));
    double temp = ((-1 / lambda) * log(1 - u)); //Make sure that we convert from double to int before returning
	
	if (temp > maxTime)
		temp = maxTime;
	
	return temp;
};

//ACK Constructor is inline

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
