/*	Name: Oliver Rankin-Starcevic
 *	Num: 5800559
 *	Email: ors549@uowmail.edu.au
 *	Assignment: 3-2a
 *	Due: 28/10/2019
 *	Citations: Casey Chow - Tutorial 8a
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

void calc_luminance(const char *filename);

int main(void) 
{
	// calcualte luminance
	calc_luminance("BunnyCity1.bmp");

#ifdef _WIN32
	// wait for a keypress on Windows OS before exiting
	std::cout << "\npress a key to quit...";
	std::cin.ignore();
#endif

	return 0;
}

void calc_luminance(const char *filename)
{
	char fileHeader[54];	// to store the file header, bmp file format bmpheader (14 bytes) + bmpheaderinfo (40 bytes) = 54 bytes 
	int width, height;		// width and height of image
	int offset;				// offset where image data starts in the file
	int imageSize;			// image size in bytes
	int padSize;			// in the bmp file format, each row must be a multiple of 4
	int rowSize;			// size per row in bytes
	int i, j;
	float totLuminance = 0;
	char colour[3];

	// open file stream
	std::ifstream textureFileStream(filename, std::ios::in | std::ios::binary);

	// check whether file opened successfully
	if (!textureFileStream.is_open())
	{
		std::cout << "Failed to open texture file - " << filename << std::endl;
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

	// allocate RGBA image data
	imageSize = width * height;

	// move read position by offset
	textureFileStream.seekg(offset, std::ios::beg);

	// compute amount of row padding
	padSize = width % 4;
	if (padSize != 0) {
		padSize = 4 - padSize;
	}

	// bitmaps are stored in upside-down raster order
	rowSize = width * 4;

	// read each RGB pixel colour
	for (i = 0; i < height; i++) {
		for (j = 0; j < rowSize; j += 4) {
			textureFileStream.read(colour, 3);
			totLuminance += 0.299 * (unsigned char)colour[0] + 0.587 * (unsigned char)colour[1] + 0.114 * (unsigned char)colour[2];
		}
		// in the bmp format, each row has to be a multiple of 4
		// read in the junk data and throw it away
		for (j = 0; j < padSize; j++) {
			textureFileStream.read(colour, 3);
		}
	}

	std::cout << "Avg Luminance: " << totLuminance/imageSize;

	// close file stream
	textureFileStream.close();
}

// function to quit program
void quit_program(const std::string str)
{
	std::cout << str << std::endl;
	std::cout << "Exiting the program..." << std::endl;

#ifdef _WIN32
	// wait for a keypress on Windows OS before exiting
	std::cout << "\npress a key to quit...";
	std::cin.ignore();
#endif

	exit(1);
}
