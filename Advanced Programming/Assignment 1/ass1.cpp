/********************************************************************
 * CSCI251 - Ass1
 * ass1.cpp - Contains function definitions for student records database
 * Details: Design a solution to a problem from a partially complete framework.
 * 			Gain some experience writing a database application in C++.
 * 			Using binary I-O in C++ programs.
 * Date: 13/03/2018
 * Name: Oliver Rankin-Starcevic
 * Login: ors549
 * Email: ors549@uowmail.edu.au
 ********************************************************************/
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

// ============== Constants ==========================================

const char cTextFileName[]   = "ass1.txt";
const char cBinaryFileName[] = "ass1.dat";
const int cMaxRecs = 100;
const int cMaxChars = 30;
const int cMaxSubjects = 8;

// ============= User Defined types ==================================

enum StatusType{eEnrolled,eProvisional,eWithdrawn};
struct SubjectType{ char Code[8]; StatusType Status; int Mark;};

struct StudentRecord{
	long StudentNo;
	char FirstName[cMaxChars];
	char LastName[cMaxChars];
	int NumSubjects;
	SubjectType Subjects[cMaxSubjects];
};


// ============= Global Data =========================================

StudentRecord gRecs[cMaxRecs];
int gNumRecs=0;


// ============= Private Function Prototypes =========================

void PrintRecord(int i);
int FindRecord(long StudentNum);
bool ReadTextFile(char Filename[]);
bool WriteTextFile(char Filename[]);
bool ReadBinaryFile(char Filename[]);
bool WriteBinaryFile(char Filename[]);
void WriteBinaryRecord(char Filename[], int Pos);

// ============= Public Function Definitions =========================


void ReadFile()
{// Reads data file into array
	if (ReadBinaryFile((char*)cBinaryFileName)){}
	else 
		if (ReadTextFile((char*)cTextFileName)){}
		else
		{
			cout << "Failed to read both data files...\n";
			exit (1);	
		}
}

void DisplayRecord()
{// Displays specified record on screen
	long studentNumber;
	int i;
	
	cout << "Enter student number: ";
	cin >> studentNumber;
	i = FindRecord(studentNumber);
	if (i==-1)
		cout << "Record not found" << endl;
	else
		PrintRecord(i);
}

void SaveFile()
{// Writes array to text data file
	if (WriteBinaryFile((char*)cBinaryFileName)){}
	else 
		if (WriteTextFile((char*)cTextFileName)){}
		else
		{
			cout << "Failed to save data!...\n";
			exit (1);	
		}
}

void UpdateRecord()
{// updates status or mark of specified subject of specified student number
	long studentNumber;
	char subjectCode[8], selectSorM, selectStat;
	bool found=false;
	int inputMark, k, pos;
	
	// Student number Input/validation
	cout << "Enter student number: ";
	cin  >> studentNumber;
	int i = FindRecord(studentNumber);
	if (i==-1) 
		cout << "Record not found!";
	else
	{// subject code input
		PrintRecord(i);
		cout << "Enter subject code: ";
		cin  >> subjectCode;
		for (int j=0; j<gRecs[i].NumSubjects; j++)
		{// subject code validation
			if (strcmp(subjectCode,gRecs[i].Subjects[j].Code) == 0)
			{	
				found=true;
				k=j; // k is used outside of the for loop to navigate to the correct subject
			}
		}
		if (found == true)
		{// selecting what to change for the subject
			cout << "Select status or mark (s/m): ";
			cin  >> selectSorM;
			if (selectSorM == 's')
			{ // changing status to:
				cout << "Select new status" << endl;
				cout << "    e: enrolled" << endl;
				cout << "    p: provisional" << endl;
				cout << "    w: withdrawn" << endl;
				cout << "Status: ";
				cin  >> selectStat;
				if (selectStat == 'e')
					gRecs[i].Subjects[k].Status = StatusType(0);
				else if (selectStat == 'p')
					gRecs[i].Subjects[k].Status = StatusType(1);
				else if (selectStat == 'w')
					gRecs[i].Subjects[k].Status = StatusType(2);
			}
			else if (selectSorM == 'm')
			{// changing mark to:
				cout << "Mark: ";
				cin  >> inputMark;
				if (inputMark > 100 || inputMark < 0)
					cout << "Invalid argument!";
				else
					gRecs[i].Subjects[k].Mark = inputMark;
			}
			
			// STEP 3b
			WriteBinaryRecord((char*)cBinaryFileName, i);
			cout << "Record " << subjectCode << " Updated." << endl;
		}
		else
			cout << "Subject code not found!";
	}
}


