/********************************************************************
 * CSCI203 - Ex1
 * Ex1.cpp - Implementing a stack
 * Date: 29/07/2018
 * Name: Oliver Rankin-Starcevic
 * Login: ors549
 * Email: ors549@uowmail.edu.au
 ********************************************************************/
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

//Globals
string stacker[11];
int index = 0;

void push(string);
string top();
void pop();
bool isEmpty();

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
	
	while (!fin.eof())
	{	
		if (fin.fail()) // if the file read fails
			break; // terminate (break) the loop
		fin >> word; // read in a word from the file
		push(word); // Push the word onto the stack
	}
	fin.close(); // close the file
	
	while (!isEmpty()) // while the stack is not empty
	{
		cout << top() << " "; // display the top stack word on the screen followed by a space
		pop(); // pop the top value from the stack
	}
}

// Stacker Functions

void push(string s)
{
	index++;
	stacker[index] = s;
}

string top()
{
	return stacker[index];
}

void pop()
{
	if (index != 0)
	{
		stacker[index] = "";
		index--;		
	}
	else
		stacker[index] = "";
}
bool isEmpty()
{
	return (index == 0);
}
