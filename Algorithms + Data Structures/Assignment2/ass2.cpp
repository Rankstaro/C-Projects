/********************************************************************
 * CSCI203 - Assignment2
 * ass2.cpp - Discrete Simulation Queues
 * Date: 10/09/2018
 * Name: Oliver Rankin-Starcevic
 * Login: ors549
 * Email: ors549@uowmail.edu.au
 ********************************************************************/
#include <iostream>
#include <fstream>

using namespace std;

// --- Globals ---
ifstream fin;

enum EventType { arrive, primary, secondary };
struct Event 
{ 
	EventType eventType;
	double occurTime, priTime, secTime;
	int serNum;
};
Event events[999]; // min heap of events
Event priQue[999];	// primary queue
Event secQue[999];	// secondary queue

struct Server
{
	bool busy;
	double idleTime;
};
Server *priServer, *secServer;

int eventIdx = 0, priQueStart = 0, secQueStart = 0, 
	priQueNxt = 0, secQueNxt = 0, 
	priSerNum, secSerNum, totPplSer,
	freePri, freeSec;
	
double timer = 0, etime, lastSerTime, 
	avgSerTime, avgQueTime, avgPriQueTime, avgSecQueTime, 
	avgQueLen, avgPriQueLen, avgSecQueLen,
	maxQueLen = 0, maxPriQueLen = 0, maxSecQueLen = 0;

void process(Event);
void printStats();
void createEvent(EventType, double, double, double, int);
void pushEvent(Event);
void popEvent();
void siftup(int);
void siftdown(int);
void swap(int,int);
void enPriQueue(Event);
void dePriQueue();
void enSecQueue(Event);
void deSecQueue();

int main()
{
	for (int i = 0; i < 999; i++) // init queues
	{
		priQue[i].occurTime = -1;
		secQue[i].occurTime = -1;
	}
	
	char FileName[10];
	
	cout << "Please enter file name: ";// read file name
	cin >> FileName;
	
	fin.open(FileName);
	if (!fin.good())
	{
		cout << endl << "Error: File not found";
		exit(0); // print an error message on the screen and exit
	}
	
	fin >> priSerNum >> secSerNum;// initialize servers
	
	priServer = new Server [priSerNum];
	secServer = new Server [secSerNum];
	
	for (int i = 0; i < priSerNum; i++) //initialize
	{
		priServer[i].busy = false;
		priServer[i].idleTime = 0;
	}
	for (int i = 0; i < secSerNum; i++) //initialize
	{
		secServer[i].busy = false;
		secServer[i].idleTime = 0;
	}
	
	double a, p, s; // first event
	fin >> a >> p >> s;
	createEvent(arrive, a, p, s, -1);
	
	while (events[0].occurTime > 0)
	{
		process(events[0]);	
		popEvent();
	}
	
	// stats
	lastSerTime = timer;
	avgSerTime += avgPriQueTime + avgSecQueTime; // add waiting times
	avgSerTime /= totPplSer;
	avgPriQueTime /= totPplSer;
	avgSecQueTime /= totPplSer;
	avgQueTime = avgPriQueTime + avgSecQueTime;
	avgPriQueLen /= timer;
	avgSecQueLen /= timer;
	avgQueLen = avgPriQueLen + avgSecQueLen;
	if (maxPriQueLen > maxSecQueLen)
		maxQueLen = maxPriQueLen;
	else
		maxQueLen = maxSecQueLen;
	for (int i = 0; i < priSerNum; i++)
	{
		priServer[i].idleTime =timer- priServer[i].idleTime;
	}
	for (int i = 0; i < secSerNum; i++)
	{
		secServer[i].idleTime =timer- secServer[i].idleTime;
	}
	
	printStats();
	
	delete priServer, secServer;
	fin.close();
}

// **************** Main Functions *****************

