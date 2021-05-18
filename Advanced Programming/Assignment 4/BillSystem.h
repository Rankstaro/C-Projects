/**********************************************************************
 * BillSystem.h - CSCI251/851 - Ass4- BillSystem class declaration
 * Date: 23/05/2018
 * Name: Oliver Rankin-Starcevic
 * Login: ors549
 * Email: ors549@uowmail.edu.au
 *********************************************************************/
#ifndef BILLSYS_H
#define BILLSYS_H

#include <iostream>
#include <vector>
#include <map>
#include "BillRecord.h"
using namespace std;

class BillSystem {
  public:
    ~BillSystem();
	bool ReadFile(char *fname); // reads file
	int GetNumRecs(); // returns BRecs size
	void DisplayRec(int i); // prints ith record on screen
	void CalDiscounts(); // identifies and sets discount values
	void CalBills(); // Updates balance for all Brecs
	void PrintReport(); // Prints report
	
  private:
	vector<BillRecord*> BRecs; // vector of pointers to class BillRecord
};


#endif

