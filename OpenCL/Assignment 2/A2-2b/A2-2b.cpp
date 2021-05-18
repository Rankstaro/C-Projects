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

#include "common.h"

int main(void) 
{
	cl::Platform platform;			// device's platform
	cl::Device device;				// device used
	cl::Context context;			// context for the device
	cl::Program program;			// OpenCL program object
	cl::Kernel ekernel;				// a single kernel object
	cl::Kernel dkernel;				// a single kernel object
	cl::CommandQueue queue;			// commandqueue for a context and device
	const char *inputfile = "plaintext.txt";
	const char *outputfile = "encrypted.txt";

	// declare data and memory objects
	std::vector<cl_char> input;
	std::vector<cl_char> output;
	std::vector<cl_char> encrypted;
	std::vector<cl_char> decrypted;

	cl::Buffer einputBuffer, eoutputBuffer, enBuffer, dinputBuffer, doutputBuffer, dnBuffer;

	// read file
	cl_char ch;
	std::ifstream fin; 
	fin.open(inputfile);
	while (fin >> std::noskipws >> ch)
	{
		input.push_back(ch);
	}
	int LENGTH = input.size();
	int bufferLength = LENGTH;
	if (LENGTH % 16 != 0)
	{
		bufferLength = LENGTH + (16 - (LENGTH % 16));
		input.resize(bufferLength);
		output.resize(bufferLength);
	}
	fin.close();

	try {
		// select an OpenCL device
		if (!select_one_device(&platform, &device))
		{
			// if no device selected
			quit_program("Device not selected.");
		}

		// create a context from device
		context = cl::Context(device);

		// build the program
		if (!build_program(&program, &context, "cipher.cl"))
		{
			// if OpenCL program build error
			quit_program("OpenCL program build error.");
		}

		cl_int n;
		std::cout << "Input valid n value: ";
		std::cin >> n;
		if (!std::cin)
		{
			quit_program("Invalid n value.");
		}
		std::cout << std::endl;

		// ENCRYPT ==========================

		// create a kernel
		ekernel = cl::Kernel(program, "encrypt");

		// create command queue
		queue = cl::CommandQueue(context, device);

		// create buffers
		einputBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_char) * bufferLength, &input[0]);
		eoutputBuffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_char) * bufferLength);
		enBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int), &n);

		// set kernel arguments
		ekernel.setArg(0, einputBuffer);
		ekernel.setArg(1, eoutputBuffer);
		ekernel.setArg(2, enBuffer);

		cl::NDRange offset(0);
		cl::NDRange globalSize(bufferLength/16);	// work-units per kernel

		queue.enqueueNDRangeKernel(ekernel, offset, globalSize);

		std::cout << "Kernel enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		// enqueue command to read from device to host memory
		queue.enqueueReadBuffer(eoutputBuffer, CL_TRUE, 0, sizeof(cl_char) * bufferLength, &output[0]);
		
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
		bufferLength = LENGTH;
		if (LENGTH % 16 != 0)
		{
			bufferLength = LENGTH + (16 - (LENGTH % 16));
			encrypted.resize(bufferLength);
			decrypted.resize(bufferLength);
		}
		fin.close();

		// create a kernel
		dkernel = cl::Kernel(program, "decrypt");

		// create buffers
		dinputBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_char) * bufferLength, &encrypted[0]);
		doutputBuffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_char) * bufferLength);
		dnBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int), &n);

		// set kernel arguments
		dkernel.setArg(0, dinputBuffer);
		dkernel.setArg(1, doutputBuffer);
		dkernel.setArg(2, dnBuffer);

		queue.enqueueNDRangeKernel(dkernel, offset, globalSize);

		std::cout << "Kernel enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		// enqueue command to read from device to host memory
		queue.enqueueReadBuffer(doutputBuffer, CL_TRUE, 0, sizeof(cl_char) * bufferLength, &decrypted[0]);

		std::cout << "Decrypted ----------------------" << std::endl;
		for (int i = 0; i < LENGTH; i++)
		{
			std::cout << decrypted[i];
		}

		// check output
		cl_bool output_check = CL_TRUE;
		for (int i = 0; i < LENGTH; i++)
		{
			if (toupper(input[i]) != decrypted[i])
			{
				std::cout << "Error at: " << i << std::endl;
				output_check = CL_FALSE;
			}
		}
		if (output_check)
		{
			std::cout << std::endl << "Successfully processed " << bufferLength << " elements." << std::endl << std::endl;
		}
	}
	// catch any OpenCL function errors
	catch (cl::Error e) {
		// call function to handle errors
		handle_error(e);
	}

#ifdef _WIN32
	// wait for a keypress on Windows OS before exiting
	std::cout << "\npress a key to quit...";
	std::cin.ignore();
	getchar();
#endif

	return 0;
}