void process(Event event)
{
	etime = event.occurTime-timer; // elapsed time since last event
	timer = event.occurTime;
	//Customer arrives
	if (event.eventType == arrive)
	{
		for (int i = 0; i < priSerNum; i++)// find available server
		{
			if (!priServer[i].busy)
			{
				freePri = i;	
				break;
			}
			else
				freePri = -1;
		}
		if (freePri != -1) // primary server idle, move straight to primary server
		{
			priServer[freePri].busy = true;
			createEvent (primary, timer+event.priTime, event.priTime, event.secTime, freePri); // create leave prim event
		}
		else // all servers are busy
		{
			enPriQueue(event); //add to prim queue
		}
		// create arrival event for next customer
		double o, p, s;
		fin >> o >> p >> s;
		createEvent (arrive, o, p, s, -1);
		
		// Stats
		totPplSer++;
		avgSerTime += event.priTime + event.secTime;
	}

	//Finished at primary
	else if (event.eventType == primary)
	{
		priServer[event.serNum].busy = false; // Free primary server
		for (int i = 0; i < secSerNum; i++)
		{
			if (!secServer[i].busy)
			{
				freeSec = i;	
				break;
			}
			else
				freeSec = -1;
		}
		if (freeSec != -1) // secondary server idle, move straight to secondary server
		{
			secServer[freeSec].busy = true;
			createEvent (secondary, timer+event.secTime, event.priTime, event.secTime, freeSec); // create leave second event
		}
		else // All servers busy
		{
			enSecQueue(event);	
		}
		
		if (priQue[priQueNxt].occurTime > 0) // Customer is waiting in primary queue
		{
			// stats
			avgPriQueTime +=timer- priQue[priQueNxt].occurTime;

			createEvent (primary, timer+priQue[priQueNxt].priTime, priQue[priQueNxt].priTime, priQue[priQueNxt].secTime, event.serNum); // create leave prim event
			dePriQueue(); // pop from pri queue
			priServer[event.serNum].busy = true;// set 1st server to busy
		}
	}
	
	//Finished at secondary
	else
	{
		secServer[event.serNum].busy = false; // free secondary server
		if (secQue[secQueNxt].occurTime > 0) // customer is waiting in secondary queue
		{
			// stats
			avgSecQueTime +=timer- secQue[secQueNxt].occurTime;
			
			createEvent (secondary, timer+secQue[secQueNxt].secTime, secQue[secQueNxt].priTime, secQue[secQueNxt].secTime, event.serNum); // create leave sec event
			deSecQueue(); // pop from sec queue
			secServer[event.serNum].busy = true;// set 2nd server to busy			
		}
	}
	// averages
	int priQueLen = (priQueNxt-priQueStart), 
		secQueLen = (secQueNxt-secQueStart);
	avgPriQueLen += etime*priQueLen;
	avgSecQueLen += etime*secQueLen;
	// maximums
	if (priQueLen > maxPriQueLen)
		maxPriQueLen = priQueLen;
	if (secQueLen > maxSecQueLen)
		maxSecQueLen = secQueLen;
	// add busy times
	for (int i = 0; i < priSerNum; i++)
	{
		if (priServer[i].busy)
			priServer[i].idleTime += etime;
	}
	for (int i = 0; i < secSerNum; i++)
	{
		if (secServer[i].busy)
			secServer[i].idleTime += etime;
	}
}

