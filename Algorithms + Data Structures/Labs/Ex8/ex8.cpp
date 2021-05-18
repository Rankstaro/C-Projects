/********************************************************************
 * CSCI203 - Ex8
 * Ex8.cpp - Crazy8s
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
struct Card
{
	String r;
	char s;
};



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
	}}
	
	while (!fin.eof())
	{
		fin >> v1 >> v2;
		insert(v1,v2);
		insert(v2,v1);
	}	
	
	for each v in V
crazyDP(V, E, s, v)
rof
solution = max M[v]
}

void output() 
{
	cout << "The length of the longest sequence: " << bigLen << endl
		<< "The first card in the sequence: " << first << endl
		<< "The last card in the sequence: " << last << endl;
}

M: dictionary = {}
Procedure crazyDP(V{}: vertex, E{}: edge, s: vertex, v: vertex)
for each v in V
m=0
if v ¡Ú s then
for each u where (u,v) „¡ E
if (u in M) then
m = max (m, D[u]+1)
else
m = max(m, crazyDP(V, E, s, u) + 1)
fi
rof
fi
M[v] = m
rof
End procedure crazyDP
