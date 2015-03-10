//Header for Host.cpp
#ifndef HOST_H
#define HOST_H

#include "packets.h"
#include "math.h"
#include <queue>
#include <list>

extern list<host> network;

using namespace std;

class host
{
public:
	host(int hNum);
	~host();
	double generateTimeToNext();			
	void receivedPacket(packet Input);												//Manages how to handle received packets
	void setBackOff();																//Randomly generate time until next on queue can be sent
	inline int returnBackOff()				{ return backOffCounter; }				//Gives value of backoff to network
	void decBackOff();																//Reduces Back Off counter by one each time loop
	packet popOutgoing();															//Pops front of queue to send
	host* returnRandHost();															//Decide on a host for the packet to go to
	inline packet* peekOutgoing()			{ return &(outgoingBuffer.front()); }	//Lets us look at front of queue from main
	void decSIFSTime();																//Decreases all unproccessed ACKs by one cycle
	inline void resetBOCounter()			{ failedBOCounter = 0; }				//Once packet sends, set failed BO counter back to 0
	void runNormalOp(double sClock);												//Determine if TTNSend is <= simClock, if so, add to outgoing buffer
	inline int returnHostNumber()			{ return hostNumber; }					//Identify yourself fool!
	inline bool wantsToSend()				{ return !outgoingBuffer.empty(); }		//Returns if it has something to send

private:

protected:
	queue<packet> outgoingBuffer;			//Buffer for outgoing packets, ACK and data
	list<ACK> unprocACK;					//All ACKs that have not waited their SIFS time go here
	//list<packet> unACK;					//List of unacknowlged packets
	int packetNumber;						//Unique number given toa ll sent data packets, ACKs get number pased on sentData
	int hostNumber;							//Unique number for host
	int backOffCounter;						//Counter until it is able to try and send again
	int failedBOCounter;					//Every time we fail to send after BO reaches 0, increase time of next BO by failedBOCounter
	double TTNSend;							//Time To Next Send, used for determining time between current simClock and packet send attempt
};

#endif