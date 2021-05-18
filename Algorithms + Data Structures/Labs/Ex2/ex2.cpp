
/********************************************************************
 * CSCI203 - Ex2
 * Ex1.cpp - Implementing a heap
 * Date: 05/08/2018
 * Name: Oliver Rankin-Starcevic
 * Login: ors549
 * Email: ors549@uowmail.edu.au
 ********************************************************************/
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

//Globals
int heaper[500];
int index = 0;

void siftup(int);
void makeheap();
void siftdown(int);
void swap(int,int);

int main()
{	
ifstream fin;
	char FileName[5];
	string word;
	
	cout << "Please enter file name: "; //display a prompt for the file name
	cin >> FileName; //read in the file name

	fin.open(FileName); // try to open the file
	if (!fin.good()) // if the file fails to open
	{
		cout << endl << "Error: File not found";	
		exit(0); // print an error message on the screen and exit
	}
	
	int input;
	while (!fin.eof())
	{
		fin >> input;
		index++;
		heaper[index] = input;
	}
	
	for (int i = 1; i <= index; i++)
		siftup(i);
		
	cout << "SIFT UP" << endl;
	for (int i = 1; i <= 10; i++) // for i = 1 to 5
		cout << heaper[i] << " "; // print the ith element of the heap
	
	cout << endl << "MAKEHEAP" << endl;
	
	makeheap();
	
	fin.close(); // close the file
	
	for (int i = 1; i <= 10; i++) // for i = 1 to 5
		cout << heaper[i] << " "; // print the ith element of the heap
}



void makeheap()
{
	for (int i = (index/2); i >= 1; i--)
		siftdown(i);
}

void siftup(int i)
{
	if (i != 1)
	{
		int p = i / 2; // integer division
		if (heaper[p] >= heaper[i])
		{
			swap (i, p);
			siftup(p);
		}
	}
}
void siftdown(int i)
{
	int c = (i * 2);
	if (heaper[c] < heaper[c + 1])
		c++;
	if (heaper[i] < heaper[c])
	{
		swap (i, c);
		siftdown(c);
	}
}

void swap(int a, int b)
{
	int tmp = heaper[a];
	heaper[a] = heaper[b];
	heaper[b] = tmp;
}
