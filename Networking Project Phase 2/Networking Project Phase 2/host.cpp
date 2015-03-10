//host.cpp
//Chris Bird and Blake Tacklind
//ECS 152A/EEC 173 Computer Networks

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
	TTNSend = generateTimeToNext();
	backOffCounter = 0;
	failedBOCounter = 0;
};

host::~host()
{
	//Destructor
};

double host::generateTimeToNext()
{
	double u; //w
	u = (rand() / (RAND_MAX + 1.0));
	double temp = (((-1 / lambda) * log(1 - u)) * 1000);								//Make sure that we convert from double to int before returning
	return temp;
};

void host::receivedPacket(packet Input)
{
	//Wait receive time
	if (Input.getAckValue())															//If packet received is an ACK
	{
		//ACK Received!
		/*if (!unACK.empty())
		{
			list<packet>::iterator it = unACK.begin();
			while (it != unACK.end() || (*it).returnNumber() != Input.returnNumber())	//After this, we have *it pointing to sentPacket in unACK list
				++it;

			unACK.erase(it);															//Erases packet from unACK'd list, saying it was successful
		}*/
	}
	else																				//If packet received is data
	{
		//unprocACK.push_back(ACK(Input.returnNumber(), Input.returnSend(), this));		//Put new ACK onto unproccessed ACK queue to wait their time
	}
};

void host::setBackOff()
{
	double u = (rand() / (RAND_MAX + 1.0));
	u *= MRT;
	u *= ++failedBOCounter;
	backOffCounter = (int)u;
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
	return temp;
}

host* host::returnRandHost()
{
	int dest;
	do{ (dest = rand() % network.size()); } while (dest == this->hostNumber);			//Sets dest to a random non-this host

	list<host>::iterator it = network.begin();

	for (int i = 0; i < dest; i++)														//Gets iterator to host chosen by dest
		++it;

	return &*it;																		//Returns pointer to host chosen
}

void host::decSIFSTime()																//Decrements all SIFS times by 1 step (of 5)
{
	if (!unprocACK.empty())																//If list is empty, do not use
		for (list<ACK>::iterator it = unprocACK.begin(); it != unprocACK.end(); ++it)
			if ((*it).decSIFS())														//Decrements, and if it's 0, puts onto outgoing queue
				outgoingBuffer.push(*it);
}

void host::runNormalOp(double sClock)
{
	if (TTNSend <= sClock)
	{
		outgoingBuffer.push(sentData(packetNumber++, returnRandHost(), this));			//Puts new packet into outgoingBuffer after TTN has arrived
		TTNSend = sClock + generateTimeToNext();										//Creates next timestamp when a new packet will be sent
	}
}