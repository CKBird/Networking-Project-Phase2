/*
 * Networking Project
 * Phase 1 - Server Emulation
 * Chris Bird, Blake Tacklind
 * 
 * Declaration of event class and sub-class
 */

#ifndef EVENTS_H
#define EVENTS_H

#include <string>
using namespace std; 

//Event class, parent to arrivals and departure events
class events
{
public:
    events();
    ~events();
    virtual string toString()		{ return "Event"; }

    inline double returnTime()		{ return timeOE; }
    inline void setTimeOE(double time)	{ timeOE = time; }
private:

protected:
    double timeOE; 	//Time event occurred

};

//Class of arrival events
class arrival : public events 
{
public:
    static double generateTimeToNext(); //Not in source

    arrival(double currentTime);
    ~arrival();
    inline string toString()		{ return "ArrivalEvent"; }

private:

protected:

};

//Class of departure events
class departure : public events
{
public:
    departure();
    ~departure();
    double generateServiceTime();
    inline double getServiceTime()	{ return serviceTime; }
    inline string toString()		{ return "DepartureEvent"; }

private:
    double serviceTime;	//Time it takes server to process and send event

protected:

};

#endif