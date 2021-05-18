/********************************************************************
 * CSCI203 - Ass3
 * ass3.cpp - Paths
 * Date: 08/10/2018
 * Name: Oliver Rankin-Starcevic
 * Login: ors549
 * Email: ors549@uowmail.edu.au
 ********************************************************************/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
using namespace std;

//Globals
struct Vertex
{
	float x, y;
};
Vertex *location;
float **edges;
int nv, ne, startv, endv;

void method1();
bool noReachables(float[], int[]);
void method2();
void method3();
void output(float,int[],int,int[],int);

int main()
{		
	ifstream fin;
	char FileName[5];
	
	cout << "Please enter file name: "; //display a prompt for the file name
	cin >> FileName; //read in the file name

	fin.open(FileName); // try to open the file
	if (!fin.good()) // if the file fails to open
	{
		cout << endl << "Error: File not found";	
		exit(0); // print an error message on the screen and exit
	}
	
	fin >> nv; // number of vertices
	location = new Vertex[nv+1]; // xy coords of vertex
	edges	 = new float*[nv+1]; // edge matrix with costs
	for (int i = 0; i < nv+1; i++)
		edges[i] = new float[nv+1];
	
	
	int ver;
	for(int i = 0; i <= nv; i++)
	{	
		// initialize matrix
		for(int j = 0; j <= nv; j++)
		{
			edges[i][j] = 9999;
		}
		// load in coordinates
		if (i != nv)
			fin >> ver >> location[ver].x >> location[ver].y;
	}
	
	fin >> ne; // number of edges
	int e1, e2;
	float cost;
	for (int i = 0; i < ne; i++)
	{
		fin >> e1 >> e2 >> cost;
		edges[e1][e2] = cost;
		edges[e2][e1] = cost;
	}
	
	fin >> startv >> endv;
	
	method1();
	method2();
	method3();
}

void method1()
{
	//Modify Dijkstra's algorithm to stop as soon as a particular node is reached;
	//thus finding the shortest path between a specific pair of points.
	//This may involve the consideration of a number of points which do not lie on the final shortest path.
	//Your program should print an appropriate error message if no path exists between the specified nodes
	float D[nv+1], // reachable C weights
		smallest;	// smallest edge
	int P[nv], 		// Path
		C[nv], 		// Candidates
		S[nv], 		// Selected
		SItr = 0,	// selected itr
		v,			// min D
		ci,			// C[i]
		smallIdx;	// index of smallest vertex
	bool fail = false;	

	for(int i = 0; i < nv; i++) // initialize S, P & C
	{
		S[i] = -1;
		P[i] = startv;
		if (i+1 == startv) // dont put starting node in candidates
			C[i] = -1;
		else
			C[i] = i+1;
	}
		
	S[SItr] = startv;
	P[0] = startv;
	
	// init D
	D[0] = 9999;
	for (int i = 0; i < nv; i++)
	{
		if (C[i] == -1)
			D[i+1] = 9999;
		else
			D[i+1] = edges[startv][C[i]];
	}
	
	while (v != endv)
	{
		if (noReachables(D, C)) // no reachable nodes!
		{
			fail = true; 
			break;
		}
		
		//find v
	    smallest = 9999;
		smallIdx = -1;
		for (int i = 0; i < nv; i++)
		{
			if (C[i] != -1)
			{
				if (D[C[i]] < smallest)
				{
					smallest = D[C[i]];
					smallIdx = C[i];
				}
			}
		}
		
		v = smallIdx; // closest vertex
		SItr++;
		S[SItr] = v; // add v to S
		// set the index where v was to -1
		for (int i = 0; i < nv; i++)
			if (C[i] == v)
				C[i] = -1;
		
		// for each ver w in C
		for (int w = 0; w < nv; w++)
		{
			if (C[w] != -1)
			{
			 	if (D[C[w]] > (D[v] + edges[C[w]][v]))
				{
					D[C[w]] = D[v] + edges[C[w]][v];
					P[C[w]] = v; // add to path
				}				
			}
		}
	}
	
	// get path length
	int len = 1;
	int child = endv;
	while (child != startv)
	{
		len++;
		child = P[child];
	}
	
	// create path array and put path in it
	int path[len];
	child = endv;
	for (int i = 0; i < len; i++)
	{
		path[i] = child;
		child = P[child];
	}
	
	if (fail == true)
		cout << "ERROR: No such path exists!" << endl;
	else
		output(D[endv], path, len, S, 1); 
}

bool noReachables(float D[], int C[])
{
	for (int i = 0; i < nv; i++)
		if (D[C[i]]!= 9999)
			return false;
	return true;
}

void method2()
{
	//Modify the algorithm to add nodes to the solution based on an A* criterion derived
	//from the Euclidean (straight line) distance from each candidate node to the desired end node.
}

void method3()
{
	//Improve our efficiency by modifying Dijkstra's algorithm to start at both
	//the source and destination nodes and to construct two partial solution trees in parallel until
	//one node is in both partial solution trees.
}

void output(float length, int path[], int pathlen, int selected[], int solNum)
{
	// length
	cout << "The length of the shortest path from solution " << solNum << ": " << length << endl;
	
	// path
	cout << "The path from solution " << solNum << ": " << path[0];
	for (int i = 1; i < pathlen; i++)
		cout << ", " << path[i];
	cout << endl;
	
	//additional
	int additional = 0;
	bool inPath = false;
	// for each selected
	for (int i = 0; i < nv; i++)
	{
		inPath = false;
		for (int j = 0; j < pathlen; j++)
			// ignore if -1 or in path
			if (selected[i] == -1 || selected[i] == path[j])	
				inPath = true;
		if (inPath != true)
			additional++;
	}
	cout << "The number of additional nodes in the solution tree for solution " << solNum << ": " << additional << endl;
}
