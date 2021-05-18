/********************************************************************
 * CSCI203 - Ex5
 * Ex5.cpp - Hashing
 * Date: 26/08/2018
 * Name: Oliver Rankin-Starcevic
 * Login: ors549
 * Email: ors549@uowmail.edu.au
 ********************************************************************/
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

//Globals
struct Node
{
	int val;
	Node *next;
};

Node dictionary[100];
int depth;

void insertval(int);
void deleteval(int);
bool search(int);

int main()
{	
	for (int i = 0; i < 100; i++)
		dictionary[i].val= -1;
		
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
	
	int vinput;
	while (!fin.eof())
	{
		fin >> vinput;
		insertval(vinput);
	}
	
	//The number of empty entries in the hash table.
	//The length of the longest chain
	int empcnt = 0;
	for (int i = 0; i < 100; i++)
		if (dictionary[i].val == -1)
			empcnt++;
		
	cout << "Empty Entries: " << empcnt << endl;
	cout << "Longest Chain: " << depth << endl;	
}

void insertval(int vinput)
{
	int i = 1;
	int hash = vinput%100; // hashvalue
	if (!search(hash)) // new entry
	{
		dictionary[hash].val = vinput;
		dictionary[hash].next = NULL;
	}
	else
	{
		Node *crnt = dictionary[hash].next;
		if (crnt != NULL)
		{
			i++;
			while (crnt->next != NULL)
			{
				crnt = crnt->next;
			}
			crnt->next = new Node;
			crnt->next->val = vinput;
			crnt->next->next = NULL;
		}
		else
		{
			dictionary[hash].next = new Node;
			dictionary[hash].next->val = vinput;
			dictionary[hash].next->next = NULL;
			i++;
		}
		if (i > depth)
			depth = i;
	}
}

bool search(int hash)
{
	if (dictionary[hash].val == -1)
		return false;	
	return true;
}
/*
void deleteval(int key)
{
	Node *crnt = dictionary[key].next;
	while (crnt->next != NULL)
	{	
		tmp = crnt->next;
		delete crnt;
		crnt = tmp;
	}
	dictionary[key].val = -1;	
	dictionary[key].
}*/
