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
#include "events.h"

using namespace std;

float mu, lambda;

//Extra credit distribution boolean - not yet implemented
#define extraCredit 0

static int timeToNext;

events::events()
{
  //Constructor
};

events::~events()
{
  //Destructor
};

arrival::arrival(double currentTime)
{
  //Constructor
  timeOE = currentTime;
};

arrival::~arrival()
{
  //Destructor
};

//Generates the time to the next arriving packet
double arrival::generateTimeToNext()
{
  if (!extraCredit)
  {
    double u; //w
    u = (rand() / (RAND_MAX + 1.0));
    return ((-1 / lambda) * log(1 - u)); //Make sure that we convert from double to int before returning
  }
  else
  {
    double u; //w
    u = (rand() / (RAND_MAX + 1.0));
    return ((-1 / lambda) * log(1 - u)); //Make sure that we convert from double to int before returning
  }
}

departure::departure()
{
  //Constructor
  serviceTime = generateServiceTime();
};

departure::~departure()
{
  //Destructor
};

//Generates the service time for particular departure event
double departure::generateServiceTime()
{
  if (!extraCredit)
  {
    //Using Mu to generate service time of CURRENT event
    double u; //w
    u = (rand() / (RAND_MAX + 1.0));
    return ((-1 / mu) * log(1 - u));
  }
  else
  {
    //Using Mu to generate service time of CURRENT event
    double u; //w
    u = (rand() / (RAND_MAX + 1.0));
    return ((-1 / mu) * log(1 - u));
  }
}