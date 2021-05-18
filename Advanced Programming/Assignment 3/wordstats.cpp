/**********************************************************************
 * wordstats.cpp - CSCI251 - Ass3 - Contains WordsStats class definition
 * Date: 03/05/2018
 * Name: Oliver Rankin-Starcevic
 * Login: ors549
 * Email: ors549@uowmail.edu.au
 ********************************************************************/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <cctype>
#include "wordstats.h"
using namespace std;

WordStats::WordStats()
{
	strcpy(Filename,"testdata.txt");
}

// Reads dictionary.txt into Dictionary
void WordStats::ReadDictionary()
{
	cout << endl;
	int i;
	string input;
	ifstream fin;
	
	fin.open("dictionary.txt");
	if (!fin.good())
	{
		cout << "Error: Data file not opened.";
		exit (1);
	}
	while (!fin.eof())
	{
		fin >> input;
		if (fin.fail())
			break;
			
		// to lowercase
		i = 0;
		while (isupper(input[i]) || !isalpha(input[i]))
		{
			input[i] = tolower(input[i]);
			i++;
		}
			
		Dictionary.insert(input);
	}
	fin.close();
	cout << Dictionary.size() << " words read from dictionary." << endl;
}

// Displays first 20 Dictwords
void WordStats::DisplayDictionary()
{
	cout << endl;
	set<string>::iterator i = Dictionary.begin();
	
	cout << "Displaying the first 20 words in the dicionary..." << endl;
	for (int j = 0; j < 20; j++)
	{
		cout << *i << endl;
		i++;
	}	
	cout << endl;
}

// Reads textfile into KnownWords and UnknownWords
void WordStats::ReadTxtFile()
{
	cout << endl;
	string input, prosinput;
	set<string>::iterator setI;
	int pos = 0;
	
	ifstream fin;
	
	fin.open("testdata.txt");
	if (!fin.good())
	{
		cout << "Error: Data file not opened.";
		exit (1);
	}
	
	// Y.J
	// debug output setting
	// string orw;
	// string clw;
	// bool flag;
	// int size=0;

	while (!fin.eof())
	{
		fin >> input;
		// orw=input;
		if (fin.fail())
			break;
			
		// preprocess
		prosinput = "";
		for (int i = 0; i < input.size(); i++)
		{
			if (isalpha(input[i]) || input[i] == '-'  || input[i] == '‘')
			{
					prosinput += tolower(input[i]);
			}
		}
		
		// Ignore solitary hyphens
		if (prosinput.compare("-") == 0)
			continue;
			
		// clw=prosinput;
		// flag=false;
		// flag=setI==Dictionary.end();
		
		// If in dictionary known, else unknown
		setI = Dictionary.find(prosinput);
		WordMapIter itr;
		
		if(setI != Dictionary.end())
		{
			itr = KnownWords.find(prosinput);
			if(itr != KnownWords.end()) // if already in map
			{
				(*itr).second.push_back(pos);
			}
			else
			{
				vector<int> vect(1,pos); // construct vector
				KnownWords.insert(itr, pair<string, vector<int> >(prosinput,vect)); // insert
			}
		}
		else
		{
			itr = UnknownWords.find(prosinput);
			if(itr != UnknownWords.end()) // if already in map
			{
				(*itr).second.push_back(pos);
			}
			else
			{
				vector<int> vect(1,pos); // construct vector
				UnknownWords.insert(itr, pair<string, vector<int> >(prosinput,vect)); // insert
				// size++;
			}
		}	
		pos++;
		// cout<< orw<<" "<<clw<<" "<< flag<<" "<< size;
		// while(cin.get()!='\n');
	}
	cout << KnownWords.size() << " known words read." << endl;
	cout << UnknownWords.size() << " unknown words read." << endl;
	
	fin.close();
	cout << endl;
}

// Displays stats of words in KnownWords
void WordStats::DisplayKnownWordStats()
{
	DisplayStats(KnownWords);
}

// Displays stats of words in Unknownwords
void WordStats::DisplayUnknownWordStats()
{
	DisplayStats(UnknownWords);
}

// Displays 10 most frequent words in KnownWords
void WordStats::DisplayMostFreqKnownWords()
{
	DisplayMostFreq(KnownWords);
}

// Displays 10 most frequent words in UnknownWords
void WordStats::DisplayMostFreqUnknownWords()
{
	DisplayMostFreq(UnknownWords);
}

// Displays original text from KnownWords & UnknownWords
void WordStats::DisplayOriginalText()
{
	typedef map<int,string> OrigMap;
	OrigMap::iterator oitr;
	WordMapIter itr;
	OrigMap omap;	
	
	for (itr = KnownWords.begin();  itr != KnownWords.end();  ++itr)
	{
		vector<int> vect = (*itr).second;
		for (int i = 0; i < vect.size(); i++)
		{
			omap.insert(pair<int, string>(vect[i],(*itr).first));
		}
	}
	for (itr = UnknownWords.begin();  itr != UnknownWords.end();  ++itr)
	{
		vector<int> vect = (*itr).second;
		for (int i = 0; i < vect.size(); i++)
		{
			omap.insert(pair<int, string>(vect[i],(*itr).first));
		}
	}
	
	cout << endl;
	for(oitr = omap.begin(); oitr != omap.end(); ++oitr)
	{
		cout << (*oitr).second << ' ';
	}
	cout << endl;
}

// ============ Private Fns ========================
void WordStats::DisplayStats(WordMap parmap)
{
	cout << setw(15) << "Word" << "    Count" << "   Position(s)" << endl << endl;
	WordMapIter itr;
	string word;
	int count;
	
	for (itr = parmap.begin();  itr != parmap.end();  ++itr)
	{
		word = (*itr).first;
		vector<int> vect = (*itr).second;
		count = vect.size();
		cout << setw(15) << word << setw(7) << count << "     ";
		for (int i = 0; i < count; i++)
		{
			if (i == 13)
				cout << endl << setw(30);
			cout <<  vect[i] << ' ';
		}
		cout << endl;
	}
	cout << endl;
}

void WordStats::DisplayMostFreq(WordMap parmap)
{
	typedef multimap<int,string> MMap;
	MMap mmap;
	MMap::iterator mulitr;
	WordMapIter itr;	
	
	for (itr = parmap.begin();  itr != parmap.end();  ++itr)
	{
		vector<int> vect = (*itr).second;
		mmap.insert(make_pair(vect.size(),(*itr).first));
	}
	
	cout << setw(15) << "Word" << "    Count" << endl;
	cout << endl;
	mulitr = --mmap.end();
	for(int i = 0; i < 10; i++)
	{
		cout << setw(15) << (*mulitr).second << setw(7) << (*mulitr).first << endl;
		mulitr--;
	}
	cout << endl;
}

