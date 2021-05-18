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
char words[500000];// contains all words in a stringpool
int windex = 0; // marks the last index of words
int start[50000]; //marks the index where a word starts in words
int sindex = 0; // marks the last index of start and counter.
int counter[50000]; // start[n] marks the start of the word, counter[n] is this words corresponding count


void process(string);
void store(string);
void sort();
void swapwords(int);
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
		process(line += " ");
	}
	
	sort();
	cout << "***** FIRST TEN *****";
	printfirst();
	cout << endl << endl << "***** LAST TEN *****";
	printlast();
}

// 3. Split each line into words, discarding punctuation and folding all letters into lower case.
void process(string line)
{	
	string newword;
	
	for (int i = 0; i < line.size(); i++)
	{
		if (isspace(line[i]) && (isspace(line[i+1]) || line.size() == 1)) // eat whitespace
		{
			while (isspace(line[i+1]))
				i++;
		}
		else if (isalpha(line[i]))
		{
			
			if (isupper(line[i])) // convert uppers and add letter to words
			{
				newword += tolower(line[i]);
			}
			else // else add to newword
			{
				newword += line[i];
			}
		}
		else if (line[i] == ' ') // mark the start of a new word
		{
			store(newword);
			newword = "";
		}
	}
}

// 4. Store the unique words and maintain a count of each different word.
void store(string newword)
{
	bool match = false;	
	int j = 0;
	while ( j < sindex && match == false ) // for all words
	{
		for (int i = 0; i < newword.size(); i++) // for all letters in current word
		{
			if (words[start[j]+i] == newword[i])
			{
				if (i+1 == newword.size())// match found (final letter matches)
				{
					counter[j]++;  // add to counter
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
	if (match == false) // add new word to array words
	{	
		start[sindex] = windex;
		counter[sindex] = 1;
		sindex++;
		for (int i = 0; i < newword.size(); i++) // for all letters in current word
		{
			words[windex] = newword[i];
			windex++;
		}
	}
}

// 5. Sort the words first by increasing count and, if there are multiple words with the same count, alphabetically.
void sort()
{
	int k, g, tmp;
	char tmp2;
	for (int i = 0; i < sindex; i++)  // for all sindex
	{	
		k = i;	
		while (k > 0 && counter[k - 1] > counter[k]) // while index is less than one before it, move back
		{
			// swap counts
			tmp = counter[k];
			counter[k] = counter[k - 1];
			counter[k - 1] = tmp;
			
			//swap words
			swapwords(k);
			
			k--;
		}
		
		while (k > 0 && counter[k - 1] == counter[k])
		{
			g = 0;
			while (words[start[k]+g] == words[start[k-1]+g]) // ignore matching letters
			{
				if (start[k]+g != start[k+1] && start[k-1]+g != start[k]) 
					g++;
				else // if word fits in word, e.g. sun -> sunny
					break;
			}
			if (words[start[k-1]+g] > words[start[k]+g])// if comes before in alphabet, swap words
				swapwords(k);
			k--;
		}
	}
}

void swapwords(int k)
{
	int crntwordsize;
	if (start[k+1] == 0) // end of array
		crntwordsize = (windex - start[k]);
	else
		crntwordsize = (start[k+1] - start[k]);
	int prevwordsize = (start[k] - start[k - 1]);
	char tmpword[prevwordsize];
	
	for (int j = 0; j < prevwordsize; j++)// save previous word in tmpword
	{
		tmpword[j] = words[start[k - 1]+j];
	}
	
	for (int j = 0; j < crntwordsize; j++)// move crnt word to position of previous
	{
		words[start[k - 1]+j] = words[start[k]+j];
	}
	
	start[k] = start[k-1]+ crntwordsize; // set start
	
	for (int j = 0; j < prevwordsize; j++) // move prevword back
	{
		words[start[k - 1]+crntwordsize+j] = tmpword[j]; 
	}
}

// 6. Output the first ten words in the sorted list, along with their counts.
void printfirst()
{
	int j=0;	
	for (int i = 0; i < start[10]; i++)
	{
		if (i == start[j] )
		{	
			cout << endl << j+1 << " ===========" << endl;
			cout << "count: "<<counter[j] << endl;
			cout << "word:  ";
			j++;
		}
		cout << words[i];
	}
}

// 7. Output the last ten words in the list, along with their counts.
void printlast()
{
	int j=sindex-10;	
	for (int i = start[sindex-10]; i < windex; i++)
	{
		if (i == start[j] )
		{	
			cout << endl << j+1 << " ===========" << endl;
			cout << "count: "<<counter[j] << endl;
			cout << "word:  ";
			j++;
		}
		cout << words[i];
	}	
}