void printStats()
{ 
	double scale1 = 0.01;
	double scale2 = 0.001;
	cout << "*************************** OUTPUT ***************************" << endl;
	cout << "Number of people served:.........." << totPplSer << endl;
	cout << "Last service completion time:....." << (int)(lastSerTime/scale2)*scale2 << endl;
	cout << "Average total service time:......." << (int)(avgSerTime/scale2)*scale2 << endl;
	cout << "Average total time in queues:....." << (int)(avgQueTime/scale1)*scale1 << endl;
	cout << "Average length of queues:........." << (int)(avgQueLen/scale1)*scale1 << endl;
	cout << "Maximum length of queues:........." << maxQueLen << endl << endl;
	cout << "== Primary ===================================================" << endl;
	cout << "Average total time in queue:......" << (int)(avgPriQueTime/scale1)*scale1 << endl;
	cout << "Average length of queue:.........." << (int)(avgPriQueLen/scale1)*scale1 << endl;
	cout << "Maximum length of queue:.........." << maxPriQueLen << endl;
	for (int i = 0; i < priSerNum; i++)
		cout << "Server " << i+1 << " total idle time:........." << (int)(priServer[i].idleTime/scale1)*scale1 << endl;
	cout << endl;
	cout << "== Secondary =================================================" << endl;
	cout << "Average total time in queue:......" << (int)(avgSecQueTime/scale1)*scale1 << endl;
	cout << "Average length of queue:.........." << (int)(avgSecQueLen/scale1)*scale1 << endl;
	cout << "Maximum length of queue:.........." << maxSecQueLen << endl;
	for (int i = 0; i < secSerNum; i++)
		cout << "Server " << i+1 << " total idle time:........." << (int)(secServer[i].idleTime/scale1)*scale1 << endl;
	cout << endl;
}

// ********************** HEAP **********************

void createEvent(EventType eType, double oTime, double pTime, double sTime, int server)
{
	Event event;
	event.eventType = eType;
	event.occurTime	= oTime;
	event.priTime 	= pTime;
	event.secTime	= sTime;
	event.serNum	= server;
	pushEvent(event);
}

void pushEvent(Event event)
{
	events[eventIdx] = event;
	siftup(eventIdx);	
	eventIdx++;
}

void popEvent()
{
	if (events[eventIdx-1].occurTime < 0) // terminator has been processed
	{ 
		events[0] = events[eventIdx-2]; // 0 = last event
		events[eventIdx-2] = events[eventIdx-1]; // last event = terminator
	}
	else
		events[0] = events[eventIdx-1]; // 0 = last event
	if (events[0].occurTime > 0 && eventIdx > 1) // not terminator or first idx
		siftdown(0); // sift last event down, sorting into heap
	eventIdx--;
}

void siftdown(int p)
{
	int c; // left child
	if (p == 0)
		c = 1;
	else
		c = (p * 2); 
		
	if ( c < eventIdx && events[c].occurTime > 0) // left child is in range and is not a terminator
	{
		if (events[c].occurTime > events[c+1].occurTime // left child is greater than right child
			&& events[c+1].occurTime > 0 	// right child is not a terminator
			&& c+1 < eventIdx) 	 			// right child is in range
			c++; // c = right child
		if (events[c].occurTime < events[p].occurTime) // smallest child is less than parent
		{
			swap (p, c);
			siftdown(c);
		}
	}
}

void siftup(int i)
{	
	if (i != 0 && events[i].occurTime > 0) // not root or terminator
	{
		int p = i / 2; // parent
		if (events[p].occurTime > events[i].occurTime) // violates min heap
		{
			swap (i, p);
			siftup(p);
		}
	}
}

void swap(int a, int b)
{
	Event tmp = events[a];
	events[a] = events[b];
	events[b] = tmp;
}

// ****************** QUEUES ********************

void enPriQueue(Event event)
{
	priQueNxt++;
	if (priQueNxt > (sizeof(priQue)/sizeof(Event)))
		priQueNxt = 1;
	priQue[priQueNxt] = event;
}

void dePriQueue()
{  
	priQueStart++;
	if (priQueStart > (sizeof(priQue)/sizeof(Event)))
		priQueStart = 1;
	priQue[priQueStart].occurTime = -1;
}

void enSecQueue(Event event)
{
	secQueNxt++;
	if (secQueNxt > (sizeof(secQue)/sizeof(Event)))
		secQueNxt = 1;
	secQue[secQueNxt] = event;	
}

void deSecQueue()
{
	secQueStart++;
	if (secQueStart > (sizeof(secQue)/sizeof(Event)))
		secQueStart = 1;
	secQue[secQueStart].occurTime = -1;
	
}
