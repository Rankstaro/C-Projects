/*	Name: Oliver Rankin-Starcevic
 *	Num: 5800559
 *	Email: ors549@uowmail.edu.au
 *	Assignment: 3-3
 *	Due: 28/10/2019
 *	Citations: Casey Chow - Tutorial 8d
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
#include "bmpfuncs.h"

int main(void) 
{
	cl::Platform platform;			// device's platform
	cl::Device device;				// device used
	cl::Context context;			// context for the device
	cl::Program program;			// OpenCL program object
	cl::Kernel lumKernel, gausKernel, addKernel;	// a single kernel object
	cl::CommandQueue queue;			// commandqueue for a context and device

	// declare data and memory objects
	unsigned char* inputImage;
	unsigned char* outputImage;
	int imgWidth, imgHeight, imageSize;
	const char* imageFile = "bunnyCity2.bmp";
	cl_float threshold;
	if (imageFile == "bunnyCity1.bmp")
		threshold = 101.49;
	else if (imageFile == "bunnyCity2.bmp")
		threshold = 116.566;

	cl::ImageFormat imgFormat;
	cl::Image2D inputImgBuffer, input2ImgBuffer, outputImgBuffer;
	cl::Buffer windowBuffer, directionBuffer, thresholdBuffer;

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
		if(!build_program(&program, &context, "bloom.cl")) 
		{
			// if OpenCL program build error
			quit_program("OpenCL program build error.");
		}

		// window value
		cl_int window;
		std::cout << "Input valid window value (1 = 3x3, 2 = 5x5, 3 = 7x7): ";
		std::cin >> window;
		if (window < 1 || window > 3)
			quit_program("Invalid n value.");
		std::cout << std::endl;
		
		// threshold value
		cl_float thresholdInput;
		std::cout << "Input valid threshold value (0-255), or -1 to use default: ";
		std::cin >> thresholdInput;
		if (thresholdInput < 0 || thresholdInput > 255)
			cout << "Using default threshold value";
		else
			threshold = thresholdInput;
		std::cout << std::endl;
		
		// create kernels
		lumKernel = cl::Kernel(program, "lum_area");
		gausKernel = cl::Kernel(program, "gauss_conv");
		addKernel = cl::Kernel(program, "add_imgs");

		// create command queue
		queue = cl::CommandQueue(context, device);
		
		// read input image
		inputImage = read_BMP_RGB_to_RGBA(imageFile, &imgWidth, &imgHeight);

		// allocate memory for output image
		imageSize = imgWidth * imgHeight * 4;
		outputImage = new unsigned char[imageSize];

		// image format
		imgFormat = cl::ImageFormat(CL_RGBA, CL_UNORM_INT8);

		// Set offset and globalsize
		cl::NDRange offset(0, 0);
		cl::NDRange globalSize(imgWidth, imgHeight);

		// enqueue command to read image from device to host memory
		cl::size_t<3> origin, region;
		origin[0] = origin[1] = origin[2] = 0;
		region[0] = imgWidth;
		region[1] = imgHeight;
		region[2] = 1;

		// Set luminance areas

		inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
		outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);
		thresholdBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_float), &threshold);

		lumKernel.setArg(0, inputImgBuffer);
		lumKernel.setArg(1, outputImgBuffer);
		lumKernel.setArg(2, thresholdBuffer);

		queue.enqueueNDRangeKernel(lumKernel, offset, globalSize);

		std::cout << "Luminance kernel enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		// enqueue command to read image from device to host memory
		queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);
		
		// Gaussian blur kernels:

		cl_int direction = 0; // vertical pass

		// create image objects
		inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);
		outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);
		windowBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int), &window);
		directionBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int), &direction);

		// set kernel arguments
		gausKernel.setArg(0, inputImgBuffer);
		gausKernel.setArg(1, outputImgBuffer);
		gausKernel.setArg(2, windowBuffer);
		gausKernel.setArg(3, directionBuffer);
		
		queue.enqueueNDRangeKernel(gausKernel, offset, globalSize);

		std::cout << "Vertical pass enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		// enqueue command to read image from device to host memory
		queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);

		direction = 1; // horizontal pass

		// create image objects
		inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);
		outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);
		windowBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int), &window);
		directionBuffer = cl::Buffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(cl_int), &direction);

		// set kernel arguments
		gausKernel.setArg(0, inputImgBuffer);
		gausKernel.setArg(1, outputImgBuffer);
		gausKernel.setArg(2, windowBuffer);
		gausKernel.setArg(3, directionBuffer);

		queue.enqueueNDRangeKernel(gausKernel, offset, globalSize);

		std::cout << "Horizontal pass enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		// enqueue command to read image from device to host memory
		queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);
		
		// Add images together

		// create image objects
		inputImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);
		input2ImgBuffer = cl::Image2D(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)inputImage);
		outputImgBuffer = cl::Image2D(context, CL_MEM_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, imgFormat, imgWidth, imgHeight, 0, (void*)outputImage);

		// set kernel arguments
		addKernel.setArg(0, inputImgBuffer);
		addKernel.setArg(1, input2ImgBuffer);
		addKernel.setArg(2, outputImgBuffer);

		queue.enqueueNDRangeKernel(addKernel, offset, globalSize);

		std::cout << "Add images kernel enqueued." << std::endl;
		std::cout << "--------------------" << std::endl;

		// enqueue command to read image from device to host memory
		queue.enqueueReadImage(outputImgBuffer, CL_TRUE, origin, region, 0, 0, outputImage);

		// output results to image file
		const char * outputFile = "output.bmp";
		if (window == 1)
			outputFile = "output1.bmp";
		if (window == 2)
			outputFile = "output2.bmp";
		if (window == 3)
			outputFile = "output3.bmp";
		write_BMP_RGBA_to_RGB(outputFile, outputImage, imgWidth, imgHeight);

		std::cout << "Done." << std::endl;

		// deallocate memory
		free(inputImage);
		free(outputImage);
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
