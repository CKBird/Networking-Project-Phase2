/*
 * Networking Project
 * Phase 1 - Server Emulation
 * Chris Bird, Blake Tacklind
 * 
 * This Program is designed to emulate a server that receives and handles 
 * incoming packets using a specific distribution for time between arrivals
 * and time to service. This server is emulated with a buffer and link node.
 * 
 * This is the main where the simulation is run and the servers buffer queue
 * and event handling is emulated
 */

#include "events.h"
#include "stdio.h"
#include "iostream"
#include "time.h"
#include <queue>
#include <list>
#include <stack>

using namespace std;

//globally used lambda and mu
extern float mu, lambda;

int main(int argc, char* argv[])
{
  //Experimental values
  mu = 1;
  float lambdaArr[7] = { (float).1, (float).25, (float).4, (float).55, 
    (float).65, (float).80, (float).90 };
  float lamdaArr2[5] = { (float).2, (float).4, (float).6, (float).8, 
    (float).9 };
  unsigned int buffMax[3] = { 1, 20, 50 };

  int BUFFER_INF = 0;
  unsigned int BUFFER_MAX = 0;

  //Experimental loop
  for (int numExp = 0; numExp < 22; numExp++)
  {
    //Set up this run's experimental values (lambdas, and buffer size)
    if (numExp < 7){
      BUFFER_INF = 1;
      lambda = lambdaArr[numExp];
    }
    else{
      BUFFER_INF = 0;
      lambda = lamdaArr2[(numExp - 7) % 5];
      BUFFER_MAX = buffMax[(int)((numExp - 7) / 5)];
    }

    //Initialize variables for simulation
    srand((unsigned int)time(NULL));
    double currentTime = 0;
    double NAT = arrival::generateTimeToNext();	//Next Arrival Time
    double NDT = 0;				//Next Departure Time
    double downTime = 0.0;
    double totalSize = 0;
    int dropped = 0;

    queue<departure> buffer;	//NOTE: top of buffer is link node
    list<events> GEL;
    stack<double> integralCalc;

    //Simulation Loop
    //One event processed per loop. Ends depending on time of last event
    while (currentTime < 100000)			
    {
      //Arrival is the next event to occur
      if ((NAT < NDT) || buffer.empty()) 	
      {
        //Advance currentTime to next event
        currentTime = NAT;

        //Only time server is not busy is if it not currently trying to send a 
        //packet when a new packet arrives. This is detected by checking if the 
        //next packet to leave happened in the past. Therefore add the 
        //difference to the total down time.
        if (NDT < currentTime)
          downTime += (currentTime - NDT);

        //Adds new arrival with time NAT to GEL
        GEL.push_back(arrival(currentTime));		

        //Check if buffer is full
        if (!BUFFER_INF && (buffer.size() > BUFFER_MAX)){ 
          dropped++;
        }
        else{
          //Puts new departure on buffer
          buffer.push(departure());

          //if the buffer was empty set the new Next Departure Time
          if (buffer.size() == 1)
            NDT = currentTime + buffer.front().getServiceTime();

          //Add element to integral stack. Pushes to the stack the time when 
          //the buffer size increases to value equal to elements in the stack.
          if(buffer.size() > 1)
            integralCalc.push(currentTime);
        }

        //Generate time for the arrival of next packet
        NAT = currentTime + arrival::generateTimeToNext();
      }
      else 		//Departure is the next event to occur
      {
        //Advance currentTime to next event
        currentTime = NDT;

        //pops the element off integral calculator stack, essentially point when
        //it rose to a value and subtracts it by the time it drops from a value. 
        //This gets us the area of the time it was at that value.
        if(!integralCalc.empty()){
          totalSize += currentTime - integralCalc.top();
          integralCalc.pop();
        }

        //Add departure event to list of events
        GEL.push_back(buffer.front());    
        //Set the time of the the event to current time
        GEL.back().setTimeOE(currentTime);
        //remove departure event from link node
        buffer.pop();                     

        //Set NDT to the time of the NEXT departure (currentTime + serviceTime)
        if (!buffer.empty())
          NDT = currentTime + buffer.front().getServiceTime(); 
        //cout << "d";
      }
      //cout << currentTime << "__" << endl;
    }
    //Simulation ended

    //Empty the integral Calculator Stack
    while(!integralCalc.empty()){
      totalSize += currentTime - integralCalc.top();
      integralCalc.pop();
    }

    //Calculate Statistics 
    double meanSize = totalSize / currentTime;
    double fractionBusy = (downTime / currentTime);// * 100;

    //Print Statistics of run
    cout << endl << "Lambda Value: " << lambda << endl;
    cout << "Buffer Size: " << BUFFER_MAX << endl;
    cout << "Downtime: " << fractionBusy <<endl;
    cout << "Buffer Mean: " << meanSize << endl;
    cout << "Dropped packets: " << dropped << endl;
  }
  //Experimental run ended
  cout << "Done creating statistics" << endl;

  //Ending Program
  char input;
  cin >> input;
  return 0;
}

