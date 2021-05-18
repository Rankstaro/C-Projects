__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | 
      CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST; 

// 3x3 Gaussian Blurring filter
__constant float GaussianFilter1[3] = {0.27901, 0.44198, 0.27901};
// 5x5 Gaussian Blurring filter
__constant float GaussianFilter2[5] = {0.06136, 0.24477, 0.38774, 0.24477, 0.06136};
// 7x7 Gaussian Blurring filter
__constant float GaussianFilter3[7] = {0.00598, 0.060626, 0.241843, 0.383103, 0.241843, 0.060626, 0.00598};

__kernel void gauss_conv(read_only image2d_t src_image,
					write_only image2d_t dst_image, 
					__global int *window,
					__global int *direction ) {
	__private int w = window[get_global_id(2)];
	__private int d = direction[get_global_id(2)];

	/* Get work-item’s row and column position */
	int column = get_global_id(0); 
	int row = get_global_id(1);

	/* Accumulated pixel value */
	float4 sum = (float4)(0.0);

	/* Filter's current index */
	int filter_index =  0;

	int2 coord;
	float4 pixel;

	if (d == 0) {
		coord.x = column;
		/* Iterate over the rows */
		for(int i = -w; i <= w; i++) {
			coord.y =  row + i;
		
			/* Read value pixel from the image */ 		
			pixel = read_imagef(src_image, sampler, coord);
			/* Acculumate weighted sum */ 
			if (w == 1)
				sum.xyz += pixel.xyz * GaussianFilter1[filter_index++];
			else if (w == 2)
				sum.xyz += pixel.xyz * GaussianFilter2[filter_index++];
			else if (w == 3)
				sum.xyz += pixel.xyz * GaussianFilter3[filter_index++];
		}
	}
	else {
		/* Iterate over the columns */
		coord.y = row;
		for(int j = -w; j <= w; j++) {
			coord.x = column + j;

			/* Read value pixel from the image */ 		
			pixel = read_imagef(src_image, sampler, coord);
			/* Acculumate weighted sum */ 
			if (w == 1)
				sum.xyz += pixel.xyz * GaussianFilter1[filter_index++];
			else if (w == 2)
				sum.xyz += pixel.xyz * GaussianFilter2[filter_index++];
			else if (w == 3)
				sum.xyz += pixel.xyz * GaussianFilter3[filter_index++];
		}
	}

	/* Write new pixel value to output */
	coord = (int2)(column, row); 
	write_imagef(dst_image, coord, sum);
}