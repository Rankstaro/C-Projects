/********************************************************************
 * CSCI203 - Ex3
 * Ex3.cpp - Virtual Initialization
 * Date: 12/08/2018
 * Name: Oliver Rankin-Starcevic
 * Login: ors549
 * Email: ors549@uowmail.edu.au
 ********************************************************************/
#include <iostream>
#include <fstream>
using namespace std;

//Globals
int data[100];
int forward[100];
int backward[100];
int valid_count = 0;

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
	
	int what, where, probe;
	while (!fin.eof())
	{
		do 
		{
			fin >> what;
			fin >> where;
			if (what < 0 && where < 0)
				break;
			valid_count++;
			data[where] = what;
			forward[valid_count]=where;
			backward[where]=valid_count;
		}
		while (what > 0 && where > 0);
		
		do
		{
			fin >> probe;
			if (probe < 0)
				break;
			if (backward[probe] > 0 && backward[probe] <= valid_count && forward[backward[probe]]==probe)
				cout << "Position " << probe << " has been initialized to value " << data[probe] << "." << endl;
			else
				cout << "Position " << probe << " has not been initialized." << endl;
		}
		while (probe > 0);
	}
}
