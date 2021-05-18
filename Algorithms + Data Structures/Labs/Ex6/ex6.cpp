/********************************************************************
 * CSCI203 - Ex6
 * Ex6.cpp - Karp-Rabin String
 * Date: 02/09/2018
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
char t[5000];
char s[10];
int s_size = 0;
int t_size = 0;
int p = 5;

int hash(char[]);
int roll(int,char,char);
int convert(char);

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
	
	while (!fin.eof())
	{
		fin >> t;
		fin >> s;
	}
	
	while (s[s_size] != '\0')
	{
		s_size++;
	}
	
	while (t[t_size] != '\0')
	{
		t_size++;
	}
	
	int hash_s=hash(s);
	int hash_t=hash(t);

	bool match = false;
	for (int i = 0; i <= t_size-s_size; i++)
	{
		if (hash_s == hash_t)
		{	
			//brute-force compare s and the substring
			for (int j = 0; j < s_size; j++)
			{
				if (s[j] != t[i+j])
					break;
				else if (j == s_size-1)
					match = true;
			}
			if (match == true)//they match 
				cout << "string found starting at location " << i << endl;
			match = false;
		}
		//cout << hash_t << " ";
		hash_t=roll(hash_t,t[i],t[i+s_size]);
	}
}

int convert(char c)
{
	if (c == 'A')
		return 0;
	else if (c == 'C')
		return 1;
	else if (c == 'G')
		return 2;
	else
		return 3;
}

int hash(char c[])
{
	int hash = 0;
	for (int i = 0; i < s_size; i++)
	{
		hash += pow(p,s_size-1-i)*convert(c[i]);
	}
	return hash;
}

int roll(int hash_t, char oldchar, char newchar)
{
	int hash = hash_t;
	
	hash -= pow(p,s_size-1)*convert(oldchar);
	hash *= p;
	hash += pow(p,0)*convert(newchar);
		
	return hash;
}
