/*	Name: Oliver Rankin-Starcevic
 *	Num: 5800559
 *	Email: ors549@uowmail.edu.au
 *	Assignment: 2-3
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

#define LENGTH 100000000

int main(void) 
{
	cl::Platform platform;			// device's platform
	cl::Device device;				// device used
	cl::Context context;			// context for the device
	cl::Program program;			// OpenCL program object
	cl::Kernel kernel;				// a single kernel object
	cl::CommandQueue queue;			// commandqueue for a context and device

	// declare data and memory objects
	cl::Buffer attemptBuffer;
	std::vector<cl_int> correct (8);

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
		if (!build_program(&program, &context, "bruteforce.cl"))
		{
			// if OpenCL program build error
			quit_program("OpenCL program build error.");
		}

		// create a kernel
		kernel = cl::Kernel(program, "makeCode");

		// create command queue
		queue = cl::CommandQueue(context, device);

		// create buffers
		attemptBuffer = cl::Buffer(context, CL_MEM_WRITE_ONLY, sizeof(cl_char) * LENGTH);
	
		// set kernel arguments
		kernel.setArg(0, attemptBuffer);

		cl::NDRange offset(0);
		cl::NDRange globalSize(1);	// work-units per kernel

		queue.enqueueNDRangeKernel(kernel, offset, globalSize);

		std::cout << "Kernel enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		// enqueue command to read from device to host memory
		queue.enqueueReadBuffer(attemptBuffer, CL_TRUE, 0, sizeof(cl_char) * 32, &correct[0]);
		std::cout << "Combination = ";
		for (int i = 0; i < 8; i++)
			std::cout << correct[i];

		// check output
		cl_bool output_check = CL_TRUE;
		if (output_check)
		{
			std::cout << std::endl << "Successfully processed " << LENGTH << " elements." << std::endl << std::endl;
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
