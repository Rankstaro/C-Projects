/*	Name: Oliver Rankin-Starcevic
 *	Num: 5800559
 *	Email: ors549@uowmail.edu.au
 *	Assignment: 1
 *	Due: 23/08/2019
 *	Citations: Casey Chow - Tutorial 1c, Tutorial 2b/c
 */

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions

// C++ standard library and STL headers
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

// OpenCL header, depending on OS
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

// functions to handle errors
#include "error.h"

// function prototypes
bool select_one_device(cl::Platform* platfm, cl::Device* dev, int type);
bool build_program(cl::Program* prog, const cl::Context* ctx, const std::string filename);

int main(void)
{
	cl::Platform platform;		// device's platform
	cl::Device device;			// device used
	cl::Context context;		// context for the device
	cl::CommandQueue queue;		// commandqueue for a context and device
	cl::Program program;			// OpenCL program object
	cl::Kernel kernel;				// a single kernel object
	unsigned int i;				// counter
	int type;					// device type
	std::string outputString;	// string for output
	
	try {
		// input type
		std::cout << "Types:" << std::endl
			<< "\t0 - CPUs" << std::endl
			<< "\t1 - GPUs" << std::endl
			<< "--------------------" << std::endl
			<< "Select type: ";
		std::cin >> type;

		// select an OpenCL device
		if (!select_one_device(&platform, &device, type))
		{
			// if no device selected
			quit_program("Device not selected.");
		}

		std::cout << "--------------------" << std::endl;
		// Check whether the device supports the cl_khr_icd extension
		outputString = device.getInfo<CL_DEVICE_EXTENSIONS>();
		if (outputString.find("cl_khr_icd") != std::string::npos)
			std::cout << device.getInfo<CL_DEVICE_NAME>() << " supports the cl_khr_icd extension." << std::endl;
		else
			std::cout << device.getInfo<CL_DEVICE_NAME>() << " does not support the cl_khr_icd extension." << std::endl;

		// create a context for device
		context = cl::Context(device);

		// create command queue for device
		queue = cl::CommandQueue(context, device);

		// build the program
		if (!build_program(&program, &context, "source.cl"))
		{
			// if OpenCL program build error
			quit_program("OpenCL program build error.");
		}

		std::vector<cl::Kernel> allKernels;		// all kernels

		// create all kernels in the program
		program.createKernels(&allKernels);

		std::cout << "Number of Kernels: " << program.getInfo<CL_PROGRAM_NUM_KERNELS>() << std::endl;
		std::cout << "Kernel names: " << std::endl;

		// output kernel name for each index
		for (i = 0; i < allKernels.size(); i++) {
			outputString = allKernels[i].getInfo<CL_KERNEL_FUNCTION_NAME>();
			std::cout << "Kernel " << i << ": " << outputString << std::endl;
		}

		std::cout << "--------------------" << std::endl;

		// Press enter to quit...
		std::getline(std::cin, outputString);
	}
	// catch any OpenCL function errors
	catch (cl::Error e) {
		// call function to handle errors
		handle_error(e);
	}

#ifdef _WIN32
	// wait for a keypress on Windows OS before exiting
	std::cout << "\nPress enter to quit...";
	std::cin.ignore();
#endif

	return 0;
}

