/**********************************************************************
 * main.cpp - CSCI251/851 - Ass4- main() tester for class BillSystem
 * Date: 23/05/2018
 * Name: Oliver Rankin-Starcevic
 * Login: ors549
 * Email: ors549@uowmail.edu.au
 *********************************************************************/
#include <iostream>
#include <iomanip>
#include "BillSystem.h"
using namespace std;
 
char Menu();

int main(){

	BillSystem BS;

	cout << "Begin tests for BillSystem\n\n";

	if(!BS.ReadFile("usage.txt")){
		cout << "File not found!\n\n";
		exit(1);
	}
	int n = BS.GetNumRecs();

	cout << "Num records read: " << n << endl << endl;
	
	// header
	cout << "  # " << left 
	<< setw(8) << "Service"
	<< setw(9) << "Supplier"
	<< setw(15) << "Customer"
	<< setw(27) << "Address"
	<< right 
	<< setw(6) << "AccBal"
	<< setw(5) << "Days" << endl;
	
	for(int i=0; i<n && i<10; i++){
		cout << right << setw(3) << i+1 << " ";
		BS.DisplayRec(i);
		cout << endl;
	}
	
	BS.CalDiscounts(); // uncoment when step 3 complete
	
	BS.CalBills();     // uncoment when step 4 complete
	BS.PrintReport();  // uncoment when step 4 complete

	cout << endl << "End tests for BillSystem\n";

	return 0;
}

