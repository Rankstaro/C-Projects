/*	Name: Oliver Rankin-Starcevic
 *	Num: 5800559
 *	Email: ors549@uowmail.edu.au
 *	Assignment: 2-1
 *	Due: 23/09/2019
 *	Citations: Casey Chow - Tutorial 5, 4, Lecture 5
 */

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions

// C++ standard library and STL headers
#include <iostream>
#include <vector>
#include <fstream>
#include <time.h>

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
	cl::Kernel kernel;				// a single kernel object
	cl::CommandQueue queue;			// commandqueue for a context and device

	// declare data and memory objects
	std::vector<cl_int> array1(8);
	std::vector<cl_int> array2(16);
	std::vector<cl_int> output(32);
	std::vector<cl_int> correct(8);

	cl::Buffer bufferA, bufferB, outputBuffer;

	// initialise values
	array2 = { 1, 2, 3, 4, 5, 6, 7, 8, -8, -7, -6, -5, -4, -3, -2, -1 };
	srand(time(NULL));
	bool gt5 = false;
	for (int i = 0; i < 8; i++) 
	{
		array1[i] = rand() % 10; // set vector a random values between 0 and 9
		if (array1[i] > 5)
			gt5 = true;
	}
	if (gt5) //one of elements > 5
	{
		for (int i = 0; i < 8; i++)
		{
			if (array1[i] > 5)
				correct[i] = array2[i];
			else
				correct[i] = array2[i+8];
		}
	}
	else
	{
		for (int i = 0; i < 4; i++)
			correct[i] = array2[i];
		for (int i = 4; i < 8; i++)
			correct[i] = array2[i+4];
	}

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
		if(!build_program(&program, &context, "vecbasic.cl")) 
		{
			// if OpenCL program build error
			quit_program("OpenCL program build error.");
		}

		// create a kernel
		kernel = cl::Kernel(program, "vecbasic");

		// create command queue
		queue = cl::CommandQueue(context, device);

		// create buffers
		bufferA = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * 8, &array1[0]);
		bufferB = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int) * 16, &array2[0]);
		outputBuffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_int) * 32);

		// set kernel arguments
		kernel.setArg(0, bufferA);
		kernel.setArg(1, bufferB);
		kernel.setArg(2, outputBuffer);

		cl::NDRange offset(0);
		cl::NDRange globalSize(2);	// work-units per kernel
		cl::NDRange localSize(2);

		queue.enqueueNDRangeKernel(kernel, offset, globalSize, localSize);
			   
		std::cout << "Kernel enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		// enqueue command to read from device to host memory
		queue.enqueueReadBuffer(outputBuffer, CL_TRUE, 0, sizeof(cl_int) * 32, &output[0]);

		// check the outputs
		cl_bool output_check = CL_TRUE;
		std::cout << "Array 1: v" << std::endl;
		for (int i = 0; i < 8; i++) 
		{
			std::cout << output[i] << ", ";
			if (output[i] != array1[i])
			{
				output_check = false;
				std::cout << "Error at: " << i << std::endl;
			}
		}
		std::cout << std::endl << "Array 2: v1" << std::endl;
		for (int i = 8; i < 16; i++)
		{
			std::cout << output[i] << ", ";
			if (output[i] != array2[i-8])
			{
				output_check = false;
				std::cout << "Error at: " << i << std::endl;
			}
		}
		std::cout << std::endl << "Array 2: v2" << std::endl;
		for (int i = 16; i < 24; i++)
		{
			std::cout << output[i] << ", ";
			if (output[i] != array2[i-8])
			{
				output_check = false;
				std::cout << "Error at: " << i << std::endl;
			}
		}
		std::cout << std::endl << "Results:" << std::endl;
		for (int i = 24; i < 32; i++)
		{
			std::cout << output[i] << ", ";
			if (output[i] != correct[i-24])
			{
				output_check = false;
				std::cout << "Error at: " << i << std::endl;
			}
		}
		std::cout << std::endl;
		if (output_check)
		{
			std::cout << std::endl << "Successfully processed " << 32 << " elements." << std::endl << std::endl;
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
#endif

	return 0;
}