bool select_one_device(cl::Platform* platfm, cl::Device* dev, int type)
{
	std::vector<cl::Platform> platforms;					// available platforms
	std::vector< std::vector<cl::Device> > platformDevices;	// devices available for each platform
	unsigned int i, j;										// counters
	std::string outputString;								// string for output

	try {
		// get the number of available OpenCL platforms
		cl::Platform::get(&platforms);

		// find and store the devices available to each platform based on type
		for (i = 0; i < platforms.size(); i++)
		{
			std::vector<cl::Device> devices;		// available devices
			if (type == 0)
				platforms[i].getDevices(CL_DEVICE_TYPE_CPU, &devices); // get all CPU devices available to the platform
			else
				platforms[i].getDevices(CL_DEVICE_TYPE_GPU, &devices); // get all GPU devices available to the platform

			// store available devices for the platform based on type
			platformDevices.push_back(devices);
		}

		// display available platforms and devices
		std::cout << "--------------------" << std::endl;
		std::cout << "Available options:" << std::endl;

		// store options as platform and device indices
		std::vector< std::pair<int, int> > options;
		unsigned int optionCounter = 0;	// option counter

		// for all platforms
		for (i = 0; i < platforms.size(); i++)
		{
			// for all devices per platform
			for (j = 0; j < platformDevices[i].size(); j++)
			{
				// display options
				std::cout << std::endl << "Option " << optionCounter << ": " << std::endl;

				// get platform vendor name
				outputString = platforms[i].getInfo<CL_PLATFORM_VENDOR>();
				std::cout << "\tPlatform - " << i << ": " << outputString << std::endl;

				// get and output device type
				cl_device_type type;
				platformDevices[i][j].getInfo(CL_DEVICE_TYPE, &type);
				if (type == CL_DEVICE_TYPE_CPU)
					std::cout << "\tType: " << "CPU" << std::endl;
				else if (type == CL_DEVICE_TYPE_GPU)
					std::cout << "\tType: " << "GPU" << std::endl;
				else
					std::cout << "\tType: " << "Other" << std::endl;

				// get device name
				outputString = platformDevices[i][j].getInfo<CL_DEVICE_NAME>();
				std::cout << "\tDevice - " << j << ": " << outputString << std::endl;

				// get # of compute units
				outputString = std::to_string(platformDevices[i][j].getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>());
				std::cout << "\t# of Compute Units: " << outputString << std::endl;

				// max work group size
				outputString = std::to_string(platformDevices[i][j].getInfo <CL_DEVICE_MAX_WORK_GROUP_SIZE>());
				std::cout << "\tMax Work Group Size: " << outputString << std::endl;

				// Max work item size
				std::vector<size_t> wiSize = platformDevices[i][j].getInfo <CL_DEVICE_MAX_WORK_ITEM_SIZES>();
				outputString = std::to_string(wiSize[0]);
				for (int w = 1; w < wiSize.size(); w++)
					outputString += " x " + std::to_string(wiSize[w]);
				std::cout << "\tMax Work Item Size: " << outputString << std::endl;

				//local memory size
				outputString = std::to_string(platformDevices[i][j].getInfo <CL_DEVICE_LOCAL_MEM_SIZE>());
				std::cout << "\tLocal Memory Size: " << outputString << std::endl;
				
				// store option
				options.push_back(std::make_pair(i, j));
				optionCounter++; // increment option counter
			}
		}

		std::cout << "--------------------" << std::endl;
		std::cout << "Select a device: ";

		std::string inputString;
		unsigned int selectedOption;	// option that was selected

		std::cin >> inputString;
		std::istringstream stringStream(inputString);

		// check whether valid option was selected
		// check if input was an integer
		if (stringStream >> selectedOption)
		{
			char c;

			// check if there was anything after the integer
			if (!(stringStream >> c))
			{
				// check if valid option range
				if (selectedOption >= 0 && selectedOption < optionCounter)
				{
					// return the platform and device
					int platformNumber = options[selectedOption].first;
					int deviceNumber = options[selectedOption].second;

					*platfm = platforms[platformNumber];
					*dev = platformDevices[platformNumber][deviceNumber];

					return true;
				}
			}
		}
		// if invalid option selected
		std::cout << "\n--------------------" << std::endl;
		std::cout << "Invalid option." << std::endl;
	}
	// catch any OpenCL function errors
	catch (cl::Error e) {
		// call function to handle errors
		handle_error(e);

		std::getline(std::cin, outputString);
	}

	return false;
}

// builds program from given filename
bool build_program(cl::Program* prog, const cl::Context* ctx, const std::string filename)
{
	// get devices from the context
	std::vector<cl::Device> contextDevices = ctx->getInfo<CL_CONTEXT_DEVICES>();

	// open input file stream to .cl file
	std::ifstream programFile(filename);

	// check whether file was opened
	if (!programFile.is_open())
	{
		std::cout << "File not found." << std::endl;
		return false;
	}

	// create program string and load contents from the file
	std::string programString(std::istreambuf_iterator<char>(programFile), (std::istreambuf_iterator<char>()));

	// create program source from one input string
	cl::Program::Sources source(1, std::make_pair(programString.c_str(), programString.length() + 1));
	// create program from source
	*prog = cl::Program(*ctx, source);

	// try to build program
	try {
		// build the program for the devices in the context
		prog->build(contextDevices);

		std::cout << "Program build: Successful" << std::endl;
		std::cout << "--------------------" << std::endl;

		// check build status for all all devices in context
		for (unsigned int i = 0; i < contextDevices.size(); i++)
		{
			// get build log
			std::string build_log = prog->getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[i]);

			std::cout << "Build log:" << std::endl;
			std::cout << build_log << "\n--------------------" << std::endl;
		}
	}
	catch (cl::Error e) {
		// if failed to build program
		if (e.err() == CL_BUILD_PROGRAM_FAILURE)
		{
			// output program build log
			std::cout << "Program build: Failed" << std::endl;
			std::cout << e.what() << std::endl;
			std::cout << "--------------------" << std::endl;

			// check build status for all all devices in context
			for (unsigned int i = 0; i < contextDevices.size(); i++)
			{
				// get build log
				std::string build_log = prog->getBuildInfo<CL_PROGRAM_BUILD_LOG>(contextDevices[i]);

				std::cout << "Build log:" << std::endl;
				std::cout << build_log << "\n--------------------" << std::endl;
			}

			return false;
		}
		else
		{
			// call function to handle errors
			handle_error(e);
		}
	}

	return true;
}