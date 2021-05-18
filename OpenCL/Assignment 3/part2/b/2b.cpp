/*	Name: Oliver Rankin-Starcevic
 *	Num: 5800559
 *	Email: ors549@uowmail.edu.au
 *	Assignment: 3-2b
 *	Due: 28/10/2019
 *	Citations: Casey Chow - Tutorial 9
 */

#define CL_USE_DEPRECATED_OPENCL_2_0_APIS	// using OpenCL 1.2, some functions deprecated in OpenCL 2.0
#define __CL_ENABLE_EXCEPTIONS				// enable OpenCL exemptions

// C++ standard library and STL headers
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

// OpenCL header, depending on OS
#ifdef __APPLE__
#include <OpenCL/cl.hpp>
#else
#include <CL/cl.hpp>
#endif

#include "common.h"

std::vector<cl_float> read_bmp_rgb(const char *filename, int *size);

int main(void) 
{
	cl::Platform platform;			// device's platform
	cl::Device device;				// device used
	cl::Context context;			// context for the device
	cl::Program program;			// OpenCL program object
	cl::Kernel kernel;			// kernel objects
	cl::CommandQueue queue;			// commandqueue for a context and device

	// declare data and memory objects
	std::vector<cl_float> data, scalarSum;
	cl::Buffer dataBuffer, scalarBuffer;	 
	cl::LocalSpaceArg localSpace;				// to create local space for the kernel
	cl_int numOfGroups;							// number of work-groups
	cl_float sum, avg, correctAvg = 0;					// results
	size_t workgroupSize;						// work group size
    size_t kernelWorkgroupSize;                 // allowed work group size for the kernel
	cl_ulong localMemorySize;					// device's local memory size
	int imgSize;

	const char* filename = "BunnyCity1.bmp";
	// initialise values
	data = read_bmp_rgb(filename, &imgSize);

	// calculate the correct result
	if (filename == "BunnyCity1.bmp")
		correctAvg = 101.49f;
	else if (filename == "BunnyCity2.bmp")
		correctAvg = 116.566f;

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
		if(!build_program(&program, &context, "reduction.cl")) 
		{
			// if OpenCL program build error
			quit_program("OpenCL program build error.");
		}

		// create a kernel
		kernel = cl::Kernel(program, "reduction_scalar");

		// create command queue
		queue = cl::CommandQueue(context, device);

		// get device information
		workgroupSize = device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>();
		localMemorySize = device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>();
        kernelWorkgroupSize = kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(device);

        // if kernel only allows one work-item per work-group, abort
        if (kernelWorkgroupSize == 1)
            quit_program("Abort: Cannot run reduction kernel, because kernel workgroup size is 1.");
        
        // if allowed kernel work group size smaller than device's max workgroup size
        if (workgroupSize > kernelWorkgroupSize)
            workgroupSize = kernelWorkgroupSize;

        // ensure sufficient local memory is available
		while (localMemorySize < sizeof(float) * workgroupSize * 4)
		{
			workgroupSize /= 4;
		}

		// compute number of groups and resize vectors
		numOfGroups = imgSize / workgroupSize;
		scalarSum.resize(numOfGroups);

		for (int i = 0; i < numOfGroups; i++)
		{
			scalarSum[i] = 0.0f;
		}

		// create buffers
		dataBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * imgSize, &data[0]);
		scalarBuffer = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * numOfGroups, &scalarSum[0]);

		cl::NDRange offset(0);
		cl::NDRange globalSize(0);
		cl::NDRange localSize(workgroupSize);

		// set kernel arguments
		localSpace = cl::Local(sizeof(float) * workgroupSize);
		kernel.setArg(0, dataBuffer);
		kernel.setArg(1, localSpace);
		kernel.setArg(2, scalarBuffer);
		globalSize = imgSize;
			
		// enqueue kernel for execution
		queue.enqueueNDRangeKernel(kernel, offset, globalSize, localSize, NULL);

		std::cout << "Kernel enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		// enqueue command to read from device to host memory
		queue.enqueueReadBuffer(scalarBuffer, CL_TRUE, 0, sizeof(cl_float) * numOfGroups, &scalarSum[0]);

		sum = 0.0f;
		for (int i = 0; i < numOfGroups; i++)
		{
			sum += scalarSum[i];
		}
		avg = sum / (imgSize / 3);
		std::cout << avg;
		// check results
		if ( avg < correctAvg - 0.5f || avg > correctAvg + 0.5f)
		{
			std::cout << "Check failed." << std::endl;
		}
		else
		{
			std::cout << "Check passed." << std::endl;
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

std::vector<cl_float> read_bmp_rgb(const char *filename, int *size)
{
	char fileHeader[54];	// to store the file header, bmp file format bmpheader (14 bytes) + bmpheaderinfo (40 bytes) = 54 bytes 
	int width, height;		// width and height of image
	int offset;				// offset where image data starts in the file
	int imageSize;			// image size in bytes
	int padSize;			// in the bmp file format, each row must be a multiple of 4
	int rowSize;			// size per row in bytes
	int i, j;
	char colour[3];
	std::vector<cl_float> imageData;	// pointer to store image data

	// open file stream
	std::ifstream textureFileStream(filename, std::ios::in | std::ios::binary);

	// check whether file opened successfully
	if (!textureFileStream.is_open())
	{
		std::cout << "Failed to open texture file - " << filename << std::endl;
		return imageData;
	}

	// get file header
	textureFileStream.read(fileHeader, 54);

	// get offset, width and height information
	offset = fileHeader[10];
	width = fileHeader[21] << 24;
	width |= fileHeader[20] << 16;
	width |= fileHeader[19] << 8;
	width |= fileHeader[18];
	width = abs(width);
	height = fileHeader[25] << 24;
	height |= fileHeader[24] << 16;
	height |= fileHeader[23] << 8;
	height |= fileHeader[22];
	height = abs(height);

	// allocate RGB image data
	imageSize = width * height * 3;
	imageData.resize(imageSize);

	// move read position by offset
	textureFileStream.seekg(offset, std::ios::beg);

	// compute amount of row padding
	padSize = width % 4;
	if (padSize != 0) {
		padSize = 4 - padSize;
	}

	// bitmaps are stored in upside-down raster order
	rowSize = width * 3;

	// read each RGB pixel colour
	for (i = 0; i < height; i++) {
		for (j = 0; j < rowSize; j += 3) {
			textureFileStream.read(colour, 3);
			imageData[i*rowSize + j] = (cl_float)(0.299*(unsigned char)colour[0]);
			imageData[i*rowSize + j + 1] = (cl_float)(0.587*(unsigned char)colour[1]);
			imageData[i*rowSize + j + 2] = (cl_float)(0.114*(unsigned char)colour[2]);
		}
		// in the bmp format, each row has to be a multiple of 4
		// read in the junk data and throw it away
		for (j = 0; j < padSize; j++) {
			textureFileStream.read(colour, 3);
		}
	}

	// close file stream
	textureFileStream.close();

	*size = imageSize;
	return imageData;
}
