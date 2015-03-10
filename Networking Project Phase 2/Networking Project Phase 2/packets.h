/*
 * Networking Project
 * Phase 1 - Server Emulation
 * Chris Bird, Blake Tacklind
 * 
 * Declaration of event class and sub-class
 */

#ifndef PACKETS_H
#define PACKETS_H

class host;

//#include "host.h"
#include <string>
#define sizeOfACK 64
extern double speedOfMedium;
extern double ACKTime;
using namespace std; 

//Event class, parent to arrivals and ACK packet
class packet
{
public:
	inline packet()										{;}
	inline ~packet()									{;}
    virtual string toString()							{ return "Packet"; }
	inline int returnSize(double sTime)					{ return (int)round((sTime * speedOfMedium) / 8); }
	inline int returnSize()								{ return size; }
	inline double returnTime()							{ return 1000 * ((8.0 * size) / speedOfMedium); }
	inline bool getAckValue()							{ return ack; }
	inline int returnNumber()							{ return packetNumber; }
	inline host* returnDest()							{ return destination; }
	inline host* returnSend()							{ return sender; }
	inline double returnTimeOC()						{ return timeOC; }

private:

protected:
	int size;					//Packet size
	double serviceTime;			//Time to process
	bool ack;					//Determines if packet is an ACK or not (can be ambiguous based on size if random-generated size is 64)
	int packetNumber;			//Identifier for packet
	host* destination;			//Is destination for packet
	host* sender;				//Points to sender of packet (us)
	double timeOC;

};

//Class of sentData packet
class sentData : public packet 
{
public:
    sentData(int pNum, host* destination, host* send);
    ~sentData();
	double generatePacketServiceTime();
    inline string toString()							{ return "Data"; }

private:

protected:

};

//Class of ACK packet
class ACK : public packet
{
public:
	ACK(int pNum, host* destination, host* send);
    ~ACK();
    inline string toString()							{ return "ACK"; }
	inline bool decSIFS()								{ return !(--SIFStime); }

private:

protected:
	double timeQueued;
	int SIFStime;
};

#endif
