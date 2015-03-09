#include "stdafx.h"
#include "host.h"
#include "packets.h"
#include "stdio.h"
#include "iostream"
#include "time.h"
#include <queue>
#include <list>
#include <stack>

#define MRT 5

using namespace std;
extern double lambda;
list<host> network;

host::host(int hNum)
{
	packetNumber = 0;
	hostNumber = hNum;
};

host::~host()
{
	//Destructor
};

double host::generateTimeToNext()
{
	double u; //w
	u = (rand() / (RAND_MAX + 1.0));
	return ((-1 / lambda) * log(1 - u));												//Make sure that we convert from double to int before returning
};

void host::receivedPacket(packet Input)
{
	//Wait receive time
	if (Input.getAckValue())															//If packet received is an ACK
	{
		list<packet>::iterator it = unACK.begin(); 
		while (it != unACK.end() || (*it).returnNumber() != Input.returnNumber())		//After this, we have *it pointing to sentPacket in unACK list
			++it;

		unACK.erase(it);																//Erases packet from unACK'd list, saying it was successful
	}
	else																				//If packet received is data
	{
		unprocACK.push_back(ACK(Input.returnNumber(), Input.returnSend(), this));		//Put new ACK onto unproccessed ACK queue to wait their time
	}
};

void host::setBackOff()
{
	double u = (rand() / (RAND_MAX + 1.0));
	u *= MRT;
	u *= ++failedBOCounter;
	backOffCounter = (int)(round(u));
}

void host::decBackOff()
{
	if (backOffCounter)
		backOffCounter--;
}

packet host::popOutgoing()
{
	packet temp = outgoingBuffer.front();
	outgoingBuffer.pop();

	if (!(temp.getAckValue()))
		unACK.push_back(temp);

	return temp;
}

host* host::returnRandHost()
{
	int dest;
	do{ (dest = rand() % network.size()); } while (dest != this->hostNumber);			//Sets dest to a random non-this host

	list<host>::iterator it = network.begin();

	for (int i = 0; i < dest; i++)															//Gets iterator to host chosen by dest
		++it;

	return &*it;																		//Returns pointer to host chosen
}

void host::decSIFSTime()																//Decrements all SIFS times by 1 step (of 5)
{
	for (list<ACK>::iterator it = unprocACK.begin(); it != unprocACK.end(); ++it)
	{
		if ((*it).decSIFS())															//Decrements, and if it's 0, puts onto outgoing queue
			outgoingBuffer.push(*it);
	}
}