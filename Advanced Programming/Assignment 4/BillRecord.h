/**********************************************************************
 * BillRecord.h - CSCI251/851 - Ass4 - BillRecord class declaration
 * Date: 23/05/2018
 * Name: Oliver Rankin-Starcevic
 * Login: ors549
 * Email: ors549@uowmail.edu.au
 *********************************************************************/
#ifndef BILLRECORD_H
#define BILLRECORD_H

#include <string>
using namespace std;

enum BillType {eElect, eGas, ePhone };

// base class...

class BillRecord {
  public:
	BillRecord();
	void ReadCustDetails(ifstream &fin); // reads base record info
	void DisplayCustDetails(); // prints base record info
	virtual void ReadUsageInfo(ifstream &fin) = 0; // virtual pure
	virtual void DisplayUsageInfo(); // Prints usage info
	virtual void UpdateBalance() = 0; // virtual pure
	void DisplayBillAmt(); // prints bill record + amount for report
	
	// Get/set private variables
	void SetBType(int i);
	void SetDiscount(float d);
	void SetBillAmount(double d);
	string GetCatstring();
	string GetName();
	string GetSupplier();

  private:
  	BillType BType; // Bill type
	string Supplier; // Supplier's name
	string Name, Address; // Customer's name and address
	double BillAmount;// Amount in dollars and cents of this bill
	
  protected:
  	float Discount; // Applies discount to BillAmount
	double AccountBalance; // Customer's account balance
	int DaysSinceLastReading; // Days since last reading
};

//derived classes...
class ElectBillRecord : public BillRecord
{
	public:
		ElectBillRecord();
		void ReadUsageInfo(ifstream &fin);
		void DisplayUsageInfo();
		void UpdateBalance();
	private:
		int PrevReading;
		int CrntReading;
		double Rate1;
		int Rate1Thresh;
		double Rate2;
		double SuppCharge;
};

class GasBillRecord : public BillRecord
{
	public:
		GasBillRecord();
		void ReadUsageInfo(ifstream &fin);
		void DisplayUsageInfo();
		void UpdateBalance();
	private:
		int PrevReading;
		int CrntReading;
		double HeatVal;
		double Rate;
		double SuppCharge;
};

class PhoneBillRecord : public BillRecord
{
	public:
		PhoneBillRecord();
		void ReadUsageInfo(ifstream &fin);
		void DisplayUsageInfo();
		void UpdateBalance();
	private:
		int NumofLocalCall;
		double LocalRate;
		int LongCallTime;
		double LongRate;
		double LineRental;
};

// declare your derived classes here


#endif