// ============= Private Function Definitions =========================

void PrintRecord(int i)
{ // Prints record i on screen
	cout << "Student No.      " << gRecs[i].StudentNo << endl;
	cout << "First Name       " << gRecs[i].FirstName << endl;
	cout << "Last Name        " << gRecs[i].LastName << endl;
	cout << "Subjects:" << endl;
	for (int j=0; j<gRecs[i].NumSubjects; j++) 
	{
		cout << "    " << gRecs[i].Subjects[j].Code;
		switch(gRecs[i].Subjects[j].Status)
		{
			case eEnrolled: cout    << "  Enrolled     "; break;
			case eProvisional: cout << "  Provisional  "; break;
			case eWithdrawn: cout   << "  Withdrawn    "; break;
		}
		cout << gRecs[i].Subjects[j].Mark << endl;
	}
}

int FindRecord(long StudentNo)
{// returns index of matching record or -1 if not found
	for (int i=0; i<gNumRecs; i++)
	{
		if (StudentNo == gRecs[i].StudentNo)
			return i;
	}
	return -1;
}

bool ReadTextFile(char Filename[])
{//reads text data from ass1.txt file to gRecs[] array
	ifstream fin;
	fin.open(Filename);
	if (!fin.good())
		return false;
	int i = 0;
	int status;
	fin >> gRecs[i].StudentNo;
	while (!fin.eof() && i<cMaxRecs)
	{
		fin >> gRecs[i].FirstName;
		fin >> gRecs[i].LastName;
		fin >> gRecs[i].NumSubjects;
		for (int j=0; j<gRecs[i].NumSubjects; j++) 
		{
			fin >> gRecs[i].Subjects[j].Code >> status;
			gRecs[i].Subjects[j].Status = StatusType(status);
			fin >> gRecs[i].Subjects[j].Mark;
		}
		i++;
		fin >> gRecs[i].StudentNo;
	}
	gNumRecs = i;
	
	fin.close();
	cout << gNumRecs << " records read from text file..." << endl;
	return true;
}


bool WriteTextFile(char Filename[]) 
{//writes text data from gRecs[] to ass1.txt file
	ofstream fout;
	fout.open(Filename);
	if(!fout.good())
		return false;
	for (int i=0; i<gNumRecs; i++)
	{
		fout << gRecs[i].StudentNo << endl;
		fout << gRecs[i].FirstName << " " 
		<< gRecs[i].LastName << endl;
		fout << gRecs[i].NumSubjects << endl;
		for (int j=0; j<gRecs[i].NumSubjects; j++)
		{  
			fout << gRecs[i].Subjects[j].Code << " " 
			<< gRecs[i].Subjects[j].Status << " " 
			<< gRecs[i].Subjects[j].Mark << endl;
		}
		fout << endl;
	}
	fout.close();
	cout << gNumRecs << " records saved to text file...";
	return true;
}

bool ReadBinaryFile(char Filename[])
{//reads binary data from ass1.txt file to gRecs[] array
	ifstream binin(Filename, ios::in | ios::binary);
	
	if(!binin.good())
		return false;
	int i = 0;
	while (binin)
	{
		binin.read((char*) &gRecs[i], sizeof(StudentRecord));
		i++;
	}
	
	gNumRecs = i-1;
	cout << gNumRecs << " records read from binary file..." << endl;
	binin.close();
	return true;
}

bool WriteBinaryFile(char Filename[])
{//writes binary data from gRecs[] to ass1.dat file
	ofstream binout(Filename, ios::out | ios::binary);
	
	if(!binout.good())
		return false;
	for (int i=0; i<gNumRecs; i++)
	{
		binout.write((char*) &gRecs[i], sizeof(StudentRecord));
	}
	cout << gNumRecs << " records saved to binary file...";
	binout.close();
	return true;
}

void WriteBinaryRecord(char Filename[], int Pos)
{//finds record position in ass1.dat file and replaces with updated record
	fstream binout(Filename, ios::in | ios::out | ios::binary);
	
	if(binout.good())
	{
		binout.seekp(Pos*sizeof(StudentRecord));
		binout.write((char*) &gRecs[Pos], sizeof(StudentRecord));
		cout << "Record updated in binary file..." << endl;
	}
	binout.close();
}

