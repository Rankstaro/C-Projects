/**********************************************
 * CardSet.cpp - Contains implementation of class CardSet
 * Details: Add implementations of the member functions.
 * Date: 04/04/2018
 * Name: Oliver Rankin-Starcevic
 * Login: ors549
 * Email: ors549@uowmail.edu.au
 ********************************************************************/

#include <iostream>
#include "CardSet.h"
using namespace std;

// (i) Default constructor - Sets up a set of 0 cards.
CardSet::CardSet()
{
	Card = NULL;
	nCards = 0;
}

// (ii) Initialising constructor - Set up a set of [arg] cards.
CardSet::CardSet(int cardNum)
{
	Card = new int[cardNum];
	for (int i = 0; i < cardNum; i++)
		Card[i] = i%52;
	nCards = cardNum;
}

// (iii) Destructor - Cleans up dynamic memory.
CardSet::~CardSet()
{
	delete[] Card;
}

// (iv) Returns the value of nCards
int CardSet::Size() const
{
	return nCards;
}

// (v) Returns true if there are no cards in this set.
bool CardSet::IsEmpty() const
{
	return !Card;
}

// (vi) Randomly rearranges the cards in this set.
void CardSet::Shuffle()
{
	int j, tmp;
	for (int i = 0; i < nCards; i++)
	{
		j = rand() % nCards;
		if (i != j)
		{	
			tmp = Card[j];
			Card[j] = Card[i];
			Card[i] = tmp;
		}
	}
}

// (vii) Returns the value of the first card in the set, creates fresh memory, transfers the rest of the 
// 		 set to this new memory and deletes the old memory.
int CardSet::Deal()
{
	if (!Card)
	{
		cout << "ERROR: Set empty!";
		exit (1);
	}
	else
	{
		int firstCard = Card[0];
		nCards -= 1;
		
		int* TmpCard = new int[nCards];
		for (int i = 0; i < nCards; i++)
			TmpCard[i] = Card[i+1];
		delete[] Card;
		
		Card = TmpCard;
		
		return firstCard;
	}
}

// (viii) Deals two hands into the two CardSet arguments passed.
void CardSet::Deal(int handSize, CardSet& Hand1, CardSet& Hand2)
{
	if (handSize*2 > Size())
	{
		cout << "ERROR: Insufficient cards to perform deal!";
		exit(0);
	}
	for (int i = 0; i < handSize; i++)
	{
		Hand1.AddCard(Deal());
		Hand2.AddCard(Deal());
	}
}

// (ix) Same as above but for four hands
void CardSet::Deal(int handSize, CardSet& Hand1, CardSet& Hand2, CardSet& Hand3, CardSet& Hand4)
{
	if (handSize*4 > Size())
	{
		cout << "ERROR: Insufficient cards to perform deal!";
		exit(0);
	}
	for (int i = 0; i < handSize; i++)
	{
		Hand1.AddCard(Deal());
		Hand2.AddCard(Deal());
		Hand3.AddCard(Deal());
		Hand4.AddCard(Deal());
	}
}

// (x) Function to add a card to the current hand.
void CardSet::AddCard(int newCard)
{
	nCards += 1;
	
	int* TmpCard = new int[nCards];
	for (int i = 0; i < nCards; i++)
	{
		if (i == nCards-1)
			TmpCard[i] = newCard;
		else
			TmpCard[i] = Card[i];
	}
	delete[] Card;
	
	Card = TmpCard;
}

// (xi) Puts all cards from the argument set into the current set in an alternating fashion.
void CardSet::MergeShuffle(CardSet &MergeCard)
{		
		int* TmpCard = new int[nCards + MergeCard.nCards];		
		int j = 0;
		
		for (int i = 0; i < (nCards + MergeCard.nCards); i++)
		{
			if (j >= nCards)
				TmpCard[i] = MergeCard.Card[j];
			else if (j >= MergeCard.nCards)
				TmpCard[i] = Card[j];
			else 
			{
				TmpCard[i] = Card[j];
				i++;
				TmpCard[i] = MergeCard.Card[j];
			}
			j++;
		}		
		delete[] Card;
		MergeCard.~CardSet();
		
		Card = TmpCard;
		nCards = nCards + MergeCard.nCards;
		
		MergeCard.Card = NULL;
		MergeCard.nCards = 0;
}


// (xii) Uses PrintCard to print contents of the set.
void CardSet::Print() const
{
// five cards to a line
	for (int i = 0; i < nCards; i++)
	{	
		PrintCard(Card[i]);
		if ((i+1) % 5 == 0 && i != 0)
			cout << endl;
		else
			cout << " ";
	}
	cout << endl;
}

// Private function to print out usual representation of playing card.
// Input is integer from 0 to 51.  There is no error checking.

void CardSet::PrintCard(int c) const
{
	int Rank = c%13;
	int Suit = c/13;
	const char NameSuit[5] = "SCDH";
	const char NameRank[14] = "23456789XJQKA";
	cout << NameRank[Rank] << NameSuit[Suit];
}
