__constant sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE | 
      CLK_ADDRESS_CLAMP_TO_EDGE | CLK_FILTER_NEAREST; 

// 3x3 Gaussian Blurring filter
__constant float GaussianFilter1[9] = {	0.077847, 0.123317, 0.077847,
										0.123317, 0.195346, 0.123317,
										0.077847, 0.123317, 0.077847};
// 5x5 Gaussian Blurring filter
__constant float GaussianFilter2[25] = {0.003765, 0.015019, 0.023792, 0.015019, 0.003765,
										0.015019, 0.059912, 0.094907, 0.059912, 0.015019,
										0.023792, 0.094907, 0.150342, 0.094907, 0.023792,
										0.015019, 0.059912, 0.094907, 0.059912, 0.015019,
										0.003765, 0.015019, 0.023792, 0.015019, 0.003765};
// 7x7 Gaussian Blurring filter
__constant float GaussianFilter3[49] = {0.000036, 0.000363, 0.001446, 0.002291, 0.001446, 0.000363, 0.000036,
										0.000363, 0.003676, 0.014662, 0.023226, 0.014662, 0.003676, 0.000363,
										0.001446, 0.014662, 0.058488, 0.092651, 0.058488, 0.014662, 0.001446,
										0.002291, 0.023226, 0.092651, 0.146768, 0.092651, 0.023226, 0.002291,
										0.001446, 0.014662, 0.058488, 0.092651, 0.058488, 0.014662, 0.001446,
										0.000363, 0.003676, 0.014662, 0.023226, 0.014662, 0.003676, 0.000363,
										0.000036, 0.000363, 0.001446, 0.002291, 0.001446, 0.000363, 0.000036};

__kernel void gauss_conv(read_only image2d_t src_image,
					write_only image2d_t dst_image, 
					__global int *window) {
	__private int w = window[get_global_id(2)];

	/* Get work-item’s row and column position */
	int column = get_global_id(0); 
	int row = get_global_id(1);

	/* Accumulated pixel value */
	float4 sum = (float4)(0.0);

	/* Filter's current index */
	int filter_index =  0;

	int2 coord;
	float4 pixel;

	/* Iterate over the rows */
	for(int i = -w; i <= w; i++) {
		coord.y =  row + i;

		/* Iterate over the columns */
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