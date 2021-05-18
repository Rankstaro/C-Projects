/***************************************************************************
 * main.cpp - CSCI251/851 - Ass3 - main() driver for testing WordStats class
 * Date: 03/05/2018
 * Name: Oliver Rankin-Starcevic
 * Login: ors549
 * Email: ors549@uowmail.edu.au
 ********************************************************************/
 
#include <iostream>
#include "wordstats.h"
using namespace std;


int main(){
	
   WordStats ws;
   cout << "Begin Text File Analyser Tests\n\n";

   cout << "Step-1 Reading and displaying dictionary\n";
   ws.ReadDictionary();
   ws.DisplayDictionary();

   cout << "Step-2 Reading words from text file\n";
   ws.ReadTxtFile();

   cout << "Step-3 Displaying known words:\n";
   ws.DisplayKnownWordStats();

   cout << "Step-4 Displaying unknown words:\n";
   ws.DisplayUnknownWordStats();

   cout << "Step-5 Displaying most frequent known words\n";
   ws.DisplayMostFreqKnownWords();

   cout << "Step-6 Displaying most frequent unknown words\n";
   ws.DisplayMostFreqUnknownWords();

   cout << "Step-7 Displaying original text\n";
   ws.DisplayOriginalText();

   cout << "\nEnd Text File Analyser Tests\n\n";

   return 0;
}

