/**********************************************************************
 * BillSystem.cpp - CSCI251/851 - Ass4 - BillSystem class definition
 * Date: 23/05/2018
 * Name: Oliver Rankin-Starcevic
 * Login: ors549
 * Email: ors549@uowmail.edu.au
 *********************************************************************/
#include <iostream>
#include <fstream>
#include <iomanip>
#include "BillSystem.h"
using namespace std;

// Global map listing discount (customer name, supplier)
typedef map<string, string > DiscMap;
DiscMap::iterator ditr;
DiscMap dmap;

// ========== class BillSystem Public Function definitions ==========

// Destructor
BillSystem::~BillSystem()
{
	//Iterate BRecs vector and delete each ptr
	for (int i = 0; i < BRecs.size(); i++)
	{
		delete BRecs[i];
	}
}

// Reads data in "usage.txt" into BRecs vector
bool BillSystem::ReadFile(char *fname)
{
	ifstream fin;
	fin.open(fname);
	if (!fin.good())
	{
		cout << "File not found.";
		return false;
	}
	while (!fin.eof())
	{
		BillRecord *temp;
		string BTStr;
		getline(fin, BTStr);
		if (fin.fail())
			break;
			
		if (BTStr.compare("Elect") == 0)
			temp = new ElectBillRecord;
		else if (BTStr.compare("Gas") == 0)
			temp = new GasBillRecord;
		else 
			temp = new PhoneBillRecord;
			
		temp->ReadCustDetails(fin);
		temp->ReadUsageInfo(fin);
		BRecs.push_back(temp);
	}
	fin.close();
	return true;
}

// Returns the number of records in BRecs
int BillSystem::GetNumRecs()
{
	return BRecs.size();
}

 // Displays ith record on screen
void BillSystem::DisplayRec(int i)
{
	BRecs[i]->DisplayCustDetails();
	cout<<endl;
	BRecs[i]->DisplayUsageInfo();
	cout<<endl;
}

// Sets discount value to appropriate indentified records 
void BillSystem::CalDiscounts()
{
    typedef multimap<string, BillRecord*> MMap;
	MMap mmap;
	MMap::iterator itr;
	
	// Creates multimap of all records with Catstring (name+address+supplier) as key
    for (int i = 0; i < BRecs.size(); i++)
	{
		string tmp = BRecs[i]->GetCatstring();
		mmap.insert(pair<string, BillRecord*>(tmp,BRecs[i]));
	}
	
	// If supplier is dodo/alinta and appears 3 times in multimap,
	// add to discount map and set discount value.
    for (itr = mmap.begin();  itr != mmap.end();  ++itr)
	{
		string tmp = (*itr).first;
		int count = mmap.count(tmp);
        if (count == 3)
        {
			BillRecord *tmpBR = (*itr).second;
			string supp = tmpBR->GetSupplier();
            if (supp.compare("Dodo") == 0)
            {
            	tmpBR->SetDiscount(0.85);
            	string name = tmpBR->GetName();
            	ditr = dmap.find(name);
				if(ditr == dmap.end()) // if not in dmap
				{
					dmap.insert(pair<string, string >(name,supp)); // insert
				}
			}
            else if (supp.compare("Alinta") == 0)
            {
				tmpBR->SetDiscount(0.8);
            	string name = tmpBR->GetName();
            	ditr = dmap.find(name);
				if(ditr == dmap.end()) // if not in dmap
				{
					dmap.insert(pair<string, string >(name,supp)); // insert
				}
			}	
		}
	}
}

// for all BRecs, update balance
void BillSystem::CalBills()
{
	for (int i = 0; i < BRecs.size(); i++)
	{
		BRecs[i]->UpdateBalance();
	}
}

// Print Record + bill amount, and discount customers.
void BillSystem::PrintReport()
{
	cout << "  # " << left 
	<< setw(8) << "Service"
	<< setw(9) << "Supplier"
	<< setw(15) << "Customer"
	<< setw(27) << "Address"
	<< right 
	<< setw(9) << "BillAmount" << endl;
	
	for(int i=0; i<10; i++)
	{
		cout << right << setw(3) << i+1 << " ";
		BRecs[i]->DisplayBillAmt();
	}
	
	cout << endl << left << setw(25) << "Discount Customers" << "Supplier" << endl;
	
	for (ditr = dmap.begin();  ditr != dmap.end();  ++ditr)
	{
		string name = (*ditr).first;
		string supp = (*ditr).second;
		cout << setw(25) << name << supp << endl;		
	}
}

// ========== class BillSystem Private Function definitions ==========

