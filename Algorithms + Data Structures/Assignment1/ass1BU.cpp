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
	}
}

// 3. Split each line into words, discarding punctuation and folding all letters into lower case.
void process(string line)
{	
	bool first = true;
	string newword;
	
	for (int i = 0; i < line.size(); i++)
	{
		if (isspace(line[i]) && isspace(line[i+1])) // eat whitespace
		{
			while (isspace(line[i+1]))
				i++;
		}
		else if (line[i] == ' ') // mark the start of a new word
		{
			store(newword);
		}
		else if (isalpha(line[i]))
		{
			if (first == true) // mark start of the first word in line
			{
				store(newword);
				first = false;
			}
			if (isupper(line[i])) // convert uppers and add letter to words
			{
				words[windex] = tolower(line[i]);
				windex++;
			}
			else // else add to words
			{
				words[windex] = line[i];
				windex++;
			}
		}
	}
}

// 4. Store the unique words and maintain a count of each different word.
void store(string newword)
{
	start[sindex] = windex;
	sindex++;
	
	int wordsize = (start[sindex-1]-start[sindex-2]); // based off difference between starting points
	char word[wordsize];
	for (int i = 0; i < wordsize; i++)
		word[i] = words[start[sindex-2]+i];
		
	int j = 0;
	while ( j < sindex ) // for all words
	{	
		for (int i = 0; i < wordsize; i++) // for all letters in current word
		{
			if (words[start[j]] != word[0]) // if letter doesnt match, go to next word
			{
				j++;
				break;
			}
			else if (i == wordsize-1) // match found (final letter matches)
			{
				cout << wordsize;
				break;
			}
		}
	}
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
