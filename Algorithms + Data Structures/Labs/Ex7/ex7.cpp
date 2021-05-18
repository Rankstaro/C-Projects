/********************************************************************
 * CSCI203 - Ex7
 * Ex6.cpp - Breadth first search
 * Date: 24/09/2018
 * Name: Oliver Rankin-Starcevic
 * Login: ors549
 * Email: ors549@uowmail.edu.au
 ********************************************************************/
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
using namespace std;

//Globals
struct Node
{
	int v;
	Node *next;
};

int n, *q, start = 0, next = 0,
	*ver, verIdx = 0, *p;
Node *l;

void insert(int,int);
void bFS(int);
bool findV(int, int[], int[]);
void enqueue(int);
int dequeue();


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
	
	fin >> n;
	l = new Node[n]; // linked list of adjacent vertices to vertex n
	ver = new int[n];  // order of BST
	q = new int[n]; // queue of vertex
	p = new int[n];  // array of parents
	for(int i = 0; i < n; i++)// init q, ver & l
	{
		q[i] = -1;
		ver[i] = -1;
		l[i].v = -1;
	}
	
	
	int v1, v2;
	while (!fin.eof())
	{
		fin >> v1 >> v2;
		insert(v1,v2);
		insert(v2,v1);
	}	
	
	bFS(0);
	
	for (int i = 1; i < n; i++)
	{
		cout << p[ver[i]] << "	" << ver[i] << endl;
	}
	
	delete [] l;
}

void insert(int v1, int v2)
{	
	Node *crnt;
	if (l[v1].v == -1)
	{
		l[v1].v = v2;
		l[v1].next = NULL;
	}
	else
	{
		crnt = l[v1].next;
		if (crnt != NULL)
		{
			while (crnt->next != NULL)
				crnt = crnt->next;
			crnt->next = new Node;
			crnt->next->v = v2;
			crnt->next->next = NULL;
		}
		else
		{
			l[v1].next = new Node;
			l[v1].next->v = v2;
			l[v1].next->next = NULL;
		}
	}
}

// output all of the nodes reachable from this node.
void bFS(int s)
{
	p[s] = -1;
	enqueue(s);
	while (q[start] != -1)
	{
		int c = dequeue();
		ver[verIdx] = c; // add c to ver
		verIdx++;
		
		if (!findV(l[c].v, ver, q)) // if l[c].v not in ver
		{
			enqueue(l[c].v);
			p[l[c].v] = c; // parent
		}
		Node *crnt = l[c].next;
		if (crnt != NULL)
		{
			if (!findV(l[c].next->v, ver, q))//if crnt v is not in ver then
			{
				enqueue(l[c].next->v);
				p[l[c].next->v] = c; // parent
			}
			
			while (crnt->next != NULL) //for each n in adj(c)
			{
				if (!findV(crnt->next->v, ver, q))//if crnt v is not in ver then
				{
					enqueue(crnt->next->v);
					p[crnt->next->v] = c; //parent
				}
				crnt = crnt->next;
			}
		}
	}
}

bool findV (int v, int arr1[], int arr2[])
{
	for (int i = 0; i < n; i++)
	{
		if (arr1[i] == v)
			return true;
		if (arr2[i] == v)
			return true;
	}
	return false;
}

void enqueue(int i)
{
	q[next] = i;
	next++;
	if (next > n)
		next = 0;
}

int dequeue()
{
	int c = q[start];
	q[start] = -1;
	start++;
	if (start > n)
		start = 0;
	return c;
}

