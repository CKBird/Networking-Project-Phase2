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
	inline int returnSize(double sTime)					{ return (int)round((sTime * speedOfMedium) / 8); }				//Returns size of packet and sets it
	inline int returnSize()								{ return size; }												//Returns size of current packet
	inline double returnTime()							{ return 1000 * ((8.0 * size) / speedOfMedium); }				//Returns time to process packet
	inline bool getAckValue()							{ return ack; }													//Tells if packet is ACK or not
	inline int returnNumber()							{ return packetNumber; }										//Unique number given to packet to differentiate from others from same host
	inline host* returnDest()							{ return destination; }											//Tells destination of packet
	inline host* returnSend()							{ return sender; }												//Gives access to name of host sending packets
	inline double returnTimeOC()						{ return timeOC; }												//Returns the time that the packet was created

private:

protected:
	int size;											//Packet size
	double serviceTime;									//Time to process
	bool ack;											//Determines if packet is an ACK or not (can be ambiguous based on size if random-generated size is 64)
	int packetNumber;									//Identifier for packet
	host* destination;									//Is destination for packet
	host* sender;										//Points to sender of packet (us)
	double timeOC;

};

class sentData : public packet																							//Class of sentData packet 
{
public:
    sentData(int pNum, host* destination, host* send);
    ~sentData();
	double generatePacketServiceTime();
    inline string toString()							{ return "Data"; }

private:

protected:

};

class ACK : public packet																								//Class of ACK packet
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
