/********************************************************************
 * CSCI203 - Ex4
 * Ex4.cpp - BST Sort
 * Date: 19/08/2018
 * Name: Oliver Rankin-Starcevic
 * Login: ors549
 * Email: ors549@uowmail.edu.au
 ********************************************************************/
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

//Globals
struct Node
{
	int val;
	Node *left;
	Node *right;
};
Node *root;

int linecnt = 0;

Node *insert(Node*, int n);
void visit(Node*);

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
	
	int input;
	fin >> input;
	root = new Node;
	root->val = input;//The first number read will be the root of the tree
	root->left = NULL;
	root->right = NULL;
	
	while (!fin.eof())
	{
		//construct a binary search tree from values in the order they are read.
		fin >> input;
		insert(root, input);
	}
	
	visit(root);
}

//insert node
Node *insert(Node *i, int n)
{
    if(i == NULL)
    {
        Node *temp = new Node;
	    temp->val = n;
	    temp->left = NULL;
	    temp->right = NULL;
	    return temp;
	}
	if (n == i->val)
		return i;
    if(n < i->val)
        i->left = insert(i->left, n); 
    if(n > i->val)    
        i->right = insert(i->right, n);        
    return i;
}

void visit(Node *i)//conduct an in-order traversal to output the values in ascending order. Print them 10 to a line in a 5-character wide field.
{
	if (i->left != NULL)
		visit(i->left);
		
	cout <<left << setw(5) << i->val;
	linecnt++;
	if (linecnt == 10)
		{linecnt = 0; cout<<endl;}
		
	if (i->right != NULL)
		visit(i->right);
	return;
}
