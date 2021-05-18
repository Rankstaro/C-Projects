/*	Name: Oliver Rankin-Starcevic
 *	Num: 5800559
 *	Email: ors549@uowmail.edu.au
 *	Assignment: 2-2
 *	Due: 23/09/2019
 *	Citations: Casey Chow - Tutorial 5, 4, Lecture 5
 */

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions

// C++ standard library and STL headers
#include <iostream>
#include <vector>
#include <fstream>

// OpenCL header, depending on OS
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif


int main(void) 
{
	const char *inputfile = "plaintext.txt";
	const char *outputfile = "encrypted.txt";

	// declare data and memory objects
	std::vector<char> input;
	std::vector<char> output;
	std::vector<char> encrypted;
	std::vector<char> decrypted;

	// read file
	char ch;
	std::ifstream fin; 
	fin.open(inputfile);
	while (fin >> std::noskipws >> ch)
	{
		input.push_back(ch);
	}
	int LENGTH = input.size();
	output.resize(LENGTH);
	fin.close();

	int n;
	std::cout << "Input valid n value: ";
	std::cin >> n;
	if (!std::cin)
	{
		exit(0);
	}
	std::cout << std::endl;

	// ENCRYPT ==========================

	if (n > 0)
	{
		for (int i = 0; i < LENGTH; i++) 
		{
			output[i] = toupper(input[i]);
			if (output[i] >= 65 && output[i] <= 90 - n)
				output[i] = output[i] + n;
			else if (output[i] <= 90 && output[i] >= 91 - n)
				output[i] = output[i] - 26 + n;
		}
	}
	else if (n < 0)
	{
		for (int i = 0; i < LENGTH; i++)
		{
			output[i] = toupper(input[i]);
			if (output[i] <= 90 && output[i] >= 65 - n)
				output[i] = output[i] + n;
			else if (output[i] >= 65 && output[i] >= 65 + n)
				output[i] = output[i] + 26 + n;
		}
	}
	else
	{
		for (int i = 0; i < LENGTH; i++)
		{
			output[i] = toupper(input[i]);
		}
	}
	// write to file
	std::ofstream fout;
	fout.open(outputfile);
	if (!fout)
		std::cout << "Opening file failed" << std::endl;
	for (int i = 0; i < LENGTH; i++)
	{
		fout << output[i];
	}
	if (!fout)
		std::cout << "Write Failed!" << std::endl;
	else
		std::cout << "Write Success!" << std::endl;
	std::cout << "--------------------" << std::endl;
	fout.close();
	std::cout << "Encrypted -----------------" << std::endl;
	for (int i = 0; i < LENGTH; i++)
	{
		std::cout << output[i];
	}

	// DECRYPT ==========================

	// read file
	std::ifstream fin1;
	fin1.open(outputfile);
	while (fin1 >> std::noskipws >> ch)
	{
		encrypted.push_back(ch);
	}
	LENGTH = encrypted.size();
	decrypted.resize(LENGTH);
	fin.close();

	if (n > 0)
	{
		for (int i = 0; i < LENGTH; i++)
		{
			decrypted[i] = encrypted[i];
			if (decrypted[i] >= 65+n && decrypted[i] <= 90)
				decrypted[i] = decrypted[i] - n;
			else if (decrypted[i] >= 65 && decrypted[i] <= 64 + n)
				decrypted[i] = decrypted[i] + 26 - n;
		}
	}
	else if (n < 0)
	{
		for (int i = 0; i < LENGTH; i++)
		{
			decrypted[i] = encrypted[i];
			if (decrypted[i] >= 65 && decrypted[i] <= 90 + n)
				decrypted[i] = decrypted[i] - n;
			else if (decrypted[i] <= 90 && decrypted[i] >= 91 + n)
				decrypted[i] = decrypted[i] - 26 - n;
		}
	}
	else
	{
		for (int i = 0; i < LENGTH; i++)
		{
			decrypted[i] = toupper(encrypted[i]);
		}
	}

	std::cout << "Decrypted ----------------------" << std::endl;
	for (int i = 0; i < LENGTH; i++)
	{
		std::cout << decrypted[i];
	}

	// check output
	bool output_check = TRUE;
	for (int i = 0; i < LENGTH; i++)
	{
		if (toupper(input[i]) != decrypted[i])
		{
			std::cout << "Error at: " << i << std::endl;
			output_check = FALSE;
		}
	}
	if (output_check)
	{
		std::cout << std::endl << "Successfully processed " << LENGTH << " elements." << std::endl << std::endl;
	}

#ifdef _WIN32
	// wait for a keypress on Windows OS before exiting
	std::cout << "\npress a key to quit...";
	std::cin.ignore();
	getchar();
#endif

	return 0;
}
