/********************************************************************
 * CSCI203 - Assignment1
 * ass1.cpp - Virtual Initialization
 * Date: 12/08/2018
 * Name: Oliver Rankin-Starcevic
 * Login: ors549
 * Email: ors549@uowmail.edu.au
 ********************************************************************/
#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <sstream>

using namespace std;

//Globals
char words[500000];
int windex = 0;
int start[50000];
int sindex = 0;
int count[50000]; // count and start share sindex.


void process(string);
void store(string);
void sort();
void printfirst();
void printlast();

int main()
{	
	// 1. Read the name of the text file from the console.
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
	
	// 2. Read in a text file, line by line.
	string line;
	while (getline(fin, line))
	{
		process(line);
	}
	
	/*
	int sindex2 = 0;
	for (int i = 0; i <= 999; i++)
	{
		if (i == start[sindex2] )
		{
			cout << start[sindex2] << " ";	
			sindex2++;
		}
		cout << words[i];
	}	
	
	for (int i = 0; i <= 99; i++)
	{
		cout << start[i] << " ";	
	}*/
}

// 3. Split each line into words, discarding punctuation and folding all letters into lower case.
void process(string line)
{	
	string word, newword;
	stringstream ss(line);
	for(;;)
	{
		ss >> word;
		if(ss.fail())break;	
		
		for (int i = 0; i < word.size(); i++)
		{
			if (isalpha(line[i]))
			{
				
				if (isupper(line[i])) // convert uppers and add letter to words
				{
					newword += tolower(word[i]);
				}
				else // else add to words
				{
					newword += word[i];
				}
			}
		}
		store(newword);
		newword = "";
	}
}

// 4. Store the unique words and maintain a count of each different word.
void store(string newword)
{/*	
	int wordsize = (start[sindex-1]-start[sindex-2]); // based off difference between starting points
	char word[wordsize];
	for (int i = 0; i < wordsize; i++)
		word[i] = words[start[sindex-2]+i];*/
	
	cout << "word: " << newword << endl;
	/*bool match = false;	
	int j = 0;
	while ( j < sindex && match == false ) // for all words
	{
		for (int i = 0; i < newword.size(); i++) // for all letters in current word
		{
			if (words[start[j]+i] == newword[i])
			{
				if (i+1 == newword.size())// match found (final letter matches)
				{	cout << "index: " << j<<endl;
					count[j]++;  // add to count
					match = true; // end search
				}
			}
			else
			{
				j++;// skip word
				break;
			}
		}
	}
	if (match == false)
	{	
		start[sindex] = windex;
		count[sindex] = 1;
		sindex++;
		for (int i = 0; i < newword.size(); i++) // for all letters in current word
		{
			words[windex] = newword[i];
			windex++;
		}
	}
	cout <<"index: " << sindex-1 << endl;
	cout <<"count: " << count[sindex-1] << endl;*/
}

// 5. Sort the words first by increasing count and, if there are multiple words with the same count, alphabetically.
void sort()
{
	
}

// 6. Output the first ten words in the sorted list, along with their counts.
void printfirst()
{
	
}

// 7. Output the last ten words in the list, along with their counts.
void printlast()
{
	
}
