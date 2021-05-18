/**********************************************************************
 * BillRecord.cpp - CSCI251/851 - Ass5 - BillRecord class definition
 * Date: 23/05/2018
 * Name: Oliver Rankin-Starcevic
 * Login: ors549
 * Email: ors549@uowmail.edu.au
 *********************************************************************/
#include <iostream>
#include <fstream>
#include <iomanip>
#include "BillRecord.h"
using namespace std;

// ========== class BillRecord function definitions ==========

// Default constructor
BillRecord::BillRecord()
{
	BillType BType;
	Supplier = "";
	Name = "";
	Address = "";  
	AccountBalance = 0; 
	DaysSinceLastReading = 0; 
	BillAmount = 0;
	Discount = 1;
}

// Reads customer details part of record from file into the private data members
void BillRecord::ReadCustDetails(ifstream &fin)
{
	getline(fin,Supplier);
	getline(fin,Name);
	getline(fin,Address);  
	fin >> AccountBalance; 
	fin >> DaysSinceLastReading; 
}

// Displays customer details part of record on screen from private data members
void BillRecord::DisplayCustDetails()
{
	cout << left << setw(8);
	if (BType == BillType(0))
		cout << "Elect";
	else if (BType == BillType(1))
		cout << "Gas";
	else if (BType == BillType(2))
		cout << "Phone";
	cout << setw(9) << Supplier;
	cout << setw(15) << Name;
	cout << setw(27) << Address;
	cout << right << setw(6) << AccountBalance;
	cout << setw(5) << DaysSinceLastReading;
}

// Virtual fn for reading usage info part of record from file in derived classes
void BillRecord::ReadUsageInfo(ifstream &fin)
{
	//Eat the usage info from file and discard it
	string eat;
	if (BType == BillType(0))
		getline(fin,eat);
	for (int i = 0; i < 7; i++)
		getline(fin,eat);
}

// Displays Base customer details + Bill amount for report
void BillRecord::DisplayBillAmt()
{
	cout << left << setw(8);
	if (BType == BillType(0))
		cout << "Elect";
	else if (BType == BillType(1))
		cout << "Gas";
	else if (BType == BillType(2))
		cout << "Phone";
	cout << setw(9) << Supplier;
	cout << setw(15) << Name;
	cout << setw(27) << Address;
	cout << right << setw(10) << fixed << setprecision(2) 
	<< BillAmount << endl;
}

// Pure virtual functions
void BillRecord::DisplayUsageInfo(){}

void BillRecord::UpdateBalance(){}

// =============== Get/Set fns ================

void BillRecord::SetBType(int i)
{
	BType = BillType(i);
}

void BillRecord::SetDiscount(float d)
{
	Discount = d;
}

void BillRecord::SetBillAmount(double d)
{
	BillAmount = d;
}

string BillRecord::GetCatstring()
{
	return Name + Address + Supplier;
}

string BillRecord::GetSupplier()
{
	return Supplier;
}

string BillRecord::GetName()
{
	return Name;
}

// ========== Derived Class function definitions ==========
// -------------------- ElectBillRecord -------------------
// Default Electricity Constructor
ElectBillRecord::ElectBillRecord()
{
	SetBType(0);
	PrevReading = 0;
	CrntReading = 0;
	Rate1 = 0;
	Rate1Thresh = 0;
	Rate2 = 0;
	SuppCharge = 0;
} 

// Reads usage info part of record from file in Electricity derived class
void ElectBillRecord::ReadUsageInfo(ifstream &fin)
{
	fin >> PrevReading;
	fin >> CrntReading;
	fin >> Rate1;
	fin >> Rate1Thresh;
	fin >> Rate2;
	fin >> SuppCharge;
	
	string eat;
	getline(fin, eat);
	getline(fin, eat);
}

// displays usage info part of record in Electricity derived class
void ElectBillRecord::DisplayUsageInfo()
{
	cout << setw(23) << "(Readings: " << PrevReading << ", " << CrntReading
	<< " R1: " << Rate1
	<< " R1Th: " << Rate1Thresh
	<< " R2: " << Rate2
	<< " SuppC: " << SuppCharge << ")";
}

// Calculates Electricty Bill amount and sets account balance to 0
void ElectBillRecord::UpdateBalance()
{
	double c = CrntReading - PrevReading;
	double p = SuppCharge * DaysSinceLastReading;
	if (c <= Rate1Thresh) 
		SetBillAmount((c * Rate1 + p) * Discount - AccountBalance);
	else 
		SetBillAmount((Rate1Thresh * Rate1 + (c - Rate1Thresh) * Rate2 + p) * Discount - AccountBalance);
	AccountBalance = 0;
}

// --------------------- GasBillRecord ---------------------
// Default Gas Constructor
GasBillRecord::GasBillRecord()
{
	SetBType(1);
	PrevReading = 0;
	CrntReading = 0;
	HeatVal = 0;
	Rate = 0;
	SuppCharge = 0;
}

// Reads usage info part of record from file in gas derived class
void GasBillRecord::ReadUsageInfo(ifstream &fin)
{
	fin >> PrevReading;
	fin >> CrntReading;
	fin >> HeatVal;
	fin >> Rate;
	fin >> SuppCharge;
	
	string eat;
	getline(fin, eat);
	getline(fin, eat);
}

// displays usage info part of record in gas derived class
void GasBillRecord::DisplayUsageInfo()
{
	cout << setw(23) << "(Readings: " << PrevReading << ", " << CrntReading
	<< "  HV: " << HeatVal
	<< "  Rate: " << Rate
	<< "  SuppC: " << SuppCharge << ")";
}

// Calculates Gas Bill amount and sets account balance to 0
void GasBillRecord::UpdateBalance()
{
	double c = CrntReading - PrevReading;
	double p = SuppCharge * DaysSinceLastReading;
	SetBillAmount((c * HeatVal * Rate) * Discount - AccountBalance);
	AccountBalance = 0;
}

// --------------------- PhoneBillRecord --------------------
// Default phone Constructor
PhoneBillRecord::PhoneBillRecord()
{
	SetBType(2);
	NumofLocalCall = 0;
	LocalRate = 0;
	LongCallTime = 0;
	LongRate = 0;
	LineRental = 0;
}

// Reads usage info part of record from file in phone derived class
void PhoneBillRecord::ReadUsageInfo(ifstream &fin)
{
	fin >> NumofLocalCall;
	fin >> LocalRate;
	fin >> LongCallTime;
	fin >> LongRate;
	fin >> LineRental;
	
	string eat;
	getline(fin, eat);
	getline(fin, eat);
}

// displays usage info part of record in phone derived class
void PhoneBillRecord::DisplayUsageInfo()
{
	cout << setw(21) << "(LCalls: " << NumofLocalCall << ", " << LocalRate
	<< "  DCalls: " << LongCallTime << ", " << LongRate
	<< "  Rental: " << LineRental << ")";
}

// Calculates Phone Bill amount and sets account balance to 0
void PhoneBillRecord::UpdateBalance()
{
	double l = LocalRate * NumofLocalCall;
	double d = LongRate * LongCallTime;
	double p = LineRental * DaysSinceLastReading;
	SetBillAmount((l + d + p) * Discount - AccountBalance);
	AccountBalance = 0;
}
